#include <mpi.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int process_id;
    int total_processes;


    MPI_Init(&argc, &argv);


    MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);


    int master_process_id = 0;
    if (process_id == master_process_id) {
        int array_size = 6;

        int array_one[array_size];
        int array_two[array_size];
        int array_of_sum[array_size];


        for (int i = 0; i < array_size; i++) {
            array_one[i] = 1;
            array_two[i] = 1;
        }

        int workload = array_size / total_processes;


        int total_sum = 0;


        for (int i = 1; i < total_processes; i++) {
            MPI_Send(&workload, 1, MPI_INT, i, 0, MPI_COMM_WORLD);


            MPI_Send(&array_one[i * workload], workload, MPI_INT, i, 0, MPI_COMM_WORLD);


            MPI_Send(&array_two[i * workload], workload, MPI_INT, i, 0, MPI_COMM_WORLD);


        }

        for (int i = 0; i < workload; i++) {
            array_of_sum[i] = array_one[i] + array_two[i];
        }


        for (int i = 1; i < total_processes; i++) {
            MPI_Recv(&array_of_sum[i * workload], workload, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }


        printf("Vector sum is: \n");
        for (int i = 0; i < array_size; i++) {
            printf("%d\n", array_of_sum[i]);
        }
    }
    else {
        int workload;
        MPI_Recv(&workload, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        int array_one[workload];
        int array_two[workload];
        int array_of_sum[workload];


        MPI_Recv(array_one, workload, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(array_two, workload, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        for (int i = 0; i < workload; i++) {
            array_of_sum[i] = array_one[i] + array_two[i];
        }


        MPI_Send(&array_of_sum, workload, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}


