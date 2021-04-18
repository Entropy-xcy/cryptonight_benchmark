#include <cryptonite_hash.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

 void *worker_thread(void *vargp)
 {
     int *num_hashes = (int *)vargp;
     printf("Thread Got %d Hashes to run\n", *num_hashes);

     char output_cpy[32];
     char output[32];
     for(int i = 0; i < *num_hashes; i ++)
     {
         memcpy(output_cpy, output, 32);
         cryptonight_hash(output, (void*) output_cpy, 1);
     }
}


int main(int argc, char **argv) {
     if(argc != 3)
     {
         printf("Required 2 Input Arguments: <num_threads> <num_hashes>\n");
         return 1;
     }

     int num_threads = atoi(argv[1]);
     int num_hashes = atoi(argv[2]);

     printf("Benchmarking %d CryptoNight Hashes on %d Threads...\n", num_hashes, num_threads);

     int num_hashes_per_thread = num_hashes / num_threads;

     // Start Parallel Execution
     clock_t t;
     t = clock();

     pthread_t thread_ids[num_threads];
     for(int i = 0; i < num_threads; i++)
     {
         pthread_create(&thread_ids[i], NULL, worker_thread, (void *)&num_hashes_per_thread);
     }

     for(int i = 0; i < num_threads; i++)
     {
         pthread_join(thread_ids[i], NULL);
     }
     t = clock() - t;
     double time_taken = ((double)t)/CLOCKS_PER_SEC;
     printf("Takes %lf Seconds.\n", time_taken);
 }
