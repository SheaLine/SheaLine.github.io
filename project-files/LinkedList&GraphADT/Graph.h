// ----------------------------------------------------------------------------
// Shea Line
// sline
// pa1
// ----------------------------------------------------------------------------
#ifndef _GRAPH_H_INCLUDE_
#define _GRAPH_H_INCLUDE_

#include <stdbool.h>
#include <stdio.h>
#include "List.h"

#define INF -1 // macro for infinity
#define NIL 0 // macro for undefined vertex label

// Exported types --------------------------------------------------------------------------------------------------------------------
typedef struct GraphObj* Graph;

// Constructors-Destructors ----------------------------------------------------------------------------------------------------------

// returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges
Graph newGraph(int n);


// frees all heap memory associated with the Graph *pG, then sets the handle *pG to NULL
void freeGraph(Graph* pG);

// Access functions -----------------------------------------------------------------------------------------------------------------

// returns the order field
int getOrder(Graph G);


// returns the size field
int getSize(Graph G);


// returns the source vertex most recently used in function BFS(), or NIL if BFS() has not yet been called
int getSource(Graph G);


// will return the parent of vertex u in the BFS tree created by BFS(), or NIL if BFS() has not yet been called.
// Pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
int getParent(Graph G, int u);


// returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called
// Pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
int getDist(Graph G, int u);


// appends to the List L the vertices of a shortest path in G from source to u, or appends to L the value NIL if no such path exists
// Pre: getSource(G)!=NIL
// Pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
void getPath(List L, Graph G, int u);

// Manipulation procedures -----------------------------------------------------------------------------------------------------------

// deletes all edges of G, restoring it to its original (no edge) state
void makeNull(Graph G);

// inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, and v to the adjacency List of u.
// Your program is required to maintain these lists in sorted order by increasing labels
// Pre: u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
void addEdge(Graph G, int u, int v);

// inserts a new directed edge from u to v, i.e. v is added to the adjacency List of u (but not u to the adjacency List of v).
// Pre: u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
void addArc(Graph G, int u, int v);

//  runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly
void BFS(Graph G, int s);

// Other operations ------------------------------------------------------------------------------------------------------------------

// prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G);

#endif