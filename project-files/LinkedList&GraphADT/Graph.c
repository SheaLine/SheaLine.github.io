// ----------------------------------------------------------------------------
// Shea Line
// sline
// pa1
// ----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "Graph.h"

// structs --------------------------------------------------------------------


typedef struct GraphObj
{
	List *neighbors;
	int *color;
	int *parent;
	int *dist;
	int order;
	int size;
	int source;
}GraphObj;

// Constructors-Destructors ---------------------------------------------------

// returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges
Graph newGraph(int n)
{
	Graph G = malloc(sizeof(GraphObj));
	G->neighbors = calloc(n + 1, sizeof(List));
	G->color = calloc(n + 1, sizeof(int));
	G->parent = calloc(n+1, sizeof(int));
	G->dist = calloc(n+1, sizeof(int));
	G->order = n;
	G->size = 0;
	G->source = NIL;
	for(int i = 1; i <= n; i++)
	{
		G->neighbors[i] = newList();
		G->color[i] = 0;
		G->parent[i] = NIL;
		G->dist[i] = INF;
	}
	return G;
}

// frees all heap memory associated with the Graph *pG, then sets the handle *pG to NULL
void freeGraph(Graph* pG)
{
	if (pG != NULL && *pG != NULL)
	{
		for (int i = 1; i<=(*pG)->order; i++)
		{
			freeList(&((*pG)->neighbors[i]));
		}
		free((*pG)->neighbors);
		free((*pG)->color);
		free((*pG)->parent);
		free((*pG)->dist);
		free(*pG);
		*pG = NULL;
	}
}

// Access functions -----------------------------------------------------------------------------------------------------------------

// returns the order field
int getOrder(Graph G)
{
	return G->order;
}

// returns the size field
int getSize(Graph G)
{
	return G->size;
}

// returns the source vertex most recently used in function BFS(), or NIL if BFS() has not yet been called
int getSource(Graph G)
{
	return G->source;
}

// will return the parent of vertex u in the BFS tree created by BFS(), or NIL if BFS() has not yet been called.
// Pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
int getParent(Graph G, int u)
{
	if (u < 1 || u > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling getParent() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	return G->parent[u];
}

// returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called
// Pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
int getDist(Graph G, int u)
{
	if (u < 1 || u > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling getDist() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	return G->dist[u];
}

// appends to the List L the vertices of a shortest path in G from source to u, or appends to L the value NIL if no such path exists
// Pre: getSource(G)!=NIL
// Pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
void getPath(List L, Graph G, int u)
{
	if (u < 1 || u > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling getPath() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	if (getSource(G) == NIL)
	{
		fprintf(stderr, "%s\n", "Graph Error: calling getPath() with an undefined source vertex");
		exit(EXIT_FAILURE);
	}
	if (u == getSource(G))
		append(L,u);
	else if (G->parent[u] == NIL)
		append(L,NIL);
	else
	{
		getPath(L, G, getParent(G, u));
		append(L,u);
	}

}

// Manipulation procedures -----------------------------------------------------------------------------------------------------------

// deletes all edges of G, restoring it to its original (no edge) state
void makeNull(Graph G)
{
	for (int i = 1; i <= G->order; i++)
	{
		clear(G->neighbors[i]);
	}
	G->size = 0;
}

// inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, and v to the adjacency List of u.
// Your program is required to maintain these lists in sorted order by increasing labels
// Pre: u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
void addEdge(Graph G, int u, int v)
{
	if (u < 1 && u > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling addEdge() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	if (v < 1 && v > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling addEdge() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	addArc(G, u, v);
	addArc(G,v,u);
	G->size--;
}

// inserts a new directed edge from u to v, i.e. v is added to the adjacency List of u (but not u to the adjacency List of v).
// Pre: u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
void addArc(Graph G, int u, int v)
{
	if (u < 1 && u > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling addArc() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	if (v < 1 && v > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling addArc() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	List L = G->neighbors[u];
	for (moveFront(L); index(L) >= 0; moveNext(L))
	{
		if (get(L) >= v)
			break;
	}
	if (index(L) == -1)
		append(L,v);
	else
		insertBefore(L,v);
	G->size++;
}

//  runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly
void BFS(Graph G, int s)
{
	if (s < 1 && s > getOrder(G))
	{
		fprintf(stderr, "%s\n", "Graph Error: calling BFS() with an invalid vertex");
		exit(EXIT_FAILURE);
	}
	if (G == NULL)
	{
		fprintf(stderr, "%s\n", "Graph Error: calling BFS() with a NULL Graph");
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i <=getOrder(G); i++)
	{
		G->color[i] = 0;
		G->dist[i] = INF;
		G->parent[i] = NIL;
	}
	G->source = s;
	G->color[s] = 1;
	G->dist[s] = 0;

	List queue = newList();
	append(queue, s);
	while(length(queue) > 0)
	{
		int current = front(queue);
		deleteFront(queue);
		List adjacencyList = G->neighbors[current];
	for (moveFront(adjacencyList); index(adjacencyList) >= 0; moveNext(adjacencyList))
	{
		int neighbor = get(adjacencyList);
		if (G->color[neighbor] == 0)
		{
			G->color[neighbor] = 1;
			G->dist[neighbor] = G->dist[current] + 1;
			G->parent[neighbor] = current;
			append(queue, neighbor);
		}
	}
	G->color[current] = 2;
	}
	
	freeList(&queue);
}

// Other operations ------------------------------------------------------------------------------------------------------------------

// prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G)
{
	if (G == NULL)
	{
		fprintf(stderr, "%s\n", "Graph Error: calling printGraph() with a NULL Graph");
		exit(EXIT_FAILURE);
	}
	for (int i =1; i <= G->order; i++)
	{
		List L = G->neighbors[i];
		fprintf(out, "%d: ", i);
		for (moveFront(L); index(L) >= 0; moveNext(L))
		{
			fprintf(out, "%d ", get(L));
		}
		fprintf(out, "\n");
	}
}