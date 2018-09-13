/*
  fibonacci sequence
*/
#include <stdio.h>
#include <stdlib.h>
static int *table;
int fibonacci(int idx) {
	if (idx == 0) return 0;
	else if (idx == 1) return 1;
	else {
		if (table[idx - 2] == -1) {
			table[idx - 2] = fibonacci(idx - 2);
		}
		if (table[idx - 1] == -1) {
			table[idx - 1] = fibonacci(idx - 1);
		}
		return table[idx - 1] + table[idx - 2];
	}
}
int main() {
	int idx;
	printf("enter idx of fibonacci:");
	scanf("%d", &idx);
	table = malloc(sizeof(int) * (idx + 1));
	for (int i = 0; i < idx + 1; i++) {
		table[i] = -1;
	}
	printf("fibonacci[%d] is %d\n", idx, fibonacci(idx));
	free(table);
	return 0;
}
