/**
  Flow Network problem
  Ford-Fulkerson algorithm
    Procedure:
      Find an path P in the residual graph
      Augment flow along path P and update the residual graph
      Repeat until get stuck
    Augment flow: if there's a edge s->t in path with flow amount F, add a reverse edge t->s with remained capacity F to let other paths can flow back

  The value of the maxflow equals the value of the min cut
  value of cut: cut the graph to separate source and sink, adding the capacities of forward edges
*/
#include <stdio.h>
#include <stdlib.h>
struct Node {
	int id;
	int explored;
	struct Edge *outs;
};
struct Edge {
	struct Node *node;
	int used;
	int capacity;
	struct Edge *next;
};
struct Path {
	struct Edge *edge;
	struct Path *next;
};
struct Path_collection {
	int bottleneck;
	struct Path *path;
	struct Path_collection *next;
};
static struct Node *nodes[6]; // source: node 0, sink: node 5
static struct Path_collection *path_collection = NULL;
struct Node *create_node(int id) {
	struct Node *node = malloc(sizeof(struct Node));
	node->id = id;
	node->explored = 0;
	node->outs = NULL;
	return node;
}
void make_nodes() {
	for (int i = 0; i < 6; i++) {
		nodes[i] = create_node(i);
	}
}
struct Edge *create_edge(int id, int capacity) {
	struct Edge *edge = malloc(sizeof(struct Edge));
	edge->node = nodes[id];
	edge->used = 0;
	edge->capacity = capacity;
	edge->next = NULL;
	return edge;
}
void make_edges(struct Node *node, int out_ids[], int capacities[], int olen) {
	struct Edge *edge;
	for (int i = 0; i < olen; i++) {
		if (node->outs == NULL) {
			node->outs = create_edge(out_ids[i], capacities[i]);
			edge = node->outs;
		} else {
			edge->next = create_edge(out_ids[i], capacities[i]);
			edge = edge->next;
		}
	}
}
/**
              (10)         (4)          (10)
            ------->1 -----------> 2----------\
            |       |\             ^           \
            |       | ---------\   |            \
            |       |    (8)    \  |             \
            |    (2)|            \ |(6)           \
            |       v             v|               v
source >>>> 0 ----> 3 -----------> 4 ------------> 5 <<<<< sink
              (10)       (9)             (10)
*/
void make_all_edges() {
	int out0[] = {1, 3};
	int out1[] = {2, 3, 4};
	int out2[] = {5};
	int out3[] = {4};
	int out4[] = {2, 5};

	int cpt0[] = {10, 10};
	int cpt1[] = {4, 2, 8};
	int cpt2[] = {10};
	int cpt3[] = {9};
	int cpt4[] = {6, 10};

	make_edges(nodes[0], out0, cpt0, 2);
	make_edges(nodes[1], out1, cpt1, 3);
	make_edges(nodes[2], out2, cpt2, 1);
	make_edges(nodes[3], out3, cpt3, 1);
	make_edges(nodes[4], out4, cpt4, 2);
}
void make_graph() {
	make_nodes();
	make_all_edges();
}
void augment_edge(struct Node *s, struct Node *t, int capacity, int flow) {
	struct Edge *prev = NULL, *out = t->outs;
	while (out) {
		if (out->node == s) {
			// increase remained amount
			out->used -= flow;
			return;
		}
		prev = out;
		out = out->next;
	}
	// not found, create a fake one
	if (prev == NULL) {
		// t has no outgoing edge
		t->outs = create_edge(s->id, capacity);
		t->outs->used = capacity - flow;
	} else {
		prev->next = create_edge(s->id, capacity);
		prev->next->used = capacity - flow;
	}
}
void update_capacity(struct Node *node, struct Path *path, int flow) {
	struct Path *p = path;
	// reset not explored
	node->explored = 0;
	while (p) {
		p->edge->used += flow;
		// reset not explored
		p->edge->node->explored = 0;
		// add augment edge
		augment_edge(node, p->edge->node, p->edge->capacity, flow);
		node = p->edge->node;
		p = p->next;
	}
}
// destination is node 5, fill up **path
int find_path(struct Node *node, struct Path **path) {
	struct Edge *out = node->outs;
	struct Path *p;
	int remain;
	node->explored = 1;
	while (out) {
		if (out->node->explored == 0 && out->capacity - out->used > 0) {
			if (out->node->id == 5) {
				p = malloc(sizeof(struct Path));
				p->edge = out;
				p->next = *path;
				*path = p;
				return out->capacity - out->used;
			} else if ((remain = find_path(out->node, path)) > 0) {
				p = malloc(sizeof(struct Path));
				p->edge = out;
				p->next = *path;
				*path = p;
				// return bottleneck
				return (out->capacity - out->used < remain) ? (out->capacity - out->used) : remain;
			}
		}
		out = out->next;
	}
	return 0;
}
void ford_fulkerson() {
	int bottleneck;
	struct Path *path = NULL;
	struct Path_collection *collection;
	bottleneck = find_path(nodes[0], &path);
	while (bottleneck > 0) {
		update_capacity(nodes[0], path, bottleneck);
		collection = malloc(sizeof(struct Path_collection));
		collection->bottleneck = bottleneck;
		collection->path = path;
		collection->next = path_collection;
		path_collection = collection;
		path = NULL;
		bottleneck = find_path(nodes[0], &path);
	}
}
void print_paths() {
	int flow = 0;
	struct Path_collection *collection = path_collection;
	struct Path *path;
	while (collection) {
		path = collection->path;
		printf("Flow amount: %d\n\t", collection->bottleneck);
		flow += collection->bottleneck;
		while (path) {
			printf(" %d ", path->edge->node->id);
			if (path->next) {
				printf("->");
			}
			path = path->next;
		}
		printf("\n");
		collection = collection->next;
	}
	printf("Total flow amount: %d\n", flow);
}
void free_edge(struct Edge *edge) {
	if (edge) {
		free_edge(edge->next);
		free(edge);
	}
}
void free_path(struct Path *path) {
	if (path) {
		free_path(path->next);
		free(path);
	}
}
void free_path_collection(struct Path_collection *collection) {
	if (collection) {
		free_path_collection(collection->next);
		free_path(collection->path);
		free(collection);
	}
	
}
void free_all() {
	free_path_collection(path_collection);
	for (int i = 0; i < 6; i++) {
		free_edge(nodes[i]->outs);
		free(nodes[i]);
	}
}
int main() {
	make_graph();
	ford_fulkerson();
	print_paths();
	free_all();
}
