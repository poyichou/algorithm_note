/**
  Bipartite Matching problem
  Goal: match pairs according to the preference graph as many as possible
  After 
    changing the edges into directed edges pointing from men to women,
    adding a node pointing to every man,
    adding a node pointed by every woman 
    and set capacity of each edge to 1,
  consider it as a network flow problem
*/
#include <stdio.h>
#include <stdlib.h>
struct Person {
	int id;
	int explored;
	struct Edge *outs;
};
struct Edge {
	struct Person *person;
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

static struct Person *people[12];
static struct Path_collection *path_collection = NULL;

struct Person* create_person(int id) {
	struct Person *p = malloc(sizeof(struct Person));
	p->id = id;
	p->explored = 0;
	p->outs = NULL;
	return p;
}
void make_people() {
	for (int i = 0; i < 12; i++) {
		people[i] = create_person(i);
	}
}
struct Edge *create_edge(int id) {
	struct Edge *e = malloc(sizeof(struct Edge));
	e->person = people[id];
	e->used = 0;
	e->capacity = 1;
	e->next = NULL;
	return e;
}
void make_edges(struct Person *p, int out_ids[], int olen) {
	struct Edge *e;
	for (int i = 0; i < olen; i++) {
		if (p->outs == NULL) {
			p->outs = create_edge(out_ids[i]);
			e = p->outs;
		} else {
			e->next = create_edge(out_ids[i]);
			e = e->next;
		}
	}
}
/**
Original graph is an undirected graph,
node 1-5 are men, node 6-10 are women,
and each edge represent man and woman on the two sides like each other.
However, to solve the problem, we make undirected edges into edges point from left to right, and add node 0 pointing to node 1-5, and node 11 pointed by node 6-10.

Nodes:

    1   6
    2   7
0   3   8   11
    4   9
    5   10

Edges:

0-->{1,2,3,4,5}
1-->{6,7}
2-->{7}
3-->{6,8,9}
4-->{7,10}
5-->{7,10}
6-->{11}
7-->{11}
8-->{11}
9-->{11}
10-->{11}

*/
void make_all_edges() {
	int e0[] = {1,2,3,4,5};
	int e1[] = {6,7};
	int e2[] = {7};
	int e3[] = {6,8,9};
	int e4[] = {7,10};
	int e5[] = {7,10};
	int e6[] = {11};
	int e7[] = {11};
	int e8[] = {11};
	int e9[] = {11};
	int e10[] = {11};
	make_edges(people[0], e0, 5);
	make_edges(people[1], e1, 2);
	make_edges(people[2], e2, 1);
	make_edges(people[3], e3, 3);
	make_edges(people[4], e4, 2);
	make_edges(people[5], e5, 2);
	make_edges(people[6], e6, 1);
	make_edges(people[7], e7, 1);
	make_edges(people[8], e8, 1);
	make_edges(people[9], e9, 1);
	make_edges(people[10], e10, 1);
}
void make_graph() {
	make_people();
	make_all_edges();
}
void augment_edge(struct Person *s, struct Person *t, int capacity, int flow) {
	struct Edge *prev = NULL, *out = t->outs;
	while (out) {
		if (out->person == s) {
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
		t->outs = create_edge(s->id);
		t->outs->used = capacity - flow;
	} else {
		prev->next = create_edge(s->id);
		prev->next->used = capacity - flow;
	}
}
void update_capacity(struct Person *person, struct Path *path, int flow) {
	struct Path *p = path;
	// reset not explored
	person->explored = 0;
	while (p) {
		p->edge->used += flow;
		// reset not explored
		p->edge->person->explored = 0;
		// add augment edge
		augment_edge(person, p->edge->person, p->edge->capacity, flow);
		person = p->edge->person;
		p = p->next;
	}
}
// destination is person 11, fill up **path
int find_path(struct Person *person, struct Path **path) {
	struct Edge *out = person->outs;
	struct Path *p;
	int remain;
	person->explored = 1;
	while (out) {
		if (out->person->explored == 0 && out->capacity - out->used > 0) {
			if (out->person->id == 11) {
				// Goal
				p = malloc(sizeof(struct Path));
				p->edge = out;
				p->next = *path;
				*path = p;
				return out->capacity - out->used;
			} else if ((remain = find_path(out->person, path)) > 0) {
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
	bottleneck = find_path(people[0], &path);
	while (bottleneck > 0) {
		update_capacity(people[0], path, bottleneck);
		// push in to collection
		collection = malloc(sizeof(struct Path_collection));
		collection->bottleneck = bottleneck;
		collection->path = path;
		collection->next = path_collection;
		path_collection = collection;

		path = NULL;
		bottleneck = find_path(people[0], &path);
	}
}
void print_result() {
	int flow = 0;
	struct Path_collection *collection = path_collection;
	struct Path *path;
	printf("========Flow network=========\n");
	while (collection) {
		path = collection->path;
		printf("Flow amount: %d\n\t0 ->", collection->bottleneck);
		flow += collection->bottleneck;
		while (path) {
			printf(" %d ", path->edge->person->id);
			if (path->next) {
				printf("->");
			}
			path = path->next;
		}
		printf("\n");
		collection = collection->next;
	}
	printf("Total flow amount: %d\n", flow);
	printf("=========Conclusion==========\nPairs are:\n");
	struct Edge *e;
	for (int i = 1; i <= 5; i++) {
		e = people[i]->outs;
		while (e) {
			if (e->used >= 1) {
				printf("%d -- %d (used %d)\n", people[i]->id, e->person->id, e->used);
			}
			e = e->next;
		}
	}
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
	for (int i = 0; i < 12; i++) {
		free_edge(people[i]->outs);
		free(people[i]);
	}
}
int main() {
	make_graph();
	ford_fulkerson();
	print_result();
	free_all();
}
