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

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	FILE *input = fopen(argv[1], "r");
	FILE *output = fopen(argv[2], "w");
	if(input == NULL || output == NULL)
	{
		fprintf(stderr, "FindPath error: in main() unable to open file\n");
		exit(EXIT_FAILURE);
	}
	int n; //number of verticies
	fscanf(input, "%d", &n);
	Graph G = newGraph(n);
	int u, v;
	while (fscanf(input, "%d %d", &u, &v) == 2 && (u != 0 || v != 0))
		addEdge(G, u, v);
	printGraph(output, G);
	while (fscanf(input, "%d %d", &u, &v) == 2 && (u != 0 || v != 0))
	{
		BFS(G, u);
		List path = newList();
		getPath(path, G, v);

		if (getDist(G,v) != INF)
		{
			fprintf(output, "\nThe distance from %d to %d is %d\n", u, v, getDist(G,v));
			fprintf(output, "A shortest %d-%d path is: ", u, v);
			printList(output, path);
			fprintf(output, "\n");
		}
		else
		{
			fprintf(output, "\nThe distance from %d to %d is infinity\n", u, v);
            fprintf(output, "No %d-%d path exists\n", u, v);
		}
		freeList(&path);
	}
	freeGraph(&G);
	fclose(input);
	fclose(output);

	return 0;
}