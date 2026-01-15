#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <float.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int id, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // N es igual al número de procesos
    int N = size;
    float *matrix = NULL;
    
    // El proceso maestro crea y llena la matriz NxN
    if (id == 0) {
        srand(time(NULL));
        matrix = (float *)malloc(sizeof(float) * N * N);
        for (int i = 0; i < N * N; i++) {
            matrix[i] = ((float)rand() / (float)RAND_MAX) * 100.0f;
        }
    }

    // Buffer para recibir la fila (cada fila tiene N elementos)
    float *row_buffer = (float *)malloc(sizeof(float) * N);

    // Reparte una fila a cada proceso usando MPI_Scatter
    MPI_Scatter(matrix, N, MPI_FLOAT, row_buffer, N, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calcula min, max y promedio de la fila local
    float mymin = FLT_MAX;
    float mymax = -FLT_MAX;
    float mysum = 0.0f;

    for (int i = 0; i < N; i++) {
        if (row_buffer[i] < mymin) mymin = row_buffer[i];
        if (row_buffer[i] > mymax) mymax = row_buffer[i];
        mysum += row_buffer[i];
    }
    float myavg = mysum / N;

    // Prepara datos locales para recolectar (3 elementos)
    float local_stats[3] = {mymin, mymax, myavg};
    float *all_stats = NULL;

    if (id == 0) {
        all_stats = (float *)malloc(sizeof(float) * 3 * size);
    }

    // Colecta todos los arreglos de 3 elementos en la raíz
    MPI_Gather(local_stats, 3, MPI_FLOAT, all_stats, 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // El proceso raíz calcula los valores globales
    if (id == 0) {
        float global_min = FLT_MAX;
        float global_max = -FLT_MAX;
        float global_avg_sum = 0.0f;

        // El maestro imprime los resultados de cada proceso
        for (int i = 0; i < size; i++) {
            float r_min = all_stats[i * 3];
            float r_max = all_stats[i * 3 + 1];
            float r_avg = all_stats[i * 3 + 2];

            printf("Process %d with row %d – min: %f; max: %f; avg: %f\n", i, i, r_min, r_max, r_avg);

            // Aprovechamos el ciclo para calcular los globales
            if (r_min < global_min) global_min = r_min;
            if (r_max > global_max) global_max = r_max;
            global_avg_sum += r_avg;
        }

        printf("\n--- Global Results ---\n");
        printf("Global Min: %f\n", global_min);
        printf("Global Max: %f\n", global_max);
        printf("Global Avg of Averages: %f\n", global_avg_sum / size);

        free(matrix);
        free(all_stats);
    }

    free(row_buffer);
    MPI_Finalize();
    return 0;
}