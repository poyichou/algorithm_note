/**
  Bellman Ford algorithm
  In this implementation, we only consider simple path (no cycle), 
  we don't consider negative cycle
  OPT(i, v) =  0										if i == 0 && v == t
               infinity										if i == 0 && v != 0
	       min(OPT(i - 1, v), min(for edge (v, w) exists: c(v, w) + OPT(i - 1, w)))		otherwise
  Running time:
    1. naive: O(n^3)	(seems like)
    2. detailed: O(nm)	(actually)
    where n = # of nodes; m = # of edges
  Space: 
    normal: O(n^2)		<- able to record path
    efficient versoion: O(n)	<- unable to record path
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
struct Node *nodes[6];

struct Node *create_node(int id) {
	struct Node *node = malloc(sizeof(struct Node));
	node->id = id;
	node->outs = NULL;
	return node;
}
void make_nodes() {
	for (int i = 0; i < 6; i++) {
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
     1 <---(6)    \(-3)
     |      \      \
     |      |       \
 (-4)|   -->4-      |
     |  /     \(4)  |
     | /(-1)   \    |
     V/         \-->V
     2              0
     ^\         /-->^
     | \(-2)   /    |
     |  \     /(2)  |
  (8)|   -->5-      |
     |      |       /
     |      /      /
     3 <---(-3)   / (3)
     \           /
      -----------
*/
void make_all_edges() {
	int out1[] = {0, 2};
	int out2[] = {4, 5};
	int out3[] = {0, 2};
	int out4[] = {0, 1};
	int out5[] = {0, 3};
	int cost1[] = {-3, -4};
	int cost2[] = {-1, -2};
	int cost3[] = {3, 8};
	int cost4[] = {4, 6};
	int cost5[] = {2, -3};
	make_edges(nodes[1], out1, cost1, 2);
	make_edges(nodes[2], out2, cost2, 2);
	make_edges(nodes[3], out3, cost3, 2);
	make_edges(nodes[4], out4, cost4, 2);
	make_edges(nodes[5], out5, cost5, 2);
}
void find_min_table_out(int table[][6][2], int nedge, struct Node *node, int *nearest_id, int *ncost) {
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
// the table[6][6][2] costs a lot of space, but can also record path
void bellman_ford(int table[][6][2]) {
	// table[i][j][0] is shortest distance from node j to 0 with i edges
	// table[i][j][1] is id of next node in path
	// where i = 0, ..., n-1 and j = 0, ... n-1
	table[0][0][0] = 0;
	table[0][0][1] = -1;
	for (int i = 1; i < 6; i++) {
		// impossible to reach node 0 with no edge, set distance to infinity
		table[0][i][0] = INT_MAX;
		// not exist
		table[0][i][1] = -1;
	}
	int nid;
	int ncost;
	// with i edges
	for (int i = 1; i < 6; i++) {
		// for node j
		for (int j = 0; j < 6; j++) {
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
void print_path(int table[][6][2]) {
	int nedge, id;
	for (int i = 1; i < 6; i++) {
		printf("Shortest path from %d to 0 (path length: %d):\n\t%d ->", i, table[5][i][0], i);
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
void find_min_table_out_efficient(int table[][6], struct Node *node, int *nearest_id, int *ncost) {
	// have called before
	struct Edge *out = node->outs;
	int min_cost = INT_MAX; // infinity
	int cost;
	*nearest_id = -1;
	*ncost = INT_MAX;  // infinity
	while (out) {
		cost = table[0][out->node->id] == INT_MAX ? INT_MAX : out->cost + table[0][out->node->id];
		if (cost < min_cost) {
			*nearest_id = out->node->id;
			*ncost      = out->cost;
			min_cost = cost;
		}
		out = out->next;
	}

}
// destination point is 0
// the table[2][6] is much more efficient in space usage, but has no ability of recording path
void bellman_ford_efficient(int table[][6]) {
	// table[0][j] is shortest distance from node j to 0 with i - 1 edges
	// where i = 0, ..., n-1 and j = 0, ... n-1
	table[0][0] = 0;
	for (int i = 1; i < 6; i++) {
		// impossible to reach node 0 with no edge, set distance to infinity
		table[0][i] = INT_MAX;
	}
	int nid;
	int ncost;
	// with i edges
	for (int i = 1; i < 6; i++) {
		// for node j
		for (int j = 0; j < 6; j++) {
			find_min_table_out_efficient(table, nodes[j], &nid, &ncost);
			if (nid >= 0) {
				if (table[0][j] <= ncost + table[0][nid]) {
					// case 1: the shortest path from node j to node 0 with i - 1 edges and i edges are the same
					table[1][j] = table[0][j];
				} else {
					// case 2: the shortest path from node j to node 0 is nodes[j]-->nearest_outgoing_node-->nodes[0]
					table[1][j] = ncost + table[0][nid];
				}
			} else {
				// no incoming edge
				table[1][j] = table[0][j];
			}
		}
		// update for next loop use
		for (int k = 0; k < 6; k++) {
			table[0][k] = table[1][k];
		}
	}
}
void print_efficient_result(int table[][6]) {
	for (int i = 1; i < 6; i++) {
		printf("Shortest path length from %d to 0 is %d\n", i, table[1][i]);
	}
}
void free_edge(struct Edge* edge) {
	if (edge) {
		free_edge(edge->next);
		free(edge);
	}
}
void free_all() {
	for (int i = 0; i < 6; i++) {
		free_edge(nodes[i]->outs);
		free(nodes[i]);
	}
}
int main() {
	make_nodes();
	make_all_edges();
	int table[6][6][2];
	bellman_ford(table);
	printf("============ Normal ver. ============\n");
	print_path(table);
	int etable[2][6];
	bellman_ford_efficient(etable);
	printf("=========== Efficient ver. ==========\n");
	print_efficient_result(etable);
	free_all();
}
