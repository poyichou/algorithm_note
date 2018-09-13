/*
  create topology order of an DAG(directed acyclic graph)
  O(n+m), n: number of nodes, m: number of edges
*/
#include <stdio.h>
#include <stdlib.h>
#include "../tool/queue.h"
struct Node{
	int id;
	int topo_order;
	int in_count;
	struct Edge *ins;
	struct Edge *outs;
};

struct Edge{
	struct Node* node;
	struct Edge* next;
};

static struct Node* (nodes[7]);

struct Node* create_node(int id) {
	struct Node* node = malloc(sizeof(struct Node));
	node->id = id;
	node->topo_order = -1;
	node->in_count = 0;
	node->ins = NULL;
	node->outs = NULL;
	return node;
}
struct Edge* create_edge(int id) {
	struct Edge* edge = malloc(sizeof(struct Edge));
	edge->node = nodes[id];
	edge->next = NULL;
	return edge;
}
void make_edges(struct Node* node, int in_ids[], int ilen, int out_ids[], int olen) {
	struct Edge *in, *out;
	for (int i = 0; i < ilen; i++) {
		if (node->ins == NULL) {
			node->ins = create_edge(in_ids[i]);
			in = node->ins;
		} else {
			in->next = create_edge(in_ids[i]);
			in = in->next;
		}
		node->in_count++;
	}
	for (int i = 0; i < olen; i++) {
		if (node->outs == NULL) {
			node->outs = create_edge(out_ids[i]);
			out = node->outs;
		} else {
			out->next = create_edge(out_ids[i]);
			out = out->next;
		}
	}
}
void create_all_edges() {
	int in2[1] = {1};
	int in3[2] = {0, 2};
	int in4[4] = {0, 1, 2, 3};
	int in5[2] = {1, 4};
	int in6[3] = {0, 4, 5};
	int out0[3] = {3, 4, 6};
	int out1[3] = {2, 4, 5};
	int out2[2] = {4, 3};
	int out3[1] = {4};
	int out4[2] = {5, 6};
	int out5[1] = {6};
	make_edges(nodes[0], NULL, 0, out0, 3);
	make_edges(nodes[1], NULL, 0, out1, 3);
	make_edges(nodes[2], in2,  1, out2, 2);
	make_edges(nodes[3], in3,  2, out3, 1);
	make_edges(nodes[4], in4,  4, out4, 2);
	make_edges(nodes[5], in5,  2, out5, 1);
	make_edges(nodes[6], in6,  3, NULL, 0);
}
void create_all_edges_ver2() {
	int in2[1] = {5};
	int in3[2] = {1, 4};
	int in4[4] = {1, 5, 4, 6};
	int in5[2] = {5, 0};
	int in6[3] = {1, 0, 2};
	int out0[3] = {6, 0, 3};
	int out1[3] = {4, 0, 2};
	int out2[2] = {0, 6};
	int out3[1] = {0};
	int out4[2] = {2, 3};
	int out5[1] = {3};
	make_edges(nodes[1], NULL, 0, out0, 3);
	make_edges(nodes[5], NULL, 0, out1, 3);
	make_edges(nodes[4], in2,  1, out2, 2);
	make_edges(nodes[6], in3,  2, out3, 1);
	make_edges(nodes[0], in4,  4, out4, 2);
	make_edges(nodes[2], in5,  2, out5, 1);
	make_edges(nodes[3], in6,  3, NULL, 0);
	/*
		1546023
	*/
}
void print_graph() {
	int outcount;
	struct Edge *out;
	printf("Graph:\n");
	for (int i = 0; i < 7; i++){
		outcount = 0;
		out = nodes[i]->outs;
		while (out) {
			outcount++;
			out = out->next;
		}
		printf("\tid: %d, %d incoming edges, %d outgoing edges\n", nodes[i]->id, nodes[i]->in_count, outcount);
	}
}
void create_directed_acyclic_graph() {
	for (int i = 0; i < 7; i++) {
		nodes[i] = create_node(i);
	}
	create_all_edges_ver2();
	//create_all_edges();
}
void initialize_queue(queue *q) {
	for (int i = 0; i < 7; i++) {
		if(nodes[i]->in_count == 0) {
			queue_push(q, nodes[i]);
		}
	}
}
// NOTE
void topo_ordering() {
	queue *q = queue_create(-1);
	// stores those who don't have incoming edge
	initialize_queue(q);
	struct Node* node;
	struct Edge* out;
	int topo_order = 0;
	while (queue_size(q) > 0) {
		// select one node who has no incoming edge (depending on in_count)
		node = queue_pull(q);
		// give it an order number
		node->topo_order = topo_order;
		topo_order++;
		out = node->outs;
		// select it's neighbors at outgoing edges
		while (out) {
			// reduce the neighbors' number of incoming edge (in_count)
			out->node->in_count--;
			//when in_count reachs 0, push it into the queue
			if (out->node->in_count == 0) {
				queue_push(q, out->node);
			}
			out = out->next;
		}
	}
	queue_destroy(q);
}
void print_topo_order() {
	printf("topo order:\n\t");
	//for (int i = 0; i < 7; i++) {
	//	printf("\tid: %d, topology order: %d\n", nodes[i]->id, nodes[i]->topo_order);
	//}
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++){
			if (nodes[j]->topo_order == i){ 
				if (i == 6) {
					printf("%d\n", nodes[j]->id);
				} else {
					printf("%d -> ", nodes[j]->id);
				}
			}
		}
	}
}
void free_edge(struct Edge* edge) {
	if (edge) {
		free_edge(edge->next);
		free(edge);
	}
}
void free_graph() {
	for (int i = 0; i < 7; i++) {
		free_edge(nodes[i]->ins);
		free_edge(nodes[i]->outs);
		free(nodes[i]);
	}
}
int main() {
	create_directed_acyclic_graph();
	print_graph();
	topo_ordering();
	print_topo_order();
	free_graph();
}
