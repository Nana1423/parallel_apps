/* Programa e8-serial.c */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static long num_steps=100000000;
double step, pi;
int n_hilos = 6;

void main() {
	 int i; double x, sum = 0.0;
	 step = 1.0/(double) num_steps;
     omp_set_num_threads(n_hilos);
	 
     #pragma omp parallel for reduction(+:sum) private(x,i)
	 for (i=0; i< num_steps; i++) {
		 x = (i+0.5)*step;            // Variable privada x
		 sum = sum + 4.0/(1.0 + x*x); // Variable compartida sum
	 }
	 
	 pi = step * sum;
	 printf("Pi = %f\n",pi);
}
