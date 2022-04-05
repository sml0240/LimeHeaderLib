//#include "../limelib.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct IntLink IntLink;

struct IntLink
{
    int value;
    IntLink* next;
};

struct IntLinkedlist
{
    IntLink* head;
    size_t size;
};

IntLinkedlist* int_linkedlist_create()
{
    IntLinkedlist* list = malloc(sizeof(IntLinkedlist));
    if (list == NULL)
    {
        fprintf(stderr, "IntLinkedlist Initialization falied");
        return NULL;
    }

    list->head = NULL;
    list->size = 0;
    return list;
}

size_t int_linkedlist_size(IntLinkedlist* list)
{
    return list->size;
}

int int_linkedlist_append(IntLinkedlist* list, int num)
{
    IntLink* new_link = malloc(sizeof(IntLink));

    if (new_link == NULL)
    {
        fprintf(stderr, "IntLinkedlist Initialization falied");
        return -1;
    }

    new_link->value = num;
    new_link->next = NULL;

    if (list->head == NULL)
    {
        list->head = new_link;
        list->size++;
        return 0;
    }

    IntLink* last_link = list->head;

    while (last_link->next != NULL)
        last_link = last_link->next;

    last_link->next = new_link;
    list->size++;

    return 1;
}

OptionalInt int_linkedlist_get(IntLinkedlist* list, size_t index)
{
    OptionalInt opt;

    if (list->size - 1 < index)
    {
        opt.value = 0;
        opt.has_value = false;
        return opt;
    }

    IntLink* link = list->head;

    for (size_t i = 0; i < index; i++)
        link = link->next;

    opt.value = link->value;
    opt.has_value = true;
    return opt;
}

int int_linkedlist_delete(IntLinkedlist* list, size_t index)
{
    if (list->size - 1 < index)
        return -1;

    IntLink* link = list->head;
    
    for (size_t i = 0; i < index - 1; i++)
        link = link->next;

    IntLink* next_link = link->next->next;
    free(link->next);
    link->next = next_link;
    list->size--;
    return 0;
}

void int_linkedlist_destroy(IntLinkedlist* list)
{
    IntLink* current_link = list->head;
    IntLink* next_link;

    for (size_t i = 0; i < list->size; i++)
    {
        next_link = current_link->next;
        free(current_link);
        current_link = next_link;
    }

    free(list);
}

void int_linkedlist_print(IntLinkedlist* list)
{
    if (list->head == NULL)
    {
        printf("No values");
        return;
    }

    IntLink* link = list->head;

    for (size_t i = 0; i < list->size; i++)
    {
        printf("%d\n", link->value);
        link = link->next;
    }
}
