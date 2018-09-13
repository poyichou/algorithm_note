// interval-scheduling with deadline
// goal schedule all request without overlapping to minimize the maximum lateness (lateness=max{0, finishtime(i)-deadline(i)})
// greedy rule: earliest deadline first
// inversion: In schedule if deadlein(i) > deadline(j), where i < j, there is an inversion
#include <stdio.h>
#include <stdlib.h>
struct Job{
	int id;
	int runtime;
	int deadline;
};
static struct Job* (jobs[6]);
void alloc_job(int runtime, int deadline) {
	static int idx = 0;
	struct Job *job = malloc(sizeof(struct Job));
	job->id = idx;
	job->runtime = runtime;
	job->deadline = deadline;
	jobs[idx++] = job;
}

void create_jobs() {
	alloc_job(2,  8);
	alloc_job(3,  6);
	alloc_job(3, 14);
	alloc_job(1,  9);
	alloc_job(2, 15);
	alloc_job(4,  9);
}

//according to their deadlines
void sort_jobs() {
	struct Job *job;
	//bubble sort, O(n^2) while it is possible to use O(nlign) sorting method
	for (int i = 0; i < 6; i++) {
		for (int j = i; j > 0; j--) {
			if (jobs[j - 1]->deadline > jobs[j]->deadline) {
				job = jobs[j - 1];
				jobs[j - 1] = jobs[j];
				jobs[j] = job;
			}
		}
	}
}

void print_result() {
	int time = 0;
	int max_late = 0, late;
	printf("Scheduling: ");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < jobs[i]->runtime; j++) {
			printf("%d", jobs[i]->id);
			time++;
		}
		late = jobs[i]->deadline - time;
		if (max_late < late) {
			max_late = late;
		}
	}
	printf("\nDeadline\n");
	for (int i = 0; i < 6; i++) {
		printf("\tdeadline of %d: %d\n", jobs[i]->id, jobs[i]->deadline);
	}
	printf("Maxmum lateness: %d\n", max_late);
}
void free_all() {
	for (int i = 0; i < 6; i++) {
		free(jobs[i]);
	}
}
int main() {
	create_jobs();
	sort_jobs();
	print_result();
	free_all();
}
