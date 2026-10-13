//Librería que contiene las funciones scanf y printf
#include <stdio.h> 
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

//Definimos las constantes del programa
#define NFA  1000
#define NCA  1000
#define NFB  1000
#define NCB  1000
#define NFC  1000
#define NCC  1000

double wallclock(void);
void producto_matrices(double *m1, double *m2, double *m3);
void matriz_matriz_ld (double *a,int fa,int ca,int lda, double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc);
void matriz_matriz_bloques (double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc,int tb);
void copiar_matriz(double *s, int nf, int nc, int tb, double *m, int f, int c, int ld);
void matriz_cero(double *s, int nf, int nc, int tb);
void imprimir(double *m, int f, int c);
void multiplicar_acumular_matrices(double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc);
void multiplicar_acumular_bloques(double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc, int tbp);
void matriz_matriz_bloquesdobles (double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc,int tb, int tbp);

//Función principal del programa
int main (int argc, char *argv[]) 

{
	
	// Declaro las variables de mi función
	int i,j,t;
	
	double *mA, *mB,*mC;
	double time_cpu, t0, t1;
	
	mA=(double*)malloc(sizeof(double)*NFA*NCA);
	mB=(double*)malloc(sizeof(double)*NFB*NCB);
	mC=(double*)malloc(sizeof(double)*NFC*NCC);
	
	srand(time(NULL));

	//En C las matrices de n filas tienen filas de la 0 a la n-1
	for (i=0;i<NFA;i++)
	{
		//En C las matrices de n columnas tienen cols. de la 0 a la n-1
		for (j=0;j<NCA;j++)
		{
			//Inicializo las matrices
			*(mA+i*NCA+j)=rand() % 100;
		}
	}
	
	//En C las matrices de n filas tienen filas de la 0 a la n-1
	for (i=0;i<NFB;i++)
	{
		//En C las matrices de n columnas tienen cols. de la 0 a la n-1
		for (j=0;j<NCB;j++)
		{
			//Inicializo las matrices
			*(mB+i*NCB+j)=rand() % 100;
		}
	}
	
	t0 = wallclock();
	producto_matrices(mA,mB,mC);
	t1 = wallclock();
	time_cpu = (t1-t0);
	
	//Imprimo la matriz
	printf("\n");
	
//	imprimir(mC,NFC,NCC);	
	
	printf("\n Se ha tardado %f ms usando el algoritmo tradicional\n",(double)time_cpu * 1e3);
	
	t0 = wallclock();
	matriz_matriz_ld (mA,NFA,NCA,NCA, mB,NFB,NCB,NCB,mC,NFC,NCC,NCC);
	t1 = wallclock();
	time_cpu = (t1-t0);
	
	//Imprimo la matriz
	printf("\n");
	
//	imprimir(mC,NFC,NCC);
	
	printf("\n Se ha tardado %f ms usando el algoritmo tradicional optimizado\n",(double)time_cpu * 1e3);
	
	t0 = wallclock();
	matriz_matriz_bloques (mA,NFA,NCA,NCA,mB,NFB,NCB,NCB,mC,NFC,NCC,NCC,500); //tamaño bloque = 50 
	t1 = wallclock();
	time_cpu = (t1-t0);
	
	//Imprimo la matriz
	printf("\n");
	
//	imprimir(mC,NFC,NCC);
	
	printf("\n Se ha tardado %f ms usando el algoritmo de multiplicación por bloques\n",(double)time_cpu * 1e3);
	
	
	t0 = wallclock();
	matriz_matriz_bloquesdobles (mA,NFA,NCA,NCA,mB,NFB,NCB,NCB,mC,NFC,NCC,NCC,500,250); //tamaño bloque = 50 Se divide a su vez en bloques tamaño = 25
	t1 = wallclock();
	time_cpu = (t1-t0);
	
	//Imprimo la matriz
	printf("\n");
	
//	imprimir(mC,NFC,NFC);
	
	printf("\n Se ha tardado %f ms usando el algoritmo de multiplicación por bloques dobles\n",(double)time_cpu * 1e3);
	
	return 0;
	
}

double wallclock(void)
{
	struct timeval tv;
	double t;
	
	gettimeofday(&tv, NULL);
	
	t = (double)tv.tv_sec;
	t += ((double)tv.tv_usec)/1000000.0;
	
	return t;
}


//algoritmo tradicional
void producto_matrices(double *m1, double *m2, double *m3)
{
	int i,j,t;
	for (i=0;i<NFC;i++)
	{
		//En C las matrices de n columnas tienen cols. de la 0 a la n-1
		for (j=0;j<NCC;j++)
		{
			//Inicializo el elemento de la matriz producto
			*(m3+i*NCC+j)=0;
			for (t=0;t<NCA;t++)
			{
				*(m3+i*NCC+j)=*(m3+i*NCC+j)+*(m1+i*NCA+t)* (*(m2+t*NCB+j));
			}
		}
	}
}

