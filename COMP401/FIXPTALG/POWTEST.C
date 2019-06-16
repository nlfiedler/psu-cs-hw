#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int main( void ) {
  double i,
         j;
     int x,
         y;
   float m,
         n;

  i = 3;
  j = 2;
  printf( "%lf to the %lf = %lf\n", i, j, pow( i, j ) );
  printf( "The error number is %d\n", errno );
  x = 4;
  y = 5;
  printf( "%d to the %d = %lf\n", x, y, pow( x, y ) );
  printf( "The error number is %d\n", errno );
  m = 2;
  n = 10;
  printf( "%f to the %f = %lf\n", m, n, pow( m, n ) );
  printf( "The error number is %d\n", errno );
  return( 0 );
} /* main */