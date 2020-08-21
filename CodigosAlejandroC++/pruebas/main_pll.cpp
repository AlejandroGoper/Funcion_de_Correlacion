#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

int main(void){ 
  const int N = 10;
  float a[N], b[N], c[N];
  int i;

  /* Initialize arrays a and b */
  for (i = 0; i < N; i++) {
    a[i] = i * 2.0;
    b[i] = i * 3.0;
  }

  printf("Vector a \t Vector b \n");

  for (i = 0; i < N; i++)
  {
    printf("%f \t %f \n",a[i],b[i]);
  }

 /* Compute values of array c = a+b in parallel. */
  #pragma omp parallel shared(a, b, c) private(i)
  { 
    #pragma omp for             
    for (i = 0; i < N; i++){
      c[i] = a[i] + b[i];
    }
  }

  printf("Vector c \n");
  for (i = 0; i < N; i++)
  {
    printf("%f\n",c[i]);
  }
  float s;
  s=0;
  for (i = 0; i < N; i++)
  {
    s+=a[i] + b[i];
  }
  printf("La suma de los elemenos de a y b: %f \n",s);
  s=0;
  // en paralelo
  #pragma omp parallel shared(a,s) private(i)
  {
    float ls = 0; // privada por defecto, pues se declara dentro de la seccion de paralelizar
    #pragma omp for
      for (i = 0; i < N; i++)
      {
        ls = a[i];
        #pragma omp critical  
        s += ls;  
      }
  }
  printf("La suma de los elemenos de a en paralelo: %f \n",s);

// en paralelo
  #pragma omp parallel shared(a,b,c,s) private(i)
  {
    #pragma omp for
      for (i = 0; i < N; i++)
      {
        s = sqrt(b[i]-a[i]);
        c[i] = s;
        //#pragma omp critical  
        //s += ls;  
      }
  }

  printf("Vector c \n");
  for (i = 0; i < N; i++)
  {
    printf("%f\n",c[i]);
  }
  //sin paralelizar
  for (i = 0; i < N; i++)
  {
    s = sqrt(b[i]-a[i]);
    c[i] = s;
        //#pragma omp critical  
        //s += ls;  
  }
  
  printf("Vector c \n");
  for (i = 0; i < N; i++)
  {
    printf("%f\n",c[i]);
  }
  i = 0;
  int j = 0,k = 0;
  printf("\tPrueba de compresion de ciclos dinamicos\n");
    printf("Normal\n");
    int n = 4;
    for(i=0; i<n; i++) {
        for(j=0; j<=i; j++) {
            printf("(%d,%d)\n", i,j);
        }
    }
    printf("\n");
    printf("Comprimido\n");
    for (int x = 0; x < n*(n+1)/2; x++)
    {
      ++j;
      if (j>i)
      {
        j = 0;
        ++i;
      }
      printf("(%d,%d)\n", i,j);
    }
    

}