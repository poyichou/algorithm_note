/**
  example: Music site tries to match music ranks of two people
  songs:          A,  B,  ... 
  My rank       : 1,  2,  ..., k
  another's rank: a1, a2, ..., ak
  inversion: if i < j && ai > aj
*/
#include <stdio.h>
// O(n)
int inter_inv(int rank[], int startidx, int middleidx, int endidx) {
	// rank[startidx..middleidx] and rank[middleidx+1..endidx]: sorted
	// inversion: rank[i] > rank[j] && i < j
	int lidx = startidx, ridx = middleidx + 1;
	int inv = 0;
	for (int i = ridx; i <= endidx; i++) {
		while(rank[lidx++] <= rank[i]);
		// the rest of them are greater than rank[i]
		inv += endidx - lidx + 1;
	}
	return inv;
}
void merge(int rank[], int startidx, int middleidx, int endidx) {
	// rank[startidx..middleidx] and rank[middleidx+1..endidx]: sorted
	int arr[endidx - startidx + 1];
	int idx = 0, lidx = startidx, ridx = middleidx + 1;
	while (lidx <= middleidx || ridx <= endidx) {
		if (lidx <= middleidx && ridx <= endidx) {
			if (rank[lidx] <= rank[ridx]) {
				arr[idx++] = rank[lidx++];
			} else {
				arr[idx++] = rank[ridx++];
			}
		} else if (lidx <= middleidx) {
			arr[idx++] = rank[lidx++];
		} else if (ridx <= endidx) {
			arr[idx++] = rank[ridx++];
		}
	}
	for (int i = 0; i < endidx - startidx + 1; i++) {
		rank[i + startidx] = arr[i];
	}
}
int merge_and_count(int rank[], int startidx, int middleidx, int endidx) {
	int inter_inversion = inter_inv(rank, startidx, middleidx, endidx);
	merge(rank, startidx, middleidx, endidx);
	return inter_inversion;

}
int sort_and_count(int rank[], int startidx, int endidx) {
	// rank[startidx..endidx]:initially unsorted
	if (startidx == endidx) return 0;
	else {
		int middleidx = (startidx + endidx) / 2;
		// inv within rank[startidx..middleidx]
		int intra_invl = sort_and_count(rank, startidx, middleidx);
		// inv within rank[middleidx+1..endidx]
		int intra_invr = sort_and_count(rank, middleidx + 1, endidx);
		// inv between two half
		int inter_inv = merge_and_count(rank, startidx, middleidx, endidx);
		return intra_invl + intra_invr + inter_inv;
	}
}
int main() {
	// myrank[] = {1, 2, ... , 12}
	int rank[] = {1, 5, 4, 8, 10, 2, 6, 9, 12, 11, 3, 7};
	printf("There are %d inversions between you and other person\n", sort_and_count(rank, 0, 11));
	return 0;
}
