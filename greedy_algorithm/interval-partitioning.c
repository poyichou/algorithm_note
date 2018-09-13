// interval coloring
#include <stdio.h>
#include <stdlib.h>
struct Job{
	int id;
	int starttime;
	int finishtime;
};
struct Job_set{
	int count;
	int depth;
	struct Job* (jobs[]);
};
struct Job_seq{
	struct Job* job;
	struct Job_seq* next;
};
struct Core{
	struct Job_seq *job_seq;
	struct Job_seq *job_seq_tail;
	int latest_finish_time;
};
static struct Core *core;
static struct Job_set* job_set;
void init_job(int start, int finish) {
	static int idx = 0;
	struct Job *job = malloc(sizeof(struct Job));
	job->id = idx;
	job->starttime = start;
	job->finishtime = finish;
	job_set->jobs[idx++] = job;
}
void create_jobs() {
	job_set = malloc(sizeof(int)+sizeof(int)+sizeof(struct Job)*10);
	job_set->depth = 0; // compute later
	job_set->count = 10;
	init_job(3,   9);
	init_job(11, 13);
	init_job(0,   2);
	init_job(3,   5);
	init_job(7,  10);
	init_job(0,   5);
	init_job(9,  13);
	init_job(0,   2);
	init_job(7,  10);
	init_job(11, 13);
}
// according to start time
void sort_jobs() {
	int job_count = job_set->count;
	struct Job *job;
	for (int i = 0; i < job_count; i++) {
		for (int j = i - 1; j > 0; j--) {
			if (job_set->jobs[j - 1]->starttime > job_set->jobs[j]->starttime) {
				job = job_set->jobs[j - 1];
				job_set->jobs[j - 1] = job_set->jobs[j];
				job_set->jobs[j] = job;
			}
		}
	}
}
void sort_seq(int seq[], int job_count) {
	int temp;
	for (int i = 0; i < job_count; i++) {
		for (int j = 1; j <= i; j++) {
			if (seq[j - 1] > seq[j]) {
				temp = seq[j - 1];
				seq[j - 1] = seq[j];
				seq[j] = temp;
			}
		}
	}
}
void make_depth_weight(int depth_weight[], int startseq[], int finishseq[], int job_count) {
	int idx = 0, sidx = 0, fidx = 0;
	while (idx < job_count * 2) {
		// if start time and finish time are the same, finish time go first (minus 1)
		if (fidx < job_count && sidx < job_count) {
			if (finishseq[fidx] <= startseq[sidx]) {
				depth_weight[idx++] = -1;
				fidx++;
			} else {
				depth_weight[idx++] = 1;
				sidx++;
			}
		} else if (fidx < job_count) {
			depth_weight[idx++] = -1;
			fidx++;
		} else if (sidx < job_count) {
			depth_weight[idx++] = 1;
			sidx++;
		} else {
			printf("error1\n");
			printf ("%d, %d, %d\n", job_count, fidx, sidx);
			exit(1);
		}
	}
}
// NOTE
/* 
  maximum of overlapping jobs
  sort start times and finish times together (if finish times and start times are the same, finish times go first)
  scan, if start time, depth++, if finish time, depth--, record the maximum
*/
void compute_depth() {
	int job_count = job_set->count;
	int startseq[job_count];
	int finishseq[job_count];
	for (int i = 0; i < job_count; i++) {
		startseq[i] = job_set->jobs[i]->starttime;
		finishseq[i] = job_set->jobs[i]->finishtime;
	}
	// startseq[] is already in order after sort_jobs()
	sort_seq(finishseq, job_count);
	int depth_weight[job_count * 2];
	make_depth_weight(depth_weight, startseq, finishseq, job_count);
	int depth = 0;
	for (int i = 0; i < job_count * 2; i++) {
		depth += depth_weight[i];
		if (depth > job_set->depth) {
			job_set->depth = depth;
		}
	}
}
// alloc resource
void alloc_core() {
	int depth = job_set->depth;
	core = malloc(sizeof(struct Core) * depth);
	for (int i = 0; i < depth; i++) {
		core[i].job_seq = NULL;
		core[i].job_seq_tail = NULL;
		core[i].latest_finish_time = 0;
	}
}
void add_into_core(struct Core *core, struct Job *job) {
	// add to job_seq
	if (core->job_seq == NULL) {
		core->job_seq = malloc(sizeof(struct Job_seq));
		core->job_seq->job = job;
		core->job_seq_tail = core->job_seq;
	} else {
		core->job_seq_tail->next = malloc(sizeof(struct Job_seq));
		core->job_seq_tail->next->job = job;
		core->job_seq_tail = core->job_seq_tail->next;
	}
	core->job_seq_tail->next = NULL;
	// update latest_finish_time
	core->latest_finish_time = job->finishtime;
}
// NOTE
void job_partition() {
	int job_count = job_set->count;
	int depth = job_set->depth; // number of cores
	int i, j;
	// for all jobs
	for (i = 0; i < job_count; i++) {
		// select a valid core that latest_finish_time <= starttime to occupy interval
		for (j = 0; j < depth; j++) {
			if (core[j].latest_finish_time <= job_set->jobs[i]->starttime) {
				add_into_core(&(core[j]), job_set->jobs[i]);
				break;
			}
		}
		if (j >= depth) {
			// no match, not possible
			printf("error2\n");
			printf("id: %d [%d, %d)\n", i, job_set->jobs[i]->starttime, job_set->jobs[i]->finishtime);
			exit(1);
		}
	}
}
void print_result() {
	struct Job_seq *job_seq;
	int time;
	for (int i = 0; i < job_set->depth; i++) {
		job_seq = core[i].job_seq;
		time = 0;
		printf("Core %d:\t", i);
		while (job_seq) {
			while (time < job_seq->job->starttime) {
				printf("-");
				time++;
			}
			for (int j = job_seq->job->starttime; j < job_seq->job->finishtime; j++) {
				printf("%d", job_seq->job->id);
			}
			time = job_seq->job->finishtime;
			job_seq = job_seq->next;
		}
		printf("\n");
	}
}
void free_job_seq(struct Job_seq *job_seq) {
	if (job_seq) {
		free_job_seq(job_seq->next);
		free(job_seq);
	}
}
void free_all(){
	for (int i = 0; i < job_set->depth; i++) {
		free_job_seq(core[i].job_seq);
	}
	free(core);
	for (int i = 0; i < job_set->count; i++) {
		free(job_set->jobs[i]);
	}
	free(job_set);
}
int main(void){
	create_jobs();
	sort_jobs();
	compute_depth();
	alloc_core();
	job_partition();
	print_result();
	free_all();
	return 0;
}
