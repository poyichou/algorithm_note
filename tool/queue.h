#pragma once
#include <stdbool.h>
#include <stdlib.h>

typedef struct queue queue;

queue *queue_create(ssize_t max_size);

ssize_t queue_size(queue *this);

void queue_destroy(queue *this);

void queue_push(queue *this, void *element);

void *queue_pull(queue *this);
