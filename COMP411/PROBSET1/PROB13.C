/********************************************************************
 *
 * PROJECT:      COMP 411 Assignments
 * MODULE:       Problem 1-3
 *
 * AUTHOR:       Nathan Fiedler, 16 Sep 1995
 *
 * DESCRIPTION:
 *      This is an algorithm to takes k sorted lists and combine
 *      them into one list that is sorted. This is done in O(nlogk)
 *      time.
 *
 * ASSUMPTIONS:
 *      I'm assuming that I have a list data type and a heap data
 *      type that support the operations I'm using in the algorithm.
 *      I'm also assuming we have the number k.
 *
 *******************************************************************/

   /*
    * Create the final list object, in which all the
    * elements will eventually end up. The Heap object
    * starts as a list, and then is built into a heap
    * using the HeapBuildHeap function.
    */
FinalList = ListCreateList()
Heap = ListCreateList()

   /*
    * Build array of pointers to the first element
    * of each list.
    */
for i = 1 to k
  Heap[ i ] = ListPtrToFirst( i )
endfor

   /*
    * Turn the unordered array into a heap, sorting
    * by the keys that the pointers point to.
    */
HeapBuildHeap( Heap )

   /*
    * This is the main loop. It takes the minimum
    * of the heap and places it in the final sorted
    * list. After removing the min element, it looks
    * for the next item in that list and inserts a
    * pointer to that element into the heap. If a
    * list becomes empty, then it is finished and
    * does not get inserted into the heap. This
    * process continues until all the lists become
    * empty, making the heap empty as well.
    */
while not HeapIsEmptyHeap do
  ListAddToList( FinalList, HeapMinValue( Heap ) )
  ptr = ListNextPointer( HeapDeleteMin( Heap ) )
  if not( ptr = null ) then
    HeapInsert( ptr )
  endif
endwhile

   /*
    * Free the heap and list objects.
    */
HeapFreeHeapStructure( Heap )
for i = 1 to k
  ListFreeListStructure( i )
endfor

