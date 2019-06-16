This is some code I wrote. Feedback would be appreciated.

Andrew Hallendorff

/*
  matprof.c (Matrix profiler)
  By Andrew Hallendorff

  This is a program that shows how you can optimize your matrix
  multiplications for a 3d graphics system. For the most part when
  using matricies to represent a 3d graphics rotation the lower
  part of the matrix is always 0,0,0,1. If we exclude this from
  our multiplication we can save 16 mults and 12 adds and 4 assigns.
  Also you can speed up your access of your variables by repersenting
  them as a single array instead of a matrix. Final optimization is
  done by expanding operations.

  You should get results about 3 times faster than the normal methods.

  Your code may look bad but your performance will look good.

  This code does NTESTD4*4 matrix multiplications for each method.
  Reduce or increase or decrease the define NTESTD4 if you have a
  slow or fast computer. Remember maxint on a 16 bit computer is 32767.

  I got this on my 486dx2 80 with watcom 10a under dos4gw
  complied with

  wcl386 /oneatx /zp4 /5r matprof

    Testing 131068 operations each

    mm1: 148 sec Fast sub matrix multiplications
    about 0.0558 mflops
    mm2: 247 sec Fast full matrix multiplications
    about 0.0594 mflops
    mm3: 407 sec Regular full matrix multiplications
    about 0.0361 mflops
    mm1: 1.000000 mm2: 1.668919 mm3: 2.750000

  I would love to hear what other computers got for this test.
  andyrew1@aol.com
  */


#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NTESTD4 32767

float x[16]= {  1, 2, 3, 4,
                5, 6, 7, 8,
                9,10,11,12,
               13,14,15,16 },
      a[16],
      y[4][4]={ { 1, 2, 3, 4},
                { 5, 6, 7, 8},
                { 9,10,11,12},
                {13,14,15,16} },
      b[4][4];

/* Fast submatrix multiplication */
void mm1() {
a[0]= x[0] * x[0] + x[1] * x[4] + x[2] * x[8];
a[1]= x[0] * x[1] +  x[1] * x[5] + x[2] * x[9];
a[2]= x[0] * x[2] +  x[1] * x[6] + x[2] * x[10];
a[3]= x[0] * x[3] +  x[1] * x[7] + x[2] * x[11] + x[3];
a[4]= x[4] * x[0] +  x[5] * x[4] + x[6] * x[8];
a[5]= x[4] * x[1] +  x[5] * x[5] + x[6] * x[9];
a[6]= x[4] * x[2] +  x[5] * x[6]+ x[6] * x[10];
a[7]= x[4] * x[3] +  x[5] * x[7] + x[6] * x[11] + x[7];
a[8]= x[8] * x[0] +  x[9] * x[4] + x[10] * x[8];
a[9]= x[8] * x[1] +  x[9] * x[5] + x[10] * x[9];
a[10]= x[8] * x[2] +  x[9] * x[6] + x[10] * x[10];
a[11]= x[8] * x[3] +  x[9] * x[7] + x[10] * x[11] + x[11];
}

/* Fast full matrix multiplication */
void mm2() {
a[0]= x[0] * x[0]  + x[1] * x[4] + x[2] * x[8] + x[3] * x[12];
a[1]= x[0] * x[1] +  x[1] * x[5] + x[2] * x[9] + x[2] * x[13];
a[2]= x[0] * x[2] +  x[1] * x[6] + x[2] * x[10] + x[3] * x[14];
a[3]= x[0] * x[3] +  x[1] * x[7] + x[2] * x[11] + x[3] * x[15];
a[4]= x[4] * x[0] +  x[5] * x[4] + x[6] * x[8] + x[7] * x[12];
a[5]= x[4] * x[1] +  x[5] * x[5] + x[6] * x[9] + x[7] * x[13];
a[6]= x[4] * x[2] +  x[5] * x[6]+ x[6] * x[10] + x[7] * x[14];
a[7]= x[4] * x[3] +  x[5] * x[7] + x[6] * x[11] + x[7] * x[15];
a[8]= x[8] * x[0] +  x[9] * x[4] + x[10] * x[8] + x[11] * x[12];
a[9]= x[8] * x[1] +  x[9] * x[5] + x[10] * x[9] + x[11] * x[13];
a[10]= x[8] * x[2] +  x[9] * x[6] + x[10] * x[10] + x[11] * x[14];
a[11]= x[8] * x[3] +  x[9] * x[7] + x[10] * x[11] + x[11] * x[15];
a[12]= x[12] * x[0] +  x[13] * x[4] + x[14] * x[8] + x[15] * x[12];
a[13]= x[12] * x[1] +  x[13] * x[5] + x[14] * x[9] + x[15] * x[13];
a[14]= x[12] * x[2] +  x[13] * x[6] + x[14] * x[10] + x[15] * x[14];
a[15]= x[12] * x[3] +  x[13] * x[7] + x[14] * x[11] + x[15] * x[15];
}

/* Regular full matrix multiplication */
void mm3() {
  int i,j,k;
  for(i=0;i<4;i++)
    for(j=0;j<4;j++) {
      b[i][j]=0;
      for(k=0;k<4;k++)
        b[i][j]+= y[i][k] * y[k][j];
  }
}


clock_t test( char *n, void (*f)(), char *d ) {
    clock_t start;
    clock_t stop;
    int k;

    start = clock();
    for( k = 0; k < NTESTD4; ++k ) {
	f();
	f();
	f();
	f();
    }
    stop = clock();
    printf( "%s: %u sec %s\n", n, stop - start, d );
    return stop - start;
}

int main( void ) {
    double t1,t2,t3,m123;
    printf("\nTesting %ld operations each\n\n",(long)NTESTD4*(long)4);
    t1 = (double)test( "mm1", mm1, "Fast sub matrix multiplications" );
    printf( "about %.4f mflops\n", (((double)NTESTD4*4)*63)/(t1*(double)1000000) );
    t2 = (double)test( "mm2", mm2, "Fast full matrix multiplications" );
    printf( "about %.4f mflops\n", (((double)NTESTD4*4)*112)/(t2*(double)1000000) );
    t3 = (double)test( "mm3", mm3, "Regular full matrix multiplications" );
    printf( "about %.4f mflops\n", (((double)NTESTD4*4)*112)/(t3*(double)1000000) );
    m123 = min( t1, min(t2, t3) );
    printf( "mm1: %f mm2: %f mm3: %f\n", t1/m123, t2/m123, t3/m123);
} /* main */
