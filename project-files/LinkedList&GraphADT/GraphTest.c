// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa1
// ----------------------------------------------------------------------------

#include<stdio.h>
#include <stdio.h>
#include "Graph.h"

int main()
{
    // Test case 1: A graph with a single vertex
    printf("=== Testing A graph with a single vertex ===\n");
    Graph G1 = newGraph(1);
    printf("Order: %d\n", getOrder(G1)); // Output: 1
    printf("Size: %d\n", getSize(G1)); // Output: 0
    printGraph(stdout, G1); // Output: 1:
    freeGraph(&G1);

    // Test case 2: A graph with multiple vertices but no edges
    printf("\n=== Testing A graph with multiple vertices but no edges ===\n");
    Graph G2 = newGraph(5);
    printf("Order: %d\n", getOrder(G2)); // Output: 5
    printf("Size: %d\n", getSize(G2)); // Output: 0
    printGraph(stdout, G2); // Output: 1: \n 2: \n 3: \n 4: \n 5:
    freeGraph(&G2);

    // Test case 3: A graph with multiple vertices and edges
    printf("\n=== Testing A graph with multiple vertices and edges ===\n");
    Graph G3 = newGraph(5);
    addEdge(G3, 1, 2);
    addEdge(G3, 2, 3);
    addEdge(G3, 3, 4);
    addEdge(G3, 4, 5);
    printf("Order: %d\n", getOrder(G3)); // Output: 5
    printf("Size: %d\n", getSize(G3)); // Output: 4
    printGraph(stdout, G3); // Output: 1: 2 \n 2: 1 3 \n 3: 2 4 \n 4: 3 5 \n 5: 4
    freeGraph(&G3);

    // Test case 4: A graph with BFS run on it
    printf("\n=== Testing A graph with BFS run on it ===\n");
    Graph G4 = newGraph(5);
    addEdge(G4, 1, 2);
    addEdge(G4, 2, 3);
    addEdge(G4, 3, 4);
    addEdge(G4, 4, 5);
    BFS(G4, 1);
    printf("Source: %d\n", getSource(G4)); // Output: 1
    printf("Distances: ");
    for (int i = 1; i <= getOrder(G4); i++)
    {
        printf("%d ", getDist(G4, i)); // Output: 0 1 2 3 4
    }
    printf("\nParents: ");
    for (int i = 1; i <= getOrder(G4); i++)
    {
        printf("%d ", getParent(G4, i)); // Output: 1 1 2 3 4
    }
    printf("\n");
    freeGraph(&G4);

    // Test case 5: A graph with getPath()
    printf("\n=== Testing A graph with getPath() ===\n");
	Graph G5 = newGraph(5);
	addEdge(G5, 1, 2);
	addEdge(G5, 2, 3);
	addEdge(G5, 3, 4);
	addEdge(G5, 4, 5);
	BFS(G5, 1);

	printf("Source: %d\n", getSource(G5)); // Output: 1
	printf("Distances: ");
	for (int i = 1; i <= getOrder(G5); i++)
	{
    	printf("%d ", getDist(G5, i)); // Output: 0 1 2 3 4
	}
	printf("\nParents: ");
	for (int i = 1; i <= getOrder(G5); i++)
	{
    	printf("%d ", getParent(G5, i)); // Output: 1 1 2 3 4
	}
	printf("\n");

	// Get the path from the source vertex to each vertex
	for (int i = 1; i <= getOrder(G5); i++)
	{
    	List path = newList();
    	getPath(path, G5, i);
    	printf("Path from %d to %d: ", getSource(G5), i);
    	printList(stdout, path);
    	printf("\n");
    	freeList(&path);
	}

	freeGraph(&G5);


    // Test case 6: A graph with a cycle
	printf("\n=== Testing A graph with a cycle ===\n");
	Graph G6 = newGraph(4);
	addEdge(G6, 1, 2);
	addEdge(G6, 2, 3);
	addEdge(G6, 3, 4);
	addEdge(G6, 4, 1);
	printf("Order: %d\n", getOrder(G6)); // Output: 4
	printf("Size: %d\n", getSize(G6)); // Output: 4
	printGraph(stdout, G6); // Output: 1: 2 4 \n 2: 1 3 \n 3: 2 4 \n 4: 1 3
	freeGraph(&G6);

	// Test case 7: A disconnected graph
	printf("\n=== Testing A disconnected graph ===\n");
	Graph G7 = newGraph(6);
	addEdge(G7, 1, 2);
	addEdge(G7, 2, 3);
	addEdge(G7, 4, 5);
	addEdge(G7, 5, 6);
	printf("Order: %d\n", getOrder(G7)); // Output: 6
	printf("Size: %d\n", getSize(G7)); // Output: 4
	printGraph(stdout, G7); // Output: 1: 2 \n 2: 1 3 \n 3: 2 \n 4: 5 \n 5: 4 6 \n 6: 5
	freeGraph(&G7);

	return 0;
}

