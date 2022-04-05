#pragma once
#include "optional.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum QueueError {GetItemError, IndexError, EmptyQueueError}QueueError;

typedef struct Queue
{
    uint8_t* memory;
    int front, back;
    size_t size;
    size_t capacity;
    size_t element_size;
}Queue;

bool _sort(Queue* queue, int elements_gone);

Queue* queue_create(size_t capacity, size_t element_size)
{
    printf("beginning of quueeu\n");
    Queue* queue;
    queue = malloc(sizeof(Queue));
    if (queue == NULL)
    {
        fprintf(stderr, "Queue init failed\n");
        return NULL;
    }
    queue->capacity = capacity;
    queue->size = 0;
    queue->element_size = element_size;

    queue->front = queue->size = 0;
    queue->back = capacity - 1;

    queue->memory = calloc(queue->capacity, queue->element_size);
    
    if (queue->memory == NULL)
    {
        fprintf(stderr, "Queue init failed\n");
        return NULL;
    }
    return queue;
}

bool queue_is_full(Queue* queue)
{
    return (queue->size == queue->capacity);
}   
bool queue_is_empty(Queue* queue)
{
    return (queue->size == 0);
}

int queue_size(Queue* queue)
{
    return queue->size;
}

void queue_print(Queue* queue)
{
   
    for (int i = 0; i < queue->capacity; i++)
    {
        printf("queue print iter: %d\n", i);
    }
}

OptionalPtr queue_pop(Queue* queue)
{
    OptionalPtr opt;

    if (queue_is_empty(queue))
    {
        opt.has_value = false;
        return opt;
    }

    uint8_t* item = (queue->memory + (queue->front * queue->element_size));

    queue->front = (queue->front + 1) % queue->capacity;

    opt.value = (void*) item;
    opt.has_value = true;

    queue->size--;

    return opt;

}

int queue_append(Queue* queue, void* element)
{
    if (queue_is_full(queue))
    {
        printf("queue is full!\n");
        return 0;
    }
    queue->back = (queue->back + 1) % queue->capacity;
    memcpy(queue->memory + (queue->back * queue->element_size),
    element, queue->element_size);
    
    queue->size++;

    printf("element count: %d\n", queue_size(queue));
    return 0;
}