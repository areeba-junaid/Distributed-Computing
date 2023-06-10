Matrix Multiplication:
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


#define N 2 // Size of matrices


int main()
{
    int rank, size, i, j, k;
    int A[N][N], B[N][N], C[N][N];
    int rows_per_process;


    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    rows_per_process = N / size;


    if (rank == 0) {
        // Initialize matrices A and B
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = i + j;
                B[i][j] = i - j;
            }
        }
    }


    int bufferA[rows_per_process][N];
    int bufferC[rows_per_process][N];


    MPI_Scatter(A, rows_per_process * N, MPI_INT, bufferA, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);


    // Perform matrix multiplication
    for (i = 0; i < rows_per_process; i++) {
        for (j = 0; j < N; j++) {
            bufferC[i][j] = 0;
            for (k = 0; k < N; k++) {
                bufferC[i][j] += bufferA[i][k] * B[k][j];
            }
        }
    }


    MPI_Gather(bufferC, rows_per_process * N, MPI_INT, C, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        // Print the result in process 0
        printf("Matrix A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%3d ", A[i][j]);
            }
            printf("\n");
        }


        printf("\nMatrix B:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%3d ", B[i][j]);
            }
            printf("\n");
        }


        printf("\nMatrix C (Result):\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%3d ", C[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}
