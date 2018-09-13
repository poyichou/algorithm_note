// interval-scheduling
// greedy rule: earliest finish time first
// sorting and not removing partial conflicts every time make it from O(n^2) to O(nlogn)
#include <stdio.h>
#include <stdlib.h>
struct Job{
	int id;
	int starttime;
	int finishtime;
	struct Job *next;
};
static struct Job* (jobs[9]);
static struct Job *head = NULL, *valid_set = NULL, *valid_set_tail = NULL;

void alloc_job(int start, int finish) {
	static int idx = 0;
	struct Job *job = malloc(sizeof(struct Job));
	job->id = idx;
	job->starttime = start;
	job->finishtime = finish;
	jobs[idx++] = job;
}

void create_jobs() {
	alloc_job(0, 11);
	alloc_job(13, 15);
	alloc_job(0, 2);
	alloc_job(3, 6);
	alloc_job(8, 10);
	alloc_job(12, 16);
	alloc_job(0, 4);
	alloc_job(5, 7);
	alloc_job(9, 11);
}

//according to their finish time
void sort_jobs() {
	struct Job *job;
	//bubble sort, O(n^2) while it is possible to use O(nlign) sorting method
	for (int i = 0; i < 9; i++) {
		for (int j = i; j > 0; j--) {
			if (jobs[j - 1]->finishtime > jobs[j]->finishtime) {
				job = jobs[j - 1];
				jobs[j - 1] = jobs[j];
				jobs[j] = job;
			}
		}
	}
	head = jobs[0];
	for (int i = 0; i < 8; i++) {
		jobs[i]->next = jobs[i + 1];
	}
}
void inject_to_valid_set(struct Job* job) {
	job->next = NULL;
	if (valid_set == NULL) {
		valid_set = job;
	} else {
		valid_set_tail->next = job;
	}
	valid_set_tail = job;
}
/**
  remove those whose start time are later than finish untill valid one appears
  it may leave partial conflict jobs
  not scanning all because invalid jobs remained would be removed later
*/
void remove_conflict(int finish) {
	while (head) {
		if (head->starttime < finish) {
			head = head->next;
		} else {
			break;
		}
	}
}
void print_answer() {
	struct Job* job = valid_set;
	while (job) {
		if (job->next)
			printf("id:%d[%d, %d) -> ", job->id, job->starttime, job->finishtime);
		else
			printf("id:%d[%d, %d)\n", job->id, job->starttime, job->finishtime);
		job = job->next;
	}
}
void greedy_algorithm() {
	struct Job* job;
	while (head) {
		job = head;
		// not removing all but just till compatible one appears
		remove_conflict(job->finishtime);
		inject_to_valid_set(job);
	}
	return;
}
void free_jobs() {
	for (int i = 0; i < 9; i++) {
		free(jobs[i]);
	}
	return;
}
int main(void) {
	create_jobs();
	sort_jobs();
	greedy_algorithm();
	print_answer();
	free_jobs();
}
