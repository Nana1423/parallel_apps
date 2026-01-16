#include <stdio.h>
#include <omp.h>

void compute_iteration(int i, int j) {
    //printf("Hilo %d ejecutando i=%d, j=%d\n", omp_get_thread_num(), i, j);
}

void parallel_schedule() {
    int iter = 16;
    int i, j;

    #pragma omp parallel for private(j) schedule(dynamic, 2) num_threads(4)
    for (i = 0; i < iter; ++i) {
        for (j = iter - (i+1); j < iter; ++j) {
            compute_iteration(i, j);
        }
    } 
}

int main() {
    parallel_schedule();
    return 0;
}
