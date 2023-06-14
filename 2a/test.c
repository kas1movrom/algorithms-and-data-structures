#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node *next;
} Node;

typedef struct stack_list {
	Node *head;
} stack_list;

typedef struct stack_vector {
	int top;
	int size;
	int *arr;
} stack_vector;

typedef struct queue_list {
	Node *tail;
	Node *head;
}queue_list;

typedef struct queue_vector {
	int size;
	int count;
	int head;
	int tail;
	int *arr;
} queue_vector;

int push_stack_vec(stack_vector *stack, int data);
int push_stack_list(stack_list *stack, int data);
int push_queue_list(queue_list *queue, int data);
int push_queue_vec(queue_vector *queue, int data);

int pop_stack_vec(stack_vector *stack, int *data);
int pop_stack_list(stack_list *stack, int *data);
int pop_queue_list(queue_list *queue, int *data);
int pop_queue_vec(queue_vector *queue, int *data);

int push_stack_vec(stack_vector *stack, int data)
{
	if (stack->top == stack->size || stack->arr == NULL)
		return 1;

	stack->arr[stack->top] = data;
	stack->top = stack->top + 1;
	return 0;
}

int push_stack_list(stack_list *stack, int data)
{
	Node *element = (Node*)malloc(sizeof(Node));
	element->next = stack->head;
	element->data = data;
	stack->head = element;
	return 0;
}

int pop_stack_vec(stack_vector *stack, int *data)
{
	if (stack->top == 0 || stack->arr == NULL)
		return 1;

	*data = stack->arr[stack->top - 1];
	stack->top = stack->top - 1;
	return 0;
}

int pop_stack_list(stack_list *stack, int *data)
{
	if (stack->head == NULL)
		return 1;

	Node *ptr = stack->head;
	stack->head = ptr->next;
	*data = ptr->data;
	free(ptr);
	return 0;
}

int push_queue_list(queue_list *queue, int data)
{
	Node *element = (Node*)malloc(sizeof(Node));
	element->next = NULL;
	element->data = data;
	if (queue->head == NULL) {
		queue->head = element;
		queue->tail = element;
	} else {
		queue->tail->next = element;
		queue->tail = element;
	}
	return 0;	
}

int push_queue_vec(queue_vector *queue, int data)
{
	 if (queue->count == queue->size)
	 	return 1;

	 queue->arr[queue->tail] = data;
	 queue->tail = (queue->tail + 1) % queue->size;
	 queue->count = queue->count + 1;
	 return 0;
}

int pop_queue_vec(queue_vector *queue, int *data)
{
	if (queue->count == 0)
		return 1;

	*data = queue->arr[queue->head];
	queue->head = (queue->head + 1) % queue->size;
	queue->count = queue->count - 1;
	return 0;
}

int pop_queue_list(queue_list *queue, int *data)
{
	if (queue->head == NULL)
		return 1;

	Node *ptr = queue->head;
	*data = ptr->data;
	queue->head = ptr->next;
	free(ptr);
	return 0;
}
