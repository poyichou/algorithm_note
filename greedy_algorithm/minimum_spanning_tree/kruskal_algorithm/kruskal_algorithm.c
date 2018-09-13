/**
  Minimum Spanning Tree problems in a undirected graph
  Goal: find the lowest-costied edges that connect the graph, which is the Minimum Spanning Tree consist of
  Kruskal's algorithm:
    Requirement:
      all distance must be greater or equal to 0
  Data structure (for enhancement of time complexity): disjoint sets with union-find algorithm
  Complexty of Find, Union: O(alpha(n)), alpha(n) < 5 -> almost constant
  Time complexity: O(mlogn), m: # of edges, n: # of nodes
    1.sort edge costs
    2.Find() twice for each edge (m edges)
    3.Union() possivly once for each edge (totally n-1 edges would result in Union())

    Comparison sort + simple disjoint set: O(mlogn)
    Linear sort + union-find: O(m alpha(m, n)) -> almost linear time
*/
#include <stdio.h>
#include <stdlib.h>
struct Edge{
	struct Node* node[2];
	int cost;
};
// node of disjoint sets
struct Node{
	int id;
	// if it's not root, a.k.a. parent != itself, 
	// it cannot be other's representative
	struct Node *parent;
};
struct Edge_collection{
	struct Edge* edge;
	struct Edge_collection* next;
};

static struct Node* nodes[7];
static struct Edge* edges[11];
static struct Edge_collection *edge_collection = NULL;

struct Node *create_node(){
	static int idx = 0;
	struct Node* node = malloc(sizeof(struct Node));
	node->id = idx;
	node->parent = node;
	idx++;
	return node;
}
void make_nodes() {
	for (int i = 0; i < 7; i++) {
		nodes[i] = create_node();
	}
}
void create_edge(struct Node* node0, struct Node* node1, int cost) {
	static int idx = 0;
	struct Edge* edge = malloc(sizeof(struct Edge));
	edge->node[0] = node0;
	edge->node[1] = node1;
	edge->cost = cost;
	edges[idx] = edge;
	idx++;
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
void make_edges() {
	create_edge(nodes[0], nodes[1], 7);
	create_edge(nodes[1], nodes[2], 8);
	create_edge(nodes[1], nodes[3], 9);
	create_edge(nodes[1], nodes[4], 7);
	create_edge(nodes[0], nodes[3], 5);
	create_edge(nodes[2], nodes[4], 5);
	create_edge(nodes[3], nodes[4], 15);
	create_edge(nodes[3], nodes[5], 6);
	create_edge(nodes[4], nodes[5], 8);
	create_edge(nodes[5], nodes[6], 11);
	create_edge(nodes[4], nodes[6], 9);
}
void sort_edges() {
	struct Edge *edge;
	for (int i = 0; i < 11; i++) {
		for (int j = i; j > 0; j--) {
			if (edges[j - 1]->cost > edges[j]->cost) {
				edge = edges[j - 1];
				edges[j - 1] = edges[j];
				edges[j] = edge;
			}
		}
	}
}
// find representative, aka root
struct Node* Find(struct Node *node) {
	// has been Find() and the answer is valid
	if (node->parent->parent == node->parent) {
		return node->parent;
	}
	struct Node* parent = node->parent;
	while (parent->parent != parent) {
		parent = parent->parent;
	}
	// point to root
	node->parent = parent;
	return parent;
}
void Union(struct Node *node0, struct Node *node1){
	node1->parent = node0;
}
// would be called n-1 times, since it would be a connected graph with no cycle
void inject_into_collection(struct Edge *edge) {
	struct Edge_collection* collection = malloc(sizeof(struct Edge_collection));
	collection->edge = edge;
	collection->next = edge_collection;
	edge_collection = collection;
}
void kruskal_algorithm() {
	struct Node *root0, *root1;
	for (int i = 0; i < 11; i++) {
		root0 = Find(edges[i]->node[0]);
		root1 = Find(edges[i]->node[1]);
		// not connected
		if (root0 != root1) {
			Union(root0, root1);
			inject_into_collection(edges[i]);
		}
	}
}
void print_edges() {
	struct Edge_collection* collection = edge_collection;
	printf("Chosen edges:\n");
	while (collection) {
		printf("\t{%d, %d}, cost: %d\n", collection->edge->node[0]->id, collection->edge->node[1]->id, collection->edge->cost);
		collection = collection->next;
	}
}
void free_collection(struct Edge_collection* collection) {
	if (collection) {
		free_collection(collection->next);
		free(collection);
	}
}
void free_all() {
	free_collection(edge_collection);
	for (int i = 0; i < 7; i++) {
		free(nodes[i]);
	}
	for (int i = 0; i < 11; i++) {
		free(edges[i]);
	}
}
int main() {
	make_nodes();
	make_edges();
	sort_edges();
	kruskal_algorithm();
	print_edges();
	free_all();
	return 0;
}
