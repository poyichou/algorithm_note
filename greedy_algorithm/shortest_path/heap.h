#pragma once

typedef struct Heap heap;

struct Heap{
	int capacity; //capacity of nodes, not include nodes[0]
	int heapsize; // not include nodes[0]
	struct Node *(nodes[]); //nodes[0] would not be used, nodes start from 1
};

heap *heap_create();

void heap_destroy(heap* h);

void heap_insert(heap **h, struct Node* node);

void max_heapify(heap* h, int selfidx);

void min_heapify(heap* h, int selfidx);

struct Node *heap_pull(heap *h);

void print_heap(heap* h);
