#include <stdio.h>
#include <omp.h>

void compute_iteration(int i, int j) {
    //printf("Hilo %d ejecutando i=%d, j=%d\n", omp_get_thread_num(), i, j);
}

void parallel_schedule() {
    int iter = 16;
    int i, j;

    #pragma omp parallel for private(j) schedule(static) num_threads(4)
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
// Hilo ID 1
// Maneja iteraciones i=0 a i=3
// Si j = 16 - (i+1) entonces j comienza en 15, 14, 13, 12
// Si tarda 2 segundos por iteraciones de j
// i=0 -> j=15 (1 iteración) -> 2 segundos
// i=1 -> j=14 a j=16 (2 iteraciones) -> 4 segundos
// i=2 -> j=13 a j=16 (3 iteraciones) -> 6 segundos
// i=3 -> j=12 a j=16 (4 iteraciones) -> 8 segundos

// Hilo ID 2
// Maneja iteraciones i=4 a i=7
// Si j = 16 - (i+1) entonces j comienza en 11, 10, 9, 8
// Si tarda 2 segundos por iteraciones de j
// i=4 -> j=11 a j=16 (5 iteraciones) -> 10 segundos
// i=5 -> j=10 a j=16 (6 iteraciones) -> 12 segundos
// i=6 -> j=9 a j=16 (7 iteraciones) -> 14 segundos
// i=7 -> j=8 a j=16 (8 iteraciones) -> 16 segundos

// Hilo ID 3
// Maneja iteraciones i=8 a i=11
// Si j = 16 - (i+1) entonces j comienza en 7, 6, 5, 4
// Si tarda 2 segundos por iteraciones de j
// i=8 -> j=7 a j=16 (9 iteraciones) -> 18 segundos
// i=9 -> j=6 a j=16 (10 iteraciones) -> 20 segundos
// i=10 -> j=5 a j=16 (11 iteraciones) -> 22 segundos
// i=11 -> j=4 a j=16 (12 iteraciones) -> 24 segundos

// Hilo ID 4
// Maneja iteraciones i=12 a i=15
// Si j = 16 - (i+1) entonces j comienza en 3, 2, 1, 0
// Si tarda 2 segundos por iteraciones de j
// i=12 -> j=3 a j=16 (13 iteraciones) -> 26 segundos
// i=13 -> j=2 a j=16 (14 iteraciones) -> 28 segundos
// i=14 -> j=1 a j=16 (15 iteraciones) -> 30 segundos
// i=15 -> j=0 a j=16 (16 iteraciones) -> 32 segundos