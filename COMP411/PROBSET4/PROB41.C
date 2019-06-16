/********************************************************************
 *
 * PROGRAM:      Problem Set 4
 * MODULE:       Problem 4-1
 *
 * AUTHOR:       Nathan Fiedler, 30 Oct. 1995
 *
 *******************************************************************/

   /* Basic algorithm */
1. Perform a sort of binary search on array A. Each time we look at
   the middle element of the subarray in A, find where that element
   would be in B, if it were there.
2. Add this position to the middle position of the subarray in A.
   If this sum is less than n, then examine the second half of A.
   If this sum is greater than n, then examine the first half of A.
3. Repeat this procedure until sum equals n, or Ai equals Aj.
4. If Ai equals Aj and sum is not equal to n, then perform this
   entire procedure on array B.

   /* Full Psuedo-code. */
Ai = 1
Aj = n
sum = 0
if ( Ai = Aj ) then
  if ( A[1] < B[1] ) then
    return( A[1] )
  else
    return( B[1] )
else
  while ( sum != n and Ai != Aj ) do
    mid = ( Ai + Aj ) / 2
    position = ModifiedBinarySearch( B, n, A[mid] )
    sum = mid + position
    if ( sum < n ) then
      Ai = mid + 1
    if ( sum > n ) then
      Aj = mid - 1
  endwhile
  if sum != n then
    Bi = 1
    Bj = n
    while ( sum != n and Bi != Bj ) do
      mid = ( Bi + Bj ) / 2
      position = ModifiedBinarySearch( A, n, B[mid] )
      sum = mid + position
      if ( sum < n ) then
        Bi = mid + 1
      if ( sum > n ) then
        Bj = mid - 1
    endwhile
  endif
  return( A[mid] )
endif

/********************************************************************
 *                 ModifiedBinarySearch
 ********************************************************************
 * SYNOPSIS:     Search for where an element would be if it were
 *               in the array.
 * PARAMETERS:   ( array, size of array, element to look for )
 * RETURNS:      Position in array where element would have been.
 * STRATEGY:     Use a basic binary search but perform the end case
 *               differently: When the low and high indices cross,
 *               return the low index minus one.
 *******************************************************************/
ModifiedBinarySearch( array, size, element ) {
  array[ 0 ] = element
  low = 1
  high = size

  do {
    mid = ( low + high ) / 2
    if ( low > high ) then
      return( low - 1 )
    else
      if ( array[ mid ] < element ) then
        low = mid + 1
      else
        high = mid - 1
  while( array[ mid ] != element )

  return( mid )
}

