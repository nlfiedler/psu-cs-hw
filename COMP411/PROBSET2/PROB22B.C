/********************************************************************
 *
 * PROJECT:        COMP 411 Assignments
 * MODULE:         Problem 2-2b
 * AUTHOR:         Nathan Fiedler, Sept 25, 1995
 *
 * DESCRIPTION:
 *      This is an algorithm to find the smallest and largest
 *      elements in an array of N elements. It makes ceil(3n/2)-2
 *      comparisons.
 *
 *******************************************************************/

if N = 1 then
  SMALLEST = array[0]
  LARGEST = array[0]
else

     /*
      * First, get an initial smallest and largest from the
      * first two elements in the list.
      * 1 comparison
      */
  if array[0] > array[1] then
    SMALLEST = array[1]
    LARGEST  = array[0]
  else
    SMALLEST = array[0]
    LARGEST  = array[1]
  endif

     /*
      * Now start the main loop at the third element in the array,
      * since we just checked the first two.
      * Set the end of the array to be an even number, since we are
      * going to make an even number of comparisons.
      * The loop checks each successive pair of elements.
      *   If the first is larger than the second of the pair:
      *     If the second is smaller than the smallest,
      *       make it the new smallest.
      *     If the first is larger than the largest,
      *       make it the new largest.
      *   Else, if the second is larger:
      *     If the first is smaller than the smallest,
      *       make it the new smallest.
      *     If the second is larger than the largest,
      *       make it the new largest.
      * 3 comparisons in any case
      *   This is for each pair of numbers (n/2).
      */
  i = 2
  MAX = N / 2
  while i < MAX do
    if array[i] > array[i+1] then
      if array[i+1] < SMALLEST then
        SMALLEST = array[i+1]
      endif
      if array[i] > LARGEST then
        LARGEST = array[i]
      endif
    else
      if array[i] < SMALLEST then
        SMALLEST = array[i]
      endif
      if array[i+1] > LARGEST then
        LARGEST = array[i+1]
      endif
    endif
    i = i + 2
  endwhile

     /*
      * If there was an odd number of elements, then we must
      * check the last element against the smallest and
      * largest so far.
      * 2 comparisons
      */
  if MAX > N then
    if array[N-1] < SMALLEST then
      SMALLEST = array[N-1]
    else
      if array[N-1] > LARGEST then
        LARGEST = array[N-1]
      endif
    endif
  endif

endif

