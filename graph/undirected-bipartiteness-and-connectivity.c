/*
  undirected graph
  test connectivity and bipartiteness
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tool/queue.h"
struct Node{
        int id;
        int color; // 0:red, 1:blue
        int explored; // for BFS_search
        struct Neighbor* neighbors;
};
struct Neighbor{
        struct Node* node;
        int explored; // for test_connect
        struct Neighbor* next;
};
//for test_connect
struct Node_collection{
        struct Node* node;
        struct Node_collection* next;
};

static struct Node* (nodes[7]);
struct Node* create_node(int id) {
        struct Node* node = malloc(sizeof(struct Node));
        node->id = id;
        node->color = -1;
        node->explored = 0;
        node->neighbors = NULL;
        return node;
}

struct Neighbor* create_neighbor(int id) {
        struct Neighbor* neighbor = malloc(sizeof(struct Neighbor));
        neighbor->node = nodes[id];
        neighbor->explored = 0;
        neighbor->next = NULL;
        return neighbor;
}

void make_neighbors(struct Node* node, int neighbor_ids[], int len) {
        struct Neighbor* neighbor;
        for (int i = 0; i < len; i++) {
                if (node->neighbors == NULL) {
                        node->neighbors = create_neighbor(neighbor_ids[i]);
                        neighbor = node->neighbors;
                } else {
                        neighbor->next = create_neighbor(neighbor_ids[i]);
                        neighbor = neighbor->next;
                }
        }
}
void create_edges() {
        int neighbor0[4] = {1, 3, 4, 6};
        int neighbor1[3] = {0, 2, 5};
        int neighbor2[4] = {1, 4, 6, 3};
        int neighbor3[2] = {0, 2};
        int neighbor4[3] = {0, 2, 5};
        int neighbor5[3] = {1, 4, 6};
        int neighbor6[3] = {0, 2, 5};
        make_neighbors(nodes[0], neighbor0, 4);
        make_neighbors(nodes[1], neighbor1, 3);
        make_neighbors(nodes[2], neighbor2, 4);
        make_neighbors(nodes[3], neighbor3, 2);
        make_neighbors(nodes[4], neighbor4, 3);
        make_neighbors(nodes[5], neighbor5, 3);
        make_neighbors(nodes[6], neighbor6, 3);
}
void create_graph() {
        for (int i = 0; i < 7; i++) {
                nodes[i] = create_node(i);
        }
        create_edges();
}
//draw color
void BFS_search(queue* q) {
        struct Node *node;
        int color = 0;
        int painted_count = 0;
        struct Neighbor *neighbor;
        int size;
        while (painted_count < 7) {
                size = queue_size(q);
                while (size > 0) {
                        node = queue_pull(q);
                        // draw color
                        if (node->color == -1) {
                                node->color = color;
                                painted_count++;
                                if (painted_count >= 7) {
                                        break;
                                }
                        }
                        // create anther layer
                        neighbor = node->neighbors;
                        while (neighbor) {
                                if (neighbor->node->explored == 0) {
                                        queue_push(q, neighbor->node);
                                        neighbor->node->explored = 1;
                                } 
                                neighbor = neighbor->next;
                        }
                        size--;
                }
                color = color==0 ? 1 : 0;
        }
        return;
}
int check_color() {
        struct Neighbor *neighbor;
        for (int i = 0; i < 7; i++) {
                neighbor = nodes[i]->neighbors;
                while (neighbor) {
                        if (neighbor->node->color == nodes[i]->color) {
                                return 1; // the same color, wrong
                        }
                        neighbor = neighbor->next;
                }
        }
        return 0;
}
// BFS, O(n+m), n: number of nodes, m: number of edges
int test_bipartiteness() {
        queue *q = queue_create(-1);
        queue_push(q, nodes[0]);
        BFS_search(q);
        queue_destroy(q);
        return check_color();
}
void print_graph() {
        char color_id[2][8]; //0:red, 1:blue
        char temp[2];
        color_id[0][0] = 0;
        color_id[1][0] = 0;
        for (int i = 0; i < 7; i++) {
                if(nodes[i]->color == -1) {
                        printf("error\n");
                        return;
                }
                snprintf(temp, 2, "%d", nodes[i]->id);
                strcat(color_id[nodes[i]->color], temp);
        }
        for (int j = 0; j < 2; j++) {
                printf("%s id:\n", j==0 ? "Red" : "Blue");
                for (int i = 0; i < strlen(color_id[j]); i++) {
                        printf("\tid:%c\n", color_id[j][i]);
                }
        }
        return;
}
int should_stop(struct Node_collection* node_collection) {
        struct Node_collection* collection = node_collection;
        struct Neighbor *neighbor;
        while (collection) {
                neighbor = collection->node->neighbors;
                while (neighbor) {
                        if (neighbor->explored == 0) {
                                return 0; //should not stop
                        }
                        neighbor = neighbor->next;
                }
                collection = collection->next;
        }
        return 1; // should stop
}
int in_collection(struct Node_collection* node_collection, struct Node* node) {
        struct Node_collection* collection = node_collection;
        while (collection) {
                if (collection->node == node) {
                        return 1; // in collection
                }
                collection = collection->next;
        }
        return 0; // not in collection
}
int member_count(struct Node_collection* node_collection) {
        struct Node_collection* collection = node_collection;
        int count = 0;
        while (collection) {
                count++;
                collection = collection->next;
        }
        return count;
}
void free_collection(struct Node_collection* collection){
        if (collection) {
                free_collection(collection->next);
                free(collection);
        }
        return;
}
void free_neighbors(struct Neighbor *neighbor) {
        if (neighbor) {
                free_neighbors(neighbor->next);
                free(neighbor);
        }
        return;
}
void free_graph() {
        for (int i = 0; i < 7; i++) {
                free_neighbors(nodes[i]->neighbors);
                free(nodes[i]);
        }
        return;
}
int test_connect() {
        struct Node_collection* node_collection = malloc(sizeof(struct Node_collection));
        struct Neighbor *neighbor;
        node_collection->node = nodes[0];
        node_collection->next = NULL;
        while (should_stop(node_collection) == 0) {
                struct Node_collection* collection = node_collection;
                // test all node in the node_collection
                while (collection) {
                        neighbor = collection->node->neighbors;
                        // test all neighbors of the node
                        while (neighbor) {
                                if (neighbor->explored == 0) {
                                        if (!in_collection(node_collection, neighbor->node)) {
                                                // add into collection
                                                struct Node_collection* temp = malloc(sizeof(struct Node_collection));
                                                temp->node = neighbor->node;
                                                temp->next = node_collection;
                                                node_collection = temp;
                                        }
                                        neighbor->explored = 1;
                                }
                                neighbor = neighbor->next;
                        }
                        collection = collection->next;
                }
        }
        int count = member_count(node_collection);
        free_collection(node_collection);
        if (count != 7) {
                return 1;// not connected
        }
        return 0; // connected

}
int main() {
        create_graph();
        if (test_connect() == 1) {
                printf("it's not a connected graph\n");
        } else {
                printf("it's a connected graph\n");
        }
        if (test_bipartiteness() == 1) {
                printf("it's not a bipartite graph\n");
        } else {
                printf("it's a bipartite graph\n");
        }
        print_graph();
        free_graph();
        return 0;
}
