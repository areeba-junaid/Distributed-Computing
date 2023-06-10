#include<iostream>
#include<pthread.h>
#include <cstdlib>
#include <sys/time.h>
using namespace std;
int *VectorA, *VectorB,*VectorC;

typedef struct{
int start_index;
int end_index;
} thread_data;

void displaying_output( int elements,int *Array)
    {
        for(int i=0;i< elements; i++){
            cout<<Array[i]<<" " ;
        }
        cout<<"\n";
    }
void * vector_add(void * args) {
    thread_data *t_args = (thread_data *) args;
    int start_index = t_args->start_index;
    int end_index = t_args->end_index;

    for (int i = start_index; i < end_index; i++) {
        VectorC[i] = VectorA[i] + VectorB[i];
    }

    pthread_exit(NULL);
}


int main()
{
    int vector_size, no_of_threads;
    cout << "Enter the size of Vector: " ; //Taking size of Array
	cin >>vector_size;
	VectorA = new int(vector_size); //Creating Dynamic Array
    VectorB = new int(vector_size);
    VectorC = new int(vector_size);
	cout<<"Enter the number of Threads: ";  //Number of Threads
	cin>>no_of_threads;
    srand(3);

    //rand( )%n returns a random number 0 to n-1 (both inclusive).
    // By adding 1 we are assigning numbers from 1 till 100.
    for (int i = 0; i < vector_size ; i++) {
        VectorA[i] = (rand() % 100) + 1;
        VectorB[i] = (rand() % 100) + 1;
        }
      displaying_output(vector_size,VectorA);
      displaying_output(vector_size,VectorB);
     int extra_workload = vector_size % no_of_threads; // Sometimes the work load does not get equally divided so getting the number of extra workload

     int workload = (vector_size - extra_workload) / no_of_threads; // Removing the extra workload, it will be given to the last vector

     //Assigning start and end index to threads
    thread_data *arg_list[no_of_threads];
    //Assigning Start and end time (preparing Arguments)

    for (int i = 0; i < no_of_threads; i++)
    {

        arg_list[i] = new  thread_data; //builds on pointer structure on heap

        arg_list[i]->start_index = i * workload;

        if (i == no_of_threads - 1) {
            arg_list[i]->end_index = ((i + 1) * workload) + extra_workload;    // If it is the last thread then giving it the extra workload
        }
        else {
        arg_list[i]->end_index = ((i + 1) * workload);
        }
    }
    pthread_t threads[no_of_threads];

    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);

    for (int i = 0; i < no_of_threads; i++) {
        pthread_create(&threads[i], NULL, &vector_add, (void *) arg_list[i]);
    }

    for (int i = 0; i < no_of_threads; i++) {
        pthread_join(threads[i], NULL);
        free(arg_list[i]);
    }

    gettimeofday(&end_time, NULL);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                                    (end_time.tv_usec - start_time.tv_usec) / 1e6;

    displaying_output(vector_size,VectorC);
    printf("Time taken: %f seconds \n", elapsed_time);
    printf("\n\n");
    return 0;
}
