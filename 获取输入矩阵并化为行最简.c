//#define DEBUG
//注意：本程序注释中所写的所有第“n”行或列，n都从0开始取！
#include <stdio.h>
#include <stdlib.h>
//行和列必须是全局变量！否则函数会出错。
int Row, Column;

void SimplificationModeSelection(double array[][Column], int r, int c, int mode);

void PrintArray(double array[][Column], int r, int c, char *string);

void InputArray(double array[][Column], int r, int c);

void RowSimplest(double a[][Column], int r, int c);

void PrintSingleArray(int ar[], int n);

void RowLadder(double a[][Column], int r, int c);

void SwapLines(double a[][Column], int c, int line1, int line2);

void Swap(double *a, double *b);

int Zero_of(const double ar[], int c);

void RowMultiplication(double a[][Column], int c, int dest, int src, double times);

int main() {
#ifdef DEBUG
    Row=3;Column=3;
    double a[100][100];
    InputArray(a,3,3);
    RowSimplest(a,3,3);
    PrintArray(a,3,3,"");
#else
    printf("Please enter the number of rows and columns of the matrix:\n");
    scanf_s("%d%d", &Row, &Column);
    //以用户输入的行列数生成一个二维数组arrayInput以存放矩阵
    double arrayInput[Row + 1][Column + 1];//我也不知道为什么如果定义arrayInput[Row][Column]会出错，就加了1，发现竟然奇迹般地好了
    //让用户输入一个矩阵
    InputArray(arrayInput, Row, Column);
    //把用户输入的矩阵打印出来
    PrintArray(arrayInput, Row, Column, "input");
/*    //让用户选择矩阵化简的模式
    int mode;
    printf("Now select the reduction mode:\n");
    printf("1\t Row stepped matrix\n");//行阶梯形矩阵
    printf("2\t Row simplest matrix\n");//行最简矩阵
    printf("Your choice:");
    scanf_s("%d", &mode);
    //调用化简函数化简矩阵
    SimplificationModeSelection(arrayInput, Row, Column, mode);
    //输出化简后的矩阵
    char *type;
    if (mode == 1) type = "Row stepped";
    if (mode == 2) type = "Row simplest";
    PrintArray(arrayInput, Row, Column, type);*/
    RowSimplest(arrayInput,Row,Column);
    PrintArray(arrayInput,Row,Column,"Row simplest");
    system("pause");
#endif
    return 0;
}

//交换两个数字
void Swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

//交换矩阵的两行
void SwapLines(double a[][Column], int c, int line1, int line2) {
    //依次交换line1行和line2行的每个元素，以交换两行
    for (int k = 0; k < c; ++k)
        Swap(&a[line1][k], &a[line2][k]);
}

//化为行最简
void RowSimplest(double a[][Column], int r, int c) {
    //第一步：化为行阶梯形矩阵且使每行首个非0元为1
    //用ini_r, ini_c表示当前工作坐标，初始放在左上角
    int ini_r = 0, ini_c = 0;
    do {
        //每次循环先转换成行阶梯
        RowLadder(a, r, c);
        //化为行阶梯后，第0行的前面的0的数量应该最少，若此时第0行的首个元素仍为0，说明该列全为0
        //因此针对前面几列全为0的特殊情况，移动ini_c到首个非0列处
        while (a[ini_r][ini_c] == 0)
            ++ini_c;
        //如果行下标为ini_r的行的首个元素不是1
        if (a[ini_r][ini_c] != 1) {
            //通过倍乘使行下标为ini_r的行的首个元素化为1
            for (int ci = c - 1; ci >= ini_c; --ci) {
                a[ini_r][ci] /= a[ini_r][ini_c];
            }
        }
        //用ri遍历ini_r以下的行
        int ri = ini_r + 1;
        //如果行下标大于ini_r的行的首个元素不是0
        if (a[ri][ini_c] != 0) {
            //通过倍加使所有行下标大于ini_r的行的首个化为0
            for (; ri < r; ++ri) {
                for (int ci = c - 1; ci >= ini_c; --ci) {
                    a[ri][ci] -= a[ri][ini_c] / a[ini_r][ini_c] * a[ini_r][ci];
                }
            }
        }
        //完成一次循环后第ini_c列一定是首个为1，其余全为0且能确保ini_c以前的列已经完成化简
        //将下一次循环时的工作坐标向右下移
        ini_r++;
        ini_c++;
    } while (ini_r < r && ini_c < c);//直到全部化简完停止循环
    //此时保证了矩阵是行阶梯且非零行的第一个非零元素是1
    //下面使非零行的第一个非零元素所在列的其余元素全化为零
    //ri和ci用于定位非零行的第一个非零元素的坐标
    int ri;
    int ci = 0;
    //第0列只可能全为0或只有一个1，不需要化简，故ri从1开始取
    for (ri = 1; ri < r; ri++) {
        //在ri行寻找首个非零元素时停止循环，此时的ri和ci即首个非零元素的坐标
        while (a[ri][ci] == 0) ci++;
        //对第ri行上方的行都与第ri列进行倍加，使其的ci列的元素为0
        for (int destLine = 0; destLine < ri; destLine++)
            //将ri行的-a[destLine][ci]/a[ri][ci]倍加到destLine行上
            RowMultiplication(a, c, destLine, ri, -a[destLine][ci] / a[ri][ci]);
    }
}

//返回矩阵中第ar行的开头的0的个数
int Zero_of(const double ar[], int c) {
    //ar代表下标为ar的行，例如ar[1]表示行标为ar列标为1处的元素
    //对ar行的元素从头依次判断是否为0，检测到不为0时返回的列标i即0的个数
    for (int i = 0; i <= c - 1; ++i) {
        if (ar[i] != 0) {
            return i;
        }
    }
    //若整行均为0，则0的个数为列数c
    return c;
}

//化为行阶梯形矩阵
void RowLadder(double a[][Column], int r, int c) {
    //针对0的数量对行排序即可化为行阶梯形矩阵，此处使用的是选择排序法
    for (int k = 0; k < r - 1; ++k) {
        int zero_min = k, i;
        for (i = k + 1; i < r; ++i) {
            if (Zero_of(a[i], c) < Zero_of(a[zero_min], c)) {
                zero_min = i;
            }
        }
        SwapLines(a, c, zero_min, k);
    }
}

//输出长度为n的一维数组
void PrintSingleArray(int ar[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", ar[i]);
    }
    printf("\n");
}

//输出矩阵
void PrintArray(double array[][Column], int r, int c, char *string) {
    printf("The %s matrix is:\n", string);
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            //以两位小数形式输出
            printf("%.2f\t", array[i][j]);
        }
        printf("\n");
    }
}

//输入矩阵
void InputArray(double array[][Column], int r, int c) {
    printf("Please input an array(%d,%d):\n", r, c);
    int i, j;
    for (i = 0; i < r; ++i)
        for (j = 0; j < c; ++j)
            scanf_s("%lf", &array[i][j]);
}

//倍加
void RowMultiplication(double a[][Column], int c, int dest, int src, double times) {
    //将第src列的times倍加到第dest列上
    for (int i = 0; i < c; ++i) {
        a[dest][i] += times * a[src][i];
    }
}

//根据mode的数值选择采用何种方式处理矩阵
void SimplificationModeSelection(double array[][Column], int r, int c, int mode) {
    switch (mode) {
        case 1:
            RowLadder(array, r, c);
            break;
        case 2:
            RowSimplest(array, r, c);
            break;
        default:
            printf("No such mode! Please select the listed above!\n");
    }
}
