/********************************************************************
 *
 * PROGRAM:      COMP 411 - Algorithms Analysis
 * MODULE:       Problem 5-2
 *
 * AUTHOR:       Nathan Fiedler; Nov 20, 1995
 *
 * DESCRIPTION:  This algorithm will determine if an undirected
 *               graph is 2 colorable. In other words, no two
 *               adjacent vertices have the same color, and the
 *               vertices can only be one of two colors.
 *
 *******************************************************************/

   /* For all nodes: */
for each v in V do
  visited[v] <- FALSE  /* Not visited. */
  color[v] <- -1       /* No color. */
end { for }

   /* Mark first node, and enqueue it. */
color[0] <- 0
Enqueue( Q, 0 )

for each v in V do
  if visited[v] = FALSE then
    Search( v )
end { for }
return( "YES" )

Search( v )
  Enqueue( v )
     /* For each node in the queue: */
  while Q not empty do
    v <- Dequeue( Q )
    visited[v] <- TRUE
    for w in L(v) do
      if visited[w] = FALSE then
        Enqueue( Q, w )
      if color[w] = color[v] then
        /* Problem - not 2 colorable. */
        Free( Q )
        return( "NO" )
      else
        if color[w] = -1 then
          color[w] <- 1 XOR color[v]
    end { for }
  end { while }
end { Search }

