#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <chrono>

float f(float);

int main(void){ 
  int nsteps = 65000,i,samples,is;
  double h = 1.0/((double)(nsteps)), x,x2,y,y2, quarterpi = 0.0,slope,hs,xs,ys;
  auto start = std::chrono::steady_clock::now();
  #pragma omp parallel for reduction(+:quarterpi) private(i,x,x2,y,y2,slope,samples,is,hs,xs,ys) schedule(guided)
    for (i = 0; i < nsteps; i++)
    {
      x = (double)(i)*h;
      x2 = (double)(i+1)*h;
      y = f(x);
      y2 = f(x2);
      slope = (y - y2)/h;
      if (slope > 15)
      {
        slope = 15;
      }
      samples = 1 + (int)slope;
      for (is = 0; is < samples; is++)
      {
        hs = h/samples;
        xs = x +(double)(is)*hs;
        ys = f(xs);
        quarterpi += hs*ys;
      }
    }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration_seconds = end - start;
  printf("pi/4 = %f \nTerminado en: %lf seg\n",quarterpi,duration_seconds.count());
  return 0;
}

float f(float x){
  return sqrt(1.0-x*x);
}

