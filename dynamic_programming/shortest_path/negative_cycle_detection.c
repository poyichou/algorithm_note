/**
  Bellman Ford algorithm to detect negative cycle
  In shortest_path.c, for a graph with n nodes, we run bellman_ford() from 0 to n-1 edges
  if we run bellman_ford() from 0 to n edges and if table[n-1][j][0] > table[n][j][0],
  means we can find path with n edges in a n nodes graph which implies that there's a negative cycle
  To find negative cycles for all nodes, I add an additional node (node 0) and incoming edges from all other node with 0 cost
  and run bellman_ford() from 0 to n-1 edges (n: # nodes, including node 0)

  Usage of finding negative cycle
  Example:
    Currency Conversion
                            (327.25)
             /--------------------------------------\
	    |                                       |
            |               (0.003065)              v
            G <------------------------------------ $ <-------\
           ^ | \---------------  ----------------/  ^         |(0.008309)
           | |          (455.2)| |(1.3941)          |         |
 (0.004816)| |(208.1)          | |          (1.0752)|         ¥
           | v                 v v                  |         ^
            £ ----------------> F ----------------> E --------/(129.52)
                  (2.1904)             (0.6677)
    Goal: r(v1, v2) * r(v2, v3) * ... * r(vn, v1) > 1
    c(u, v) = -log(r(u, v))
    --> c(v1, v2) + c(v2, v3) + ... + c(vn, v1) < 0 --> finding negative cycle

*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
struct Node {
	int id;
	struct Edge *outs;
};
struct Edge {
	struct Node *node;
	int cost;
	struct Edge *next;
};
struct Node *nodes[7];

struct Node *create_node(int id) {
	struct Node *node = malloc(sizeof(struct Node));
	node->id = id;
	node->outs = NULL;
	return node;
}
void make_nodes() {
	for (int i = 0; i < 7; i++) {
		nodes[i] = create_node(i);
	}
}
struct Edge *create_edge(int id, int cost) {
	struct Edge *edge = malloc(sizeof(struct Edge));
	edge->node = nodes[id];
	edge->cost = cost;
	edge->next = NULL;
	return edge;
}
void make_edges(struct Node* node, int out_ids[], int out_costs[], int olen) {
	struct Edge *edge;
	for (int i = 0; i < olen; i++) {
		if (node->outs == NULL) {
			node->outs = create_edge(out_ids[i], out_costs[i]);
			edge = node->outs;
		} else {
			edge->next = create_edge(out_ids[i], out_costs[i]);
			edge = edge->next;
		}
	}
}
/*
Graph:

i: node id, (j): edge cost

       ----------
      /          \
     2 <---(-6)    \(-3)
     |      \      \
     |      |       \
 (-4)|   -->5-      |
     |  /     \(4)  |
     | /(-1)   \    |
     V/         \-->V
     3              1
     ^\         /-->^
     | \(-2)   /    |
     |  \     /(2)  |
  (8)|   -->6-      |
     |      |       /
     |      /      /
     4 <---(-3)   / (3)
     \           /
      -----------

                             (0)
      every other point ------------> 0
*/
void make_all_edges() {
	int out1[] = {0};
	int out2[] = {0, 1, 3};
	int out3[] = {0, 5, 6};
	int out4[] = {0, 1, 3};
	int out5[] = {0, 1, 2};
	int out6[] = {0, 1, 4};
	int cost1[] = {0};
	int cost2[] = {0, -3, -4};
	int cost3[] = {0, -1, -2};
	int cost4[] = {0, 3, 8};
	int cost5[] = {0, 4, -6};
	int cost6[] = {0, 2, -3};
	make_edges(nodes[1], out1, cost1, 1);
	make_edges(nodes[2], out2, cost2, 3);
	make_edges(nodes[3], out3, cost3, 3);
	make_edges(nodes[4], out4, cost4, 3);
	make_edges(nodes[5], out5, cost5, 3);
	make_edges(nodes[6], out6, cost6, 3);
}
void find_min_table_out(int table[][7][2], int nedge, struct Node *node, int *nearest_id, int *ncost) {
	// have called before
	struct Edge *out = node->outs;
	int min_cost = INT_MAX; // infinity
	int cost;
	*nearest_id = -1;
	*ncost = INT_MAX;  // infinity
	while (out) {
		cost = table[nedge][out->node->id][0] == INT_MAX ? INT_MAX : out->cost + table[nedge][out->node->id][0];
		if (cost < min_cost) {
			*nearest_id = out->node->id;
			*ncost      = out->cost;
			min_cost = cost;
		}
		out = out->next;
	}

}
// destination point is 0
void bellman_ford(int table[][7][2]) {
	// table[i][j][0] is shortest distance from node j to 0 with i edges
	// table[i][j][1] is id of next node in path
	// where i = 0, ..., n-1 and j = 0, ... n-1
	table[0][0][0] = 0;
	table[0][0][1] = -1;
	for (int i = 1; i < 7; i++) {
		// impossible to reach node 0 with no edge, set distance to infinity
		table[0][i][0] = INT_MAX;
		// not exist
		table[0][i][1] = -1;
	}
	int nid;
	int ncost;
	// with i edges
	for (int i = 1; i < 8; i++) {
		// for node j
		for (int j = 0; j < 7; j++) {
			find_min_table_out(table, i - 1, nodes[j], &nid, &ncost);
			if (nid >= 0) {
				if (table[i - 1][j][0] <= ncost + table[i - 1][nid][0]) {
					// case 1: the shortest path from node j to node 0 with i - 1 edges and i edges are the same
					table[i][j][0] = table[i - 1][j][0];
					table[i][j][1] = j;
				} else {
					// case 2: the shortest path from node j to node 0 is nodes[j]-->nearest_outgoing_node-->nodes[0]
					table[i][j][0] = ncost + table[i - 1][nid][0];
					table[i][j][1] = nid;
				}
			} else {
				// no incoming edge
				table[i][j][0] = table[i - 1][j][0];
				table[i][j][1] = j;
			}
		}
	}
}
// table[8][7][2]
void print_path(int table[][7][2]) {
	int nedge, id;
	for (int i = 1; i < 7; i++) {
		printf("Shortest path from %d to 0 (path length: %d):", i, table[7][i][0]);
		if (table[7][i][0] < table[6][i][0]) {
			printf(" <-- contain negative cycle");
		}
		printf("\n\t%d ->", i);
		nedge = 5;
		id = i;
		while (nedge > 0) {
			if (id != table[nedge][id==-1?i:id][1]){
				printf (" %d", table[nedge][id==-1?i:id][1]);
				id = table[nedge][id==-1?i:id][1];
				if (nedge > 1) {
					printf(" ->");
				} else {
					printf("\n");
				}
			}
			nedge--;

		}
	}
}
void free_edge(struct Edge* edge) {
	if (edge) {
		free_edge(edge->next);
		free(edge);
	}
}
void free_all() {
	for (int i = 0; i < 7; i++) {
		free_edge(nodes[i]->outs);
		free(nodes[i]);
	}
}
int main() {
	make_nodes();
	make_all_edges();
	int table[8][7][2];
	bellman_ford(table);
	print_path(table);
	free_all();
}
