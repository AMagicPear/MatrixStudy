#include<stdio.h>
#include<stdlib.h>

int Inverse() {
    float **input, **Inverse, localVariable;
    float *temprow;
    int i, j, k, sizeOfMatrix;

    printf("Enter matrix size. It's a square matrix. So enter value of n (nXn)\n");
    scanf("%d", &sizeOfMatrix);
    input = (float **) malloc(sizeOfMatrix * sizeof(float *));

    for (i = 0; i < sizeOfMatrix; i++)
        input[i] = (float *) malloc(sizeOfMatrix * sizeof(float));
    temprow = (float *) malloc(sizeOfMatrix * sizeof(float));
    Inverse = (float **) malloc(sizeOfMatrix * sizeof(float *));

    for (i = 0; i < sizeOfMatrix; i++) {

        Inverse[i] = (float *) malloc(sizeOfMatrix * sizeof(float));
    }

    printf("Now enter the matrix:\n");
    for (i = 0; i < sizeOfMatrix; i++)
        for (j = 0; j < sizeOfMatrix; j++)
            scanf("%f", &input[i][j]);

    for (i = 0; i < sizeOfMatrix; i++)
        for (j = 0; j < sizeOfMatrix; j++)
            if (i == j)
                Inverse[i][j] = 1;
            else
                Inverse[i][j] = 0;


    for (k = 0; k < sizeOfMatrix; k++) {
        if (input[k][k] == 0) {
            for (j = k + 1; j < sizeOfMatrix; j++) {
                if (input[j][k] != 0)
                    break;
            }

            if (j == sizeOfMatrix) {
                printf("\nMATRIX IS NOT INVERSIBLE\n\n");
                return 0;
            }
            temprow = input[k];
            input[k] = input[j];
            input[j] = temprow;
            temprow = Inverse[k];
            Inverse[k] = Inverse[j];
            Inverse[j] = temprow;
        }
        localVariable = input[k][k];
        for (j = 0; j < sizeOfMatrix; j++) {
            input[k][j] /= localVariable;
            Inverse[k][j] /= localVariable;

        }
        for (i = 0; i < sizeOfMatrix; i++) {
            localVariable = input[i][k];
            for (j = 0; j < sizeOfMatrix; j++) {
                if (i == k)
                    break;
                input[i][j] -= input[k][j] * localVariable;
                Inverse[i][j] -= Inverse[k][j] * localVariable;
            }
        }
    }
    printf("The inverse matrix is:\n");
    for (i = 0; i < sizeOfMatrix; i++) {
        for (j = 0; j < sizeOfMatrix; j++)
            printf("%.2f\t", Inverse[i][j]);
        printf("\n");
    }
    return 0;
}