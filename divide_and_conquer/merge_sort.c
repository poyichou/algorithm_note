/**
  implement merge sort
  Time complexity: O(nlogn)
  Proof: Tree

  T(n) = D(n) + 2T(n/2) + C(n) = O(1) + 2T(n/2) + O(n)
           ^Divide          ^Combine(Merge)

  T(n) <= 2(T/2) + cn

  Base: For n <= 2, T(n) <= c
  -----------------------------------------------------------------------------------------------------------
  T(n) = |       cn      = |              cn             = |            cn              ---->cn ---         |
         |                 |                               |                                     |          |
         |  T(n/2) T(n/2)  |       cn/2         cn/2       |     cn/2        cn/2       ---->cn  |          |
         |                 |                               |                                     |          |
         |                 |  T(n/4) T(n/4) T(n/4) T(n/4)  |  cn/4  cn/4  cn/4  cn/4    ---->cn  |          |
         |                 |                               |             .                       |  log(n)  |
         |                 |                               |             .              ---->cn  |          |
         |                 |                               |             .                       |          |
         |                 |                               |       T(2) ... T(2)        ---->cn ---         |
  -----------------------------------------------------------------------------------------------------------
  --->T(n) <= cn logn = O(nlogn)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// O(n)
void merge(char* str, int startidx, int middleidx, int endidx) {
	int leftidx = startidx, rightidx = middleidx + 1;
	int idx = 0;
	char arr[endidx - startidx + 1];
	while (leftidx <= middleidx || rightidx <= endidx) {
		if (leftidx <= middleidx && rightidx <= endidx) {
			if (str[leftidx] <= str[rightidx]) {
				arr[idx++] = str[leftidx++];
			} else {
				arr[idx++] = str[rightidx++];
			}
		} else if (leftidx <= middleidx) {
			arr[idx++] = str[leftidx++];
		} else if (rightidx <= endidx) {
			arr[idx++] = str[rightidx++];
		}
	}
	for (int i = 0; i < endidx - startidx + 1; i++) {
		str[i + startidx] = arr[i];
	}
}
// T(n)
void merge_sort(char *str, int startidx, int endidx) {
	// D(n)
	if (startidx < endidx) {
		int middleidx = (startidx + endidx) / 2;
		// 2T(n/2)
		merge_sort(str, startidx, middleidx);
		merge_sort(str, middleidx + 1, endidx);
		// O(n)
		merge(str, startidx, middleidx, endidx);
	}
}

int main() {
	char *str = strdup("ThisIsTestCodeForMergeSortAlgorithm");
	merge_sort(str, 0, strlen(str) - 1);
	printf("%s\n", str);
	free(str);
}
