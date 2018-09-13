#pragma once

struct Node{
	int id;
	int d; // shortest distance from last point
	int explored; //0:not, 1:yes
	int only_neigh_id;
	struct Edge *edges;
};

// for final answer
struct Node_collection{
	struct Node* node;
	struct Node_collection* next;
};

// undirected edge
struct Edge{
	struct Node* node;
	int d; // distance from neighbor
	struct Edge* next;
};

