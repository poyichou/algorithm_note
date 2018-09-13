/**
  shortest path problems in a directed graph
  Goal: find shortest paths from origin for all nodes
  Dijkstra's algorithm:
    Requirement:
      all distance must be greater or equal to 0
  Data structure (for enhancement of time complexity): implement priority queue with min binary heap (complete binary tree) to
  Time complexity: O(mlogn), m: # of edges, n: # of nodes
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"
#include "shortest_path.h"

static struct Node* (nodes[5]);
static struct Node_collection* node_collection = NULL;

struct Node *create_node(){
	static int idx = 0;
	struct Node* node = malloc(sizeof(struct Node));
	node->id = idx;
	node->d = INT_MAX; // supposed to be infinity
	node->outs = NULL;
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
		if (node->outs == NULL) {
			node->outs = create_edge(out_ids[i], out_d[i]);
			out = node->outs;
		} else {
			out->next = create_edge(out_ids[i], out_d[i]);
			out = out->next;
		}
	}
}

void create_all_edges() {
	int out0[2]  = {1, 3};
	int out1[2]  = {2, 3};
	int out2[1]  = {4};
	int out3[3]  = {1, 2, 4};
	int out4[2]  = {0, 2};
	int outd0[2] = {10, 5};
	int outd1[2] = {1, 2};
	int outd2[1] = {4};
	int outd3[3] = {3, 9, 2};
	int outd4[2] = {7, 6};
	make_edges(nodes[0], out0, outd0, 2);
	make_edges(nodes[1], out1, outd1, 2);
	make_edges(nodes[2], out2, outd2, 1);
	make_edges(nodes[3], out3, outd3, 3);
	make_edges(nodes[4], out4, outd4, 2);

}
void make_graph(heap** h) {
	for (int i = 0; i < 5; i++) {
		nodes[i] = create_node();
	}
	create_all_edges();
	nodes[0]->d = 0; // root
	for (int i = 0; i < 5; i++) {
		heap_insert(h, nodes[i]);
	}
	min_heapify(*h, 1);
}

void inject_into_collection(struct Node *node) {
	struct Node_collection *collection;
	collection = malloc(sizeof(struct Node_collection));
	collection->node = node;
	collection->next = node_collection;
	node_collection = collection;
}
//NOTE
void Dijkstra_algorithm(heap* h) {
	struct Node *node = heap_pull(h); // get node whose distance from origin is smallest so far
	struct Edge *edge;
	while (node) {
		edge = node->outs;
		while (edge) {
			if (node->d + edge->d < edge->node->d) {
				edge->node->d = node->d + edge->d;
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
	struct Node_collection *collection = node_collection;
	while (collection) {
		printf("id:%d, path length:%d", collection->node->id, collection->node->d);
		if (collection->node->d == 0) {
			printf(" <- root\n");
		} else {
			printf("\n");
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
	for (int i = 0; i < 5; i++) {
		free_edge(nodes[i]->outs);
		free(nodes[i]);
	}
	free_collection(node_collection);
	heap_destroy(h);
}
int main() {
	heap* h = heap_create();
	make_graph(&h);
	Dijkstra_algorithm(h);
	print_collection();
	free_all(h);
}
