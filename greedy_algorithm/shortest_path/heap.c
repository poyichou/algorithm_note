#include <stdio.h>
#include <stdlib.h>
#include "shortest_path.h"
#include "heap.h"

heap *heap_create() {
	heap *h = malloc(sizeof(int) * 2 + sizeof(struct Node*));
	h->capacity = 0;
	h->heapsize = 0;
	return h;
}

void heap_destroy(heap* h) {
	free(h);
}

void heap_insert(heap **h, struct Node* node) {
	if ((*h)->heapsize >= (*h)->capacity) {
		if ((*h)->capacity == 0) {
			*h = realloc(*h, sizeof(int) * 2 + sizeof(struct Node*) * (10 + 1));
			(*h)->capacity = 10;
		} else {
			*h = realloc(*h, sizeof(int) * 2 + sizeof(struct Node*) * ((*h)->capacity * 2 + 1));
			(*h)->capacity *= 2;
		}
	}
	(*h)->heapsize++;
	(*h)->nodes[(*h)->heapsize] = node;
	return;
}

void max_sink_down(heap* h, int selfidx) {
	int childidx;
	if (selfidx * 2 + 1 <= h->heapsize) {
		// right child exists
		if (h->nodes[selfidx * 2]->d >= h->nodes[selfidx * 2 + 1]->d && h->nodes[selfidx]->d < h->nodes[selfidx * 2]->d) {
			// left child is greater than right child and parent
			// swap with left child
			childidx = selfidx * 2;
		} else if (h->nodes[selfidx * 2]->d < h->nodes[selfidx * 2 + 1]->d && h->nodes[selfidx]->d < h->nodes[selfidx * 2 + 1]->d) {
			// right child is greater than left child and parent
			// swap with right child
			childidx = selfidx * 2 + 1;
		} else
			return;
	} else if (selfidx * 2 <= h->heapsize && h->nodes[selfidx]->d < h->nodes[selfidx * 2]->d) {
		// right child does not exist and left child is greater than parent
		childidx = selfidx * 2;
	} else
		return;

	struct Node *node;
	// swap with child
	node = h->nodes[selfidx];
	h->nodes[selfidx] = h->nodes[childidx];
	h->nodes[childidx] = node;

	max_sink_down(h, childidx);
	return;
}
void min_sink_down(heap* h, int selfidx) {
	int childidx;
	if (selfidx * 2 + 1 <= h->heapsize) {
		// right child exists
		if (h->nodes[selfidx * 2]->d <= h->nodes[selfidx * 2 + 1]->d && h->nodes[selfidx * 2]->d < h->nodes[selfidx]->d) {
			// left child is less than right child and parent
			// swap with left child
			childidx = selfidx * 2;
		} else if (h->nodes[selfidx * 2 + 1]->d < h->nodes[selfidx * 2]->d && h->nodes[selfidx * 2 + 1]->d < h->nodes[selfidx]->d) {
			// right child is less than left child and parent
			// swap with right child
			childidx = selfidx * 2 + 1;
		} else
			return;
	} else if (selfidx * 2 <= h->heapsize && h->nodes[selfidx * 2]->d < h->nodes[selfidx]->d) {
		// onlt left child exists and left child is less than parent
		childidx = selfidx * 2;
	} else
		return;

	struct Node *node;
	// swap with child
	node = h->nodes[selfidx];
	h->nodes[selfidx] = h->nodes[childidx];
	h->nodes[childidx] = node;

	min_sink_down(h, childidx);
}

// bottom-up
// selfidx of root == 1
// according to node->d
void max_heapify(heap* h, int selfidx) {
	// heapify it's left child and right child
	if (selfidx * 2 <= h->heapsize) {
		max_heapify(h, selfidx * 2);
	}
	if (selfidx * 2 + 1 <= h->heapsize) {
		max_heapify(h, selfidx * 2 + 1);
	}
	//sink down itself
	max_sink_down(h, selfidx);
}
//NOTE
void min_heapify(heap* h, int selfidx) {
	// heapify it's left child and right child
	if (selfidx * 2 <= h->heapsize) {
		min_heapify(h, selfidx * 2);
	}
	if (selfidx * 2 + 1 <= h->heapsize) {
		min_heapify(h, selfidx * 2 + 1);
	}
	//sink down itself
	min_sink_down(h, selfidx);
}

/**
  pull root
  it's user's res[onsibility to heapify
*/
struct Node *heap_pull(heap *h) {
	if (h->heapsize <= 0) {
		return NULL;
	}
	struct Node *node = h->nodes[1];
	h->nodes[1] = h->nodes[h->heapsize];
	h->nodes[h->heapsize] = NULL;
	h->heapsize--;
	return node;
}

void print_heap(heap* h) {
	for (int i = 1; i <= h->heapsize; i++) {
		printf("%d\n", h->nodes[i]->d);
	}
}
