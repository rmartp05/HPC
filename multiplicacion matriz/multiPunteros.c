#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#define F 800
#define C 800

double wallclock(void)
{
	struct timeval tv;
	double t;
	
	gettimeofday(&tv, NULL);
	
	t = (double)tv.tv_sec;
	t += ((double)tv.tv_usec)/1000000.0;
	
	return t;
}

// Compara x e y celda a celda. Devuelve 1 si son iguales, 0 si falla.
int comprobar(int (*x)[C], int (*y)[C])
{
    int i, j;
    for (i = 0; i < F; i++){
        for (j = 0; j < C; j++){
            if (x[i][j] - y[i][j] != 0){
                printf("\n ERROR: primera diferencia en [%d][%d] (%d vs %d)",
                       i, j, x[i][j], y[i][j]);
                return 0;   // sale de la funcion, no sigue comprobando
            }
        }
    }
    return 1;
}

int main(){
    int i,j,t,temp;
    double tiempo;

    int (*pa)[C]  = malloc(F * sizeof(*pa));
    int (*pb)[C]  = malloc(F * sizeof(*pb));
    int (*pc)[C]  = malloc(F * sizeof(*pc));
    int (*pc2)[C] = malloc(F * sizeof(*pc2));

    if (pa == NULL || pb == NULL || pc == NULL || pc2 == NULL){
        printf("Error: no se pudo reservar memoria.\n");
        return 1;
    }

    for (i = 0; i < F; i++){
        for (j = 0; j < C; j++){
            pa[i][j] = rand() % 10;
            pb[i][j] = rand() % 10;
        }
    }

    tiempo=wallclock();
    for (i = 0; i < F; i++){
        for(j = 0; j < C; j++){
            temp = 0;
            for(t = 0; t < C; t++){
                temp += pa[i][t] * pb[t][j];
            }
            pc[i][j] = temp;
        }
    }
    printf("\n Tiempo = %f ", wallclock() - tiempo);

    tiempo=wallclock();
    for(i = 0; i < F; i++){
        for(j = 0; j < C; j++){
            pc2[i][j] = 0;                       // ahora se escribe en c2 para comparar
            for(t = 0; t < C; t++){
                pc2[i][j] += pa[i][t] * pb[t][j];
            }
        }
    }
    printf("\n Tiempo = %f ", wallclock() - tiempo);

    // comprobacion
    if (comprobar(pc, pc2))
        printf("\n Comprobacion OK: ambos metodos dan el mismo resultado.\n");
    else
        printf("\n La multiplicacion fallo, ver posicion arriba.\n");

    free(pa);
    free(pb);
    free(pc);
    free(pc2);

    return 0;
}