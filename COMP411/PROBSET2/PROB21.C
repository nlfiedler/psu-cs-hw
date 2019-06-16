/********************************************************************
 *
 * PROJECT:      COMP 411 Assignmentns
 * MODULE:       Problem 2-1
 * AUTHOR:       Nathan Fiedler, Sep 21, 1995
 *
 * DESCRIPTION:
 *      Sort an array of True and Falses to have all the Falses
 *      before all the Trues.
 *
 *******************************************************************/

   /*
    * Assume the first true is in the first slot.
    * Find the first true by scanning past any initial falses.
    */
T = 0
while ( array[T] = FALSE ) and ( T < N ) do
  inc( T )
endwhile

   /*
    * Start looking for falses after the first true.
    * If we find a false,
    *   swap with the first true, and
    *   move the "true" finger to point to the new first true.
    * Move the "false" finger to the next slot to check.
    */
if T < N then
  F = T + 1
  while F < N do
    if array[F] = FALSE then
      swap( array[T], array[F] )
      inc( T )
    endif
    inc( F )
  endwhile
endif

