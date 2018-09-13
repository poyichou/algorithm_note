/**
  knapsack problem, 0-1 knapsack
  Goal: for a knapsack of # capacity, and items with weights, value, fill the knapsack to approach max value
  Time complexity: O(nW), n: # items, W: capacity
  W not polynomial with input size
  pseudo-polynomial
  NP-complete problem
*/
#include <stdio.h>
#include <stdlib.h>
struct Item{
	int id;
	int value;
	int weight;
};
struct Item* items[5];
void create_item(int value, int weight) {
	static int idx = 0;
	struct Item *item = malloc(sizeof(struct Item));
	// start with 1
	item->id = idx + 1;
	item->value = value;
	item->weight = weight;
	items[idx++] = item;
}
void make_items() {
	create_item(1, 1);
	create_item(6, 2);
	create_item(18, 5);
	create_item(22, 6);
	create_item(28, 7);
}
int max(int v1, int v2) {
	return v1 > v2 ? v1 : v2;
}
int subset_sum(int item_count, int capacity, int table[][12]) {
	for (int i = 0; i <= item_count; i++) {
		// zero capacity left
		table[i][0] = 0;
	}
	for (int i = 0; i <= capacity; i++) {
		// no item left
		table[0][i] = 0;
	}
	int idx;
	// for first i items
	for (int i = 1; i <= item_count; i++) {
		idx = i - 1;
		// for a w-capacity knapsack
		for (int w = 1; w <= capacity; w++) {
			if (items[i - 1]->weight > w) {
				// no capacity left to include items[i - 1]
				table[i][w] = table[i - 1][w];
			} else {
				// case 1: it should include items[idx]; case 2: it shouldn't include items[idx]
				table[i][w] = max(table[i - 1][w], items[idx]->value + table[i - 1][w - items[idx]->weight]);
			}
		}
	}
	return table[item_count][capacity];
}
void free_all() {
	for (int i = 0; i < 5; i++) {
		free(items[i]);
	}
}
int main() {
	// 5 items, knapsack capacity is 11
	// table[i][j] record max value with first i items and j capacity
	// table[0][j] and table[i][0] must be 0; where i = 0, ..., 5 and j = 0, ... 11
	// i = first 0 item, namely, no item, j = 0, namly 0 capacity
	int table[5 + 1][11 + 1];
	make_items();
	printf("capacity of knapsack is %d, it can contain items of %d value\n", 11, subset_sum(5, 11, table));
	free_all();
	return 0;
}
