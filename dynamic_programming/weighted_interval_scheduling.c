/**
  Weighted interval scheduling
  Goal: select jobs to get highest value
  Method1: Meoization
    Top-down
    An recursive algorithm
  Method2: Iteration
    Bottom-up
    An iteractive algorithm
    Construct solutions from the smallest subproblem to the largest one
    Compute every small piece
*/
/*
p(j) is the largest index i < j s.t. intervals i and j are disjoint after sorting based on finish time
a.k.a. finish_time(i) <= start_time(j)
1:####2####                           p(1) = 0
2:  ######4####                       p(2) = 0
3:           ####4####                p(3) = 1
4:   ######7##############            p(4) = 0
5:                     ####2###       p(5) = 3
6:                      ###1######    p(6) = 3

Oj = the optimal solution for intervals 1, ..., j
OPT(j) = the value of Oj for intervals 1, ..., j
e.g. O6 = {6, O3} or O5
OPT(6) = max(valeu6 + OPT(3), OPT(5))
*/
#include <stdio.h>
#include <stdlib.h>
struct Job {
	int start_time;
	int finish_time;
	int value;
	int p; // largest index s.t. intervals are disjoint
};
struct Job *jobs[6];
void create_job(int start, int finish, int value) {
	static int idx = 0;
	struct Job *job = malloc(sizeof(struct Job));
	job->start_time = start;
	job->finish_time = finish;
	job->value = value;
	jobs[idx++] = job;
}

void make_jobs() {
	create_job(1, 4, 2);
	create_job(2, 6, 4);
	create_job(5, 7, 4);
	create_job(3, 10, 7);
	create_job(8, 11, 2);
	create_job(9, 12, 1);
}
// based on their finish time
void sort_jobs() {
	struct Job *job;
	// bubble sort, but we can sort with O(nlogn) algorithm
	for (int i = 0; i < 6; i++) {
		for (int j = i; j > 0; j--) {
			if (jobs[j - 1]->finish_time > jobs[j]->finish_time) {
				job = jobs[j - 1];
				jobs[j - 1] = jobs[j];
				jobs[j] = job;
			}
		}
	}
}
void compute_p() {
	int j;
	for (int i = 0; i < 6; i++) {
		for (j = 0; jobs[j]->finish_time <= jobs[i]->start_time; j++);
		jobs[i]->p = j - 1;
	}
}
int max(int v1, int v2) {
	return v1 > v2 ? v1 : v2;
}
int M_compute_opt(int idx) {
	static int MM[6] = {-1, -1, -1, -1, -1, -1};
	if (idx == -1) return 0;
	else if (MM[idx] != -1) return MM[idx];
	else {
		MM[idx] = max(jobs[idx]->value + M_compute_opt(jobs[idx]->p), M_compute_opt(idx - 1));
		return MM[idx];
	}
	
}
int I_compute_opt(int idx) {
	int MI[idx + 1];
	int sol_1, sol_2;
	for (int i = 0; i <= idx; i++) {
		sol_1 = jobs[i]->value + (jobs[i]->p >= 0 ? MI[jobs[i]->p] : 0);
		sol_2 = i > 0 ? MI[i - 1] : 0;
		MI[i] = max(sol_1, sol_2);
	}
	return MI[idx];
}
void free_all() {
	for (int i = 0; i < 6; i++) {
		free(jobs[i]);
	}
}
int main() {
	make_jobs();
	sort_jobs();
	compute_p();
	printf("M: max value is %d\n", M_compute_opt(5));
	printf("I: max value is %d\n", I_compute_opt(5));
	free_all();
	return 0;
}
