#pragma once

struct Node{
	int id;
	int d; // distance from origin
	struct Edge *outs;
};

// for final answer
struct Node_collection{
	struct Node* node;
	struct Node_collection* next;
};

// directed edge
struct Edge{
	struct Node* node;
	int d; // distance from neighbor
	struct Edge* next;
};

