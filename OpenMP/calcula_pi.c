#include <stdio.h>
#include <omp.h>

double calcula_pi(double step) {
    double sum = 0.0;

    #pragma omp parallel 
    {
        double x;
        double local_sum = 0.0; 

        // Repartimos las iteraciones entre los hilos
        #pragma omp for private(x)
        for (int i = 0; i < 1000000; ++i) {
            x = (i - 0.5) * step; 
            local_sum += 4.0 / (1.0 + x * x);
        }

        // Cada hilo suma su total local al global de forma segura
        #pragma omp critical
        {
            sum += local_sum;
        }
    }
    return step * sum;
}

int main () {
    double step = 1.0 / 1000000.0;
    double pi = calcula_pi(step);
    printf("Valor de Pi: %f\n", pi);
    return 0;
}