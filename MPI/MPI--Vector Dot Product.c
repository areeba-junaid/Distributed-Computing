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
        int array_size = 20;
        int array_one[array_size];
        int array_two[array_size];
        for (int i = 0; i < array_size; i++) {
            array_one[i] = 1;
            array_two[i] = 1;
        }

        int workload = array_size / total_processes;


        int total_sum = 0;


        // Send workload, array_one, and array_two to each worker process
        for (int i = 1; i < total_processes; i++) {
            MPI_Send(&workload, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&array_one[i * workload], workload, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&array_two[i * workload], workload, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Calculate the partial sum for the master process
        for (int i = 0; i < workload; i++) {
            total_sum = total_sum + (array_one[i] * array_two[i]);
        }


        // Receive the partial sums from worker processes and accumulate the total sum
        for (int i = 1; i < total_processes; i++) {
            int sum_from_worker;
            MPI_Recv(&sum_from_worker, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += sum_from_worker;
        }


        printf("The final total sum is: %d\n", total_sum);
    }
    else {
        int workload;
        MPI_Recv(&workload, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        int array_one[workload];
        int array_two[workload];


        // Receive the workload, array_one, and array_two from the master process
        MPI_Recv(array_one, workload, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(array_two, workload, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        int array_sum = 0;
        // Calculate the partial sum for each worker process
        for (int i = 0; i < workload; i++) {
            array_sum = array_sum + (array_one[i] * array_two[i]);
        }


        // Send the partial sum back to the master process
        MPI_Send(&array_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}
