//-----------------------------------------------------------------------------------------+
// CuentaPar.c: Cuenta apariciones de un número en un arreglo muy |
// grande. Version paralela simple |
// ESQUELETO |
//------------------------------------------------------------------------------------------+
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define MAX_ENTERO 1000
#define NUM_VECTORES 10000 // Simula vector todavía mayor
#define NUM_BUSCADO 8
//--------------------------------------------------------------------
void esclavo(void) {
	  int i, j, cardinalidad, miTrozoSize, cuentaParcial = 0;
    int *miVector;
    int numProcs;

    // Recibir la cardinalidad total
    MPI_Bcast(&cardinalidad, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular tamaño de mi parte (asumiendo división exacta)
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    miTrozoSize = cardinalidad / numProcs;

    miVector = (int *)malloc(sizeof(int) * miTrozoSize);
    
    // Recibir mi parte del vector
    MPI_Scatter(NULL, 0, MPI_INT, miVector, miTrozoSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Realizar la búsqueda (10,000 veces según el enunciado)
    for (j = 0; j < NUM_VECTORES; j++) {
        for (i = 0; i < miTrozoSize; i++) {
            if (miVector[i] == NUM_BUSCADO) cuentaParcial++;
        }
    }

    // Enviar resultado al maestro para la suma total
    MPI_Reduce(&cuentaParcial, NULL, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    free(miVector);
}
//-------------------------------------------------------------------
//--------------------------------------------------------------------

void maestro (int NumProcesos, int Cardinalidad) {
	 int i, j, totalNumVeces = 0, cuentaParcial = 0;
	 int *vector, *miTrozo;
	 int miTrozoSize = Cardinalidad / NumProcesos;
	 struct timeval t0, tf, t;
	
	 // Inicializar el vector
	 assert((vector =(int *)malloc(sizeof(int)*Cardinalidad))!=NULL);
	 for (i=0; i<Cardinalidad; i++)
		 vector[i] = random() % MAX_ENTERO;
		 
	 assert (gettimeofday (&t0, NULL) == 0);
	 
	 // Repartir trabajo
	 MPI_Bcast(&Cardinalidad, 1, MPI_INT, 0, MPI_COMM_WORLD);
	 miTrozo = (int *)malloc(sizeof(int) * miTrozoSize);
	 MPI_Scatter(vector, miTrozoSize, MPI_INT, miTrozo, miTrozoSize, MPI_INT, 0, MPI_COMM_WORLD);
	 
	 // Computar mi trozo
	 for (j = 0; j < NUM_VECTORES; j++) {
        for (i = 0; i < miTrozoSize; i++) {
            if (miTrozo[i] == NUM_BUSCADO) cuentaParcial++;
        }
   }
	 
	 // Recoger resultados
	 MPI_Reduce(&cuentaParcial, &totalNumVeces, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	 
	 assert (gettimeofday (&tf, NULL) == 0);
	 timersub(&tf, &t0, &t);
	 
	 printf("Numero de veces que aparece el %d = %d\n", NUM_BUSCADO, totalNumVeces);
    printf("tiempo total = %ld:%03ld\n", t.tv_sec, t.tv_usec / 1000);
    
    free(vector);
    free(miTrozo);
}

//--------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	 int yo, numProcesos, laCardinalidad;
	 
	 if (argc != 2) {
		 printf ("Uso: cuentaPar cardinalidad \n");
		 return 0;
	 }
	 
	 laCardinalidad = atoi(argv[1]);
	 assert (laCardinalidad > 0);
	 setbuf (stdout, NULL);
	 
	 MPI_Init (&argc, &argv);
	 MPI_Comm_rank (MPI_COMM_WORLD, &yo);
	 MPI_Comm_size (MPI_COMM_WORLD, &numProcesos);
	 
	 if (yo == 0) maestro(numProcesos,laCardinalidad);
	 else esclavo();
	 
	 MPI_Finalize();
	 return 0;
 }