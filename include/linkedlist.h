#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "variant.h"
#include "optional.h"

typedef struct Link Link;

typedef struct VariantLinkedList VariantLinkedList;


typedef struct Link
{
    Variant* value;
    Link* next;
}Link;


typedef struct LinkedList
{
    Link* head;
    size_t size;
}LinkedList;


LinkedList* linkedlist_create()
{
    LinkedList* list = malloc(sizeof(LinkedList));
    if (list == NULL)
    {
        fprintf(stderr, "IntLinkedlist Initialization falied");
        return NULL;
    }

    list->head = NULL;
    list->size = 0;
    return list;
}
bool is_index_valid(LinkedList* list, size_t index)
{
    return !((list->size - 1 < index) || (index < 0));

}

size_t linkedlist_size(LinkedList* list)
{
    return list->size;
}

int linkedlist_append(LinkedList* list, void* item)
{
    Link* new_link = malloc(sizeof(Link));
    int ins_position = 0;

    if (new_link == NULL)
    {
        //fprintf(stderr, "VariantLinkedlist Initialization falied");
        return -1;
    }

    new_link->value = item;
    new_link->next = NULL;

    if (list->head == NULL)
    {
        list->head = new_link;
        list->size++;
        return ins_position;
    }

    Link* last_link = list->head;

    while (last_link->next != NULL)
    {
        ins_position++;
        last_link = last_link->next;
    }

    last_link->next = new_link;
    list->size++;
    printf("here at variantlinkedlist append end\n");

    return ins_position;
}


OptionalPtr linkedlist_get(LinkedList* list, size_t index)
{
    OptionalPtr opt;

    // if (list->size - 1 < index)
    // {
    //     //opt.value = 0;
    //     opt.has_value = false;
    //     return opt;
    // }

    if (!is_index_valid(list, index))
    {

        opt.has_value = false;
        return opt;
    }
    
    Link* link = list->head;

    for (size_t i = 0; i < index; i++)
        link = link->next;

    opt.value = link->value;
    opt.has_value = true;
    return opt;
}

Link* __get_link(LinkedList* list, size_t index)
{
    //OptionalPtr opt;

    // if (list->size - 1 < index)
    // {
    //     //opt.value = 0;
    //     opt.has_value = false;
    //     return opt;
    // }

    // if (!is_index_valid(list, index))
    // {
    //     opt.has_value = false;
    //     return opt;
    // }
    
    Link* link = list->head;

    for (size_t i = 0; i < index; i++)
        link = link->next;

    // opt.value = link;
    // opt.has_value = true;
    return link;
}



int linkedlist_delete(LinkedList* list, size_t index)
{
    if (!is_index_valid(list, index))
    {
        return -1;
    }
    Link* current_link = __get_link(list, index);

    bool has_prev = false;
    bool has_next = false;

    if (index > 0)
        has_prev = true;

    if (is_index_valid(list, index+1))
        has_next = true;

    // is only item
    if (!has_prev && !has_next)
    {
        printf("NOT prev and NOT next\n");
        list->head = NULL;
        free(current_link);
        list->size--;
        return 0;
    }

    Link* prev;
    Link* next;

    if (has_prev)
        prev = __get_link(list, index-1);

    if (has_next)
    {
        next = current_link->next;
        if (has_prev)
        {
            printf("both prev and next\n");
            prev->next = next;
        }
        else
        {
            printf("only next\n");
            list->head = next;
        }
    }

    free(current_link);
    list->size--;
    return 0;
}



void linkedlist_destroy(LinkedList* list)
{
    Link* current_link = list->head;
    Link* next_link;

    for (size_t i = 0; i < list->size; i++)
    {
        next_link = current_link->next;
        free(current_link);
        current_link = next_link;
    }

    free(list);
}

void linkedlist_print(LinkedList* list)
{
    if (list->head == NULL)
    {
        printf("No values");
        return;
    }

    Link* link = list->head;

    for (size_t i = 0; i < list->size; i++)
    {
        printf("%d\n", link->value);
        link = link->next;
    }
}
