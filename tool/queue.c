#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * This queue is implemented with a linked list of queue_nodes.
 */
typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

struct queue {
    /* queue_node pointers to the head and tail of the queue */
    queue_node *head, *tail;

    /* The number of elements in the queue */
    ssize_t size;

    /**
     * The maximum number of elements the queue can hold.
     * max_size is non-positive if the queue does not have a max size.
     */
    ssize_t max_size;

    /* Mutex and Condition Variable for thread-safety */
    pthread_cond_t cv;
    pthread_mutex_t m;
};

ssize_t queue_size(queue* this) {
	ssize_t size;
	pthread_mutex_lock(&this->m);
	size = this->size;
	pthread_mutex_unlock(&this->m);
	return size;
}

queue *queue_create(ssize_t max_size) {
	queue *result = malloc(sizeof(queue));
	result->size = 0;
	result->max_size = max_size;
	result->head = NULL;
	result->tail = NULL;
	pthread_mutex_init(&result->m, NULL);
	pthread_cond_init(&result->cv, NULL);
	return result;
}

void queue_destroy(queue *this) {
	if(this->head != NULL){
		queue_node *node, *del;
		node = this->head->next;
		while(node != this->head){
			del = node;
			node = node->next;
			//free(del->data);
			free(del);
		}
		//free(this->head->data);
		free(this->head);
	}
	free(this);
}

void queue_push(queue *this, void *data) {
	pthread_mutex_lock(&this->m);
	while(this->max_size > 0 && this->size >= this->max_size){
		pthread_cond_wait(&this->cv, &this->m);
		if(this->max_size > 0 && this->size < this->max_size){
			pthread_cond_signal(&this->cv);
		}
	}
	if(this->tail == NULL && this->head == NULL){
		this->tail = malloc(sizeof(queue_node));
		this->tail->next = this->tail;
		this->head = this->tail;
	}else{
		queue_node *new = malloc(sizeof(queue_node));
		new->next = this->tail->next;
		this->tail->next = new;
		this->tail = this->tail->next;
	}
	this->tail->data = data;
	this->size++;
	if(this->size == 1){
		pthread_cond_signal(&this->cv);
	}
	pthread_mutex_unlock(&this->m);
}

void *queue_pull(queue *this) {
	pthread_mutex_lock(&this->m);
	while(this->size < 1){
		pthread_cond_wait(&this->cv, &this->m);
		if(this->size >= 1){
			pthread_cond_signal(&this->cv);
		}
	}
	void *result = this->head->data;
	queue_node *del = this->head;
	if(this->head->next == this->head){//last one
		this->head = NULL;
		this->tail = this->head;
	}else{
		this->head = this->head->next;
		this->tail->next = this->head;
	}
	free(del);
	this->size--;
	if(this->max_size > 0 && this->size == this->max_size - 1){
		pthread_cond_signal(&this->cv);
	}
	pthread_mutex_unlock(&this->m);
	return result;
}