//algoritmo tradicional. Uso de variable auxiliar para calcular el elemento c(i,j)
void matriz_matriz_ld (double *a,int fa,int ca,int lda, double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc) 
{ 
	int i,j,k;
	double s; 
	for(i=0;i<fa;i++)
		for(j=0;j<cb;j++) 
		{
			s=0.; 
			for(k=0;k<ca;k++)
				s+=a[i*lda+k]*b[k*ldb+j]; 
			c[i*ldc+j]=s;
		}
}

//algoritmo por bloques. Multiplicación por bloques de tamaño tb
void matriz_matriz_bloques (double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc,int tb)
{
	int i,j,k; 
	double *s;
	s=(double *) malloc(sizeof(double)* tb * tb);
	for(i=0;i<fa;i=i+ tb) 
		for(j=0;j<cb;j=j+ tb)
		{
			matriz_cero(s, tb, tb, tb); 
			for(k=0;k<ca;k=k+ tb)
				multiplicar_acumular_matrices(&a[i*lda+k], tb, tb,lda,&b[k*ldb+j],tb, tb,ldb,s, tb, tb, tb);
			copiar_matriz(s,tb, tb, tb,&c[i*ldc+j], tb, tb,ldc);
		}
	free(s);
}

//algoritmo por bloques. Multiplicación por bloques de tamaño tb y a su vez, se vuelve a dividir en bloques de tamaño tbp
void matriz_matriz_bloquesdobles (double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc,int tb, int tbp)
{
	int i,j,k; 
	double *s;
	s=(double *) malloc(sizeof(double)* tb * tb);
	for(i=0;i<fa;i=i+ tb) 
		for(j=0;j<cb;j=j+ tb)
		{
			matriz_cero(s, tb, tb, tb); 
			for(k=0;k<ca;k=k+ tb)
				multiplicar_acumular_bloques(&a[i*lda+k], tb, tb,lda,&b[k*ldb+j],tb, tb,ldb,s, tb, tb, tb, tbp);
			copiar_matriz(s,tb, tb, tb,&c[i*ldc+j], tb, tb,ldc);
		}
	free(s);
}

//inicializa un bloque a 0
void matriz_cero(double *s, int nf, int nc, int tb)
{
	int i,j;
	for (i=0;i<nf;i++)
		for(j=0;j<nc;j++)
			*(s+i*tb+j)=0;
	
}

//copia un bloque a su correspondiente posición en la matriz resultado
void copiar_matriz(double *s, int nf, int nc, int tb, double *m, int f, int c, int ld)
{
	int i,j;
	for (i=0;i<nf;i++)
		for(j=0;j<nc;j++)
			*(s+i*tb+j)=m[i*ld+j];
	
}

//multiplica los bloques 
void multiplicar_acumular_matrices(double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc)
{
	
	int i,j,k,kb;
	
	double *da,*db,s;
	
	for(i=0;i<fa;i++){ 
			da=&a[i*lda];	
			for(j=0;j<cb;j++){
				db=&b[j];
				s=c[i*ldc+j];
				for(k=0,kb=0;k<ca;k++,kb=kb+ldb){
					s=s+da[k]*db[kb];
				}
				c[i*ldc+j]=s;
			}
	}
}


//multiplica los bloques dobles
void multiplicar_acumular_bloques(double *a,int fa,int ca,int lda,double *b,int fb,int cb,int ldb,double *c,int fc,int cc,int ldc, int tbp)
{
	
	int i,j,k;
	double *s;
	s=(double *) malloc(sizeof(double)*tbp*tbp);
	for(i=0;i<fa;i=i+tbp)
	{
		for(j=0;j<cb;j=j+tbp)
		{ 
			copiar_matriz(&c[i*ldc+j],tbp,tbp,ldc,s,tbp,tbp,tbp);
			for(k=0;k<ca;k=k+tbp)
			{
				multiplicar_acumular_matrices(&a[i*lda+k],tbp,tbp,lda,&b[k*ldb+j],tbp,tbp,ldb,s,tbp,tbp,tbp);
		    }					
			copiar_matriz(s,tbp,tbp,tbp,&c[i*ldc+j],tbp,tbp,ldc); 
		}
	}
	free(s);
}


//imprime la matriz
void imprimir(double *m, int f, int c)
{
	int i,j;
	for (i=0;i<f;i++)
	{
		for(j=0;j<c;j++)
			printf("%f\t", *(m+i*c+j));
		printf("\n");
	}
}