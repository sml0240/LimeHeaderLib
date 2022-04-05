#include <stdio.h>
#include <stdlib.h>
#include "variant.h"
#include "optional.h"

typedef struct Link Link;

typedef struct VariantLinkedList VariantLinkedList;


struct Link
{
    Variant* value;
    Link* next;
};


struct VariantLinkedList
{
    Link* head;
    size_t size;

};


VariantLinkedList* variant_linkedlist_create()
{
    VariantLinkedList* list = malloc(sizeof(VariantLinkedList));
    if (list == NULL)
    {
        fprintf(stderr, "IntLinkedlist Initialization falied");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

size_t variant_linkedlist_size(VariantLinkedList* list)
{
    return list->size;
}

int variant_linkedlist_append(VariantLinkedList* list, Variant* item)
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


OptionalPtr variant_linkedlist_get(VariantLinkedList* list, size_t index)
{
    OptionalPtr opt; 

    if (list->size - 1 < index)
    {
        //opt.value = 0;
        opt.has_value = false;
        return opt;
    }

    Link* link = list->head;

    for (size_t i = 0; i < index; i++)
        link = link->next;
    
    //printf("here at variantlinkedlist get\n");

    opt.value = link->value;
    opt.has_value = true;
    return opt;
}

int variant_linkedlist_delete(VariantLinkedList* list, size_t index)
{
    if (list->size - 1 < index)
        return -1;

    Link* link = list->head;
    
    for (size_t i = 0; i < index - 1; i++)
        link = link->next;

    Link* next_link = link->next->next;
    free(link->next);
    link->next = next_link;
    list->size--;
    return 0;
}

void variant_linkedlist_destroy(VariantLinkedList* list)
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

void variant_linkedlist_print(VariantLinkedList* list)
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