/**
  Minimum Spanning Tree problems in a undirected graph
  Goal: find the lowest-costied edges that connect the graph, which is the Minimum Spanning Tree consist of
  Prim's algorithm: similar to Dijkstra's algorithm, though the goal is different
    Requirement:
      all distance must be greater or equal to 0
  Data structure (for enhancement of time complexity): implement priority queue with min binary heap (complete binary tree)
  Time complexity: O(mlogn), m: # of edges, n: # of nodes
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"
#include "minimum_spanning_tree.h"

static struct Node* (nodes[7]);
static struct Node_collection* explored_nodes = NULL;

struct Node *create_node(){
	static int idx = 0;
	struct Node* node = malloc(sizeof(struct Node));
	node->id = idx;
	node->d = INT_MAX; // supposed to be infinity
	node->explored = 0;
	node->only_neigh_id = -1;
	node->edges = NULL;
	idx++;
	return node;
}

struct Edge* create_edge(int id, int d) {
	struct Edge* edge = malloc(sizeof(struct Edge));
	edge->node = nodes[id];
	edge->d = d;
	edge->next = NULL;
	return edge;
}
void make_edges(struct Node* node, int out_ids[], int out_d[], int olen) {
	struct Edge *out;
	for (int i = 0; i < olen; i++) {
		if (node->edges == NULL) {
			node->edges = create_edge(out_ids[i], out_d[i]);
			out = node->edges;
		} else {
			out->next = create_edge(out_ids[i], out_d[i]);
			out = out->next;
		}
	}
}
/* #: node, (#): distance of edge

   0__(7)__   ___(8)__2
   |       \ /       |
(5)|        1        |(5)
   | _(9)__/ \__(7)_ |
   |/               \|
   3-------(15)------4
    \               /|
  (6)\   ___(8)____/ |(9)
      \ /            |
       5-------(11)--6

*/
void create_all_edges() {
	int edge0[2]  = {1, 3};          int edged0[2] = {7, 5};
	int edge1[4]  = {0, 2, 3, 4};    int edged1[4] = {7, 8, 9, 7};
	int edge2[2]  = {1, 4};          int edged2[2] = {8, 5};
	int edge3[4]  = {0, 1, 4, 5};    int edged3[4] = {5, 9, 15, 6};
	int edge4[5]  = {1, 2, 3, 5, 6}; int edged4[5] = {7, 5, 15, 8, 9};
	int edge5[3]  = {3, 4, 6};       int edged5[3] = {6, 8, 11};
	int edge6[2]  = {4, 5};          int edged6[2] = {9, 11};

	make_edges(nodes[0], edge0, edged0, 2);
	make_edges(nodes[1], edge1, edged1, 4);
	make_edges(nodes[2], edge2, edged2, 2);
	make_edges(nodes[3], edge3, edged3, 4);
	make_edges(nodes[4], edge4, edged4, 5);
	make_edges(nodes[5], edge5, edged5, 3);
	make_edges(nodes[6], edge6, edged6, 2);

}
void make_graph(heap** h) {
	for (int i = 0; i < 7; i++) {
		nodes[i] = create_node();
	}
	create_all_edges();
	nodes[0]->d = 0; // root
	nodes[0]->only_neigh_id = 0; // root
	for (int i = 0; i < 7; i++) {
		heap_insert(h, nodes[i]);
	}
	min_heapify(*h, 1);
}

void inject_into_collection(struct Node *node) {
	struct Node_collection *collection;
	collection = malloc(sizeof(struct Node_collection));
	collection->node = node;
	collection->next = explored_nodes;
	explored_nodes = collection;
	node->explored = 1;
}
//NOTE
void Prim_algorithm(heap* h) {
	struct Node *node = heap_pull(h); // get node whose distance from origin is smallest so far
	struct Edge *edge;
	while (node) {
		edge = node->edges;
		while (edge) {
			// only defference from Dijkstra's algorithm
			if (edge->node->explored == 0 && edge->d < edge->node->d) {
				edge->node->d = edge->d;
				edge->node->only_neigh_id = node->id;
			}
			edge = edge->next;
		}
		min_heapify(h, 1);
		//add this node into final collection
		inject_into_collection(node);
		//heap sort, update order
		min_heapify(h, 1);
		// get node whose distance from origin is smallest so far
		node  = heap_pull(h);
	}
}

void print_collection() {
	struct Node_collection *collection = explored_nodes;
	printf("Chosen edges:\n");
	while (collection) {
		if (collection->node->d != 0) { // do not print root (eg. {0, 0})
			printf("\t{%d, %d}, cost:%d\n", collection->node->id, collection->node->only_neigh_id, collection->node->d);
		}
		collection = collection->next;
	}
}
void free_edge(struct Edge* edge) {
	if (edge) {
		free_edge(edge->next);
		free(edge);
	}
}
void free_collection(struct Node_collection *collection) {
	if (collection) {
		free_collection(collection->next);
		free(collection);
	}
}
void free_all(heap *h) {
	for (int i = 0; i < 7; i++) {
		free_edge(nodes[i]->edges);
		free(nodes[i]);
	}
	free_collection(explored_nodes);
	heap_destroy(h);
}
int main() {
	heap* h = heap_create();
	make_graph(&h);
	Prim_algorithm(h);
	print_collection();
	free_all(h);
}
