#include<stdio.h>
#include<mpi.h>


#define N 2


int main(int argc, char** argv)
{
    int rank, size, i, j, k;
    int a[N][N], b[N][N], c[N][N], start, end;
    MPI_Init(&argc, &argv); // initiate mpi computation
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // no.of instances
    MPI_Comm_size(MPI_COMM_WORLD, &size); // size of the group


    int bufferA[N/size][N];
    int bufferB[N/size][N];
    int bufferC[N/size][N];
    const int ROOT = 0; // define root process


    if (rank == ROOT)
    {
        // initialize matrices a and b
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                a[i][j] = i + j;
                b[i][j] = i - j;
            }
        }
    }


    int rows_per_process = N / size;


    MPI_Scatter(a, rows_per_process * N, MPI_INT, bufferA, rows_per_process * N, MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Scatter(b, rows_per_process * N, MPI_INT, buffer, rows_per_process * N, MPI_INT, ROOT, MPI_COMM_WORLD);


    // Perform element-wise addition of matrix A and B
    for (i = 0; i < rows_per_process; i++) {
        for (j = 0; j < N; j++) {
            bufferC[i][j] = bufferA[i][j] + bufferB[i][j];
        }
    }


    MPI_Gather(bufferC, rows_per_process * N, MPI_INT, c, rows_per_process * N, MPI_INT, ROOT, MPI_COMM_WORLD);


    if (rank == ROOT) {
        // Print the result in process 0
        printf("Matrix A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%3d ", a[i][j]);
            }
            printf("\n");
        }


        printf("\nMatrix B:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%3d ", b[i][j]);
            }
            printf("\n");
        }


        printf("\nMatrix C (Result):\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%3d ", c[i][j]);
            }
            printf("\n");
        }
    }


    MPI_Finalize();
    return 0;
}



