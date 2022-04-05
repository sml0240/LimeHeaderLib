#pragma once
#include "optional.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// ------- GENERIC DYNAMIC LIST -------

/*
    -- Listan körs bara på optionalPtr( vill renama till Option)

    ÄNDRINGAR:
    - logiken för resize är utlagd till egna funktioner
    - insert() (insert_at) implementerad, insert på specificerad index
    - remove() (remove at) implementerad, ta bort specificerad index
    - pop_front() implementerad (index 0)
    - pop back() implementerad (index -1)
    - list_count() implementerad, där alla element räknas iterativt
        list_size() finns fortfarande för size check, går på list_index

    CHECKS:
    - valid_index() funktionen ska köras först när ett index är med
    - resize_check() är bra att köra på alla inserts/appends \
        gör först en resize_check, är den true så kör list_resize
*/

typedef struct List
{
    uint8_t* list_memory;
    size_t list_index;
    size_t capacity;
    size_t element_size;
    char empty;
}List;

// func defs
List* list_create(size_t size, size_t element_size);
int list_append(List* list, void* element);
bool list_insert(List* list, void* element, size_t index);
OptionalPtr list_get(List* list, size_t index);
OptionalPtr list_get_back(List* list);
size_t capacity(List* list);
bool list_remove(List* list, size_t index);
size_t list_resize(List* list);
void list_destroy(List* list);
int list_count(List* list);
bool resize_check(List* list);
bool valid_index(List* list, size_t index);
float get_multiplyer(size_t size);



float get_multiplyer(size_t size)
{
        if (size <= 10)
            return 2.0f;
        if (size <= 100)
            return 1.9f;
        if (size <= 1000)
            return 1.8f;
        if (size <= 10000)
            return 1.7f;
        if (size <= 100000)
            return 1.6f;
        return 1.5f;
}

// creates list
List* list_create(size_t size, size_t element_size)
{
    List* list;
    list = malloc(sizeof(List));
    if (list == NULL)
    {
        fprintf(stderr, "List init failed\n");
        return NULL;
    }
    list->capacity = size;
    list->element_size = element_size;
    list->list_memory = malloc(size * element_size);
    if (list->list_memory == NULL)
    {
        fprintf(stderr, "List init failed\n");
        return NULL;
    }
    list->list_index = 0;
    list->empty = 0;
    return list;
}

// fråga: varför returnar du reallocating?
// gjort om så list_resize() används internt ist för hårdkåådat
int list_append(List* list, void* element)
{   
    int reallocating = 0;

    if (resize_check(list))
    {
        printf("\nreallocating\n");
        reallocating = 1;
        if (!list_resize(list))
        {
            printf("failed to reallocate memory");
            return -1;
        }
    }

    memmove(list->list_memory + (list->list_index * list->element_size),
    element, list->element_size);

    list->list_index++;
    list->empty = 0;
    printf("element count: %d\n", list_count(list));
    return reallocating;
}

bool list_insert(List* list, void* element, size_t index)
{
    if (!valid_index(list, index))
    {
        return false;
    }
    if (resize_check(list))
    {
        printf("\nreallocating\n");

        if (!list_resize(list))
        {
            printf("failed to reallocate memory");
            return false;
        }
    }

    size_t c = list_count(list);

    // -----------------------------------------------------------------------------------------
    // -----------------------------   Version 1
    // -----------------------------------------------------------------------------------------
    //uint8_t* new_mem = malloc((list->capacity) * list->element_size);
    // // insert new item first
    // memmove(new_mem + (index * list->element_size), element, list->element_size);


    // replace all items in new memory
    // for (int i = 0; i <c; i++)
    // {
    //     size_t get_addr, set_addr;

    //     // if (i == index)
    //     // {
    //     //     get_addr = (i) * list->element_size;
    //     //     set_addr = (i+1) * list->element_size;
    //     // }
    //     if (i < index)
    //     {
    //         get_addr = i * list->element_size;
    //         set_addr = i * list->element_size;
    //     }
    //     else
    //     {
    //         break;
    //     }

    //     memmove(new_mem + set_addr, list->list_memory + get_addr, list->element_size);
    // }
    // free(list->list_memory);
    // list->list_memory = new_mem;

    // ---------------------------------------------------------------------------------------------
    // ----------------------------------- Version 2 -------
    // ---------------------------------------------------------------------------------------------
    
    // moves before index
    if (index > 0)
    memmove(list->list_memory, list->list_memory, (index-1) * list->element_size);
    
    // moves whole block after insert index
    if (index < list->list_index)
        memmove(list->list_memory + (index * list->element_size), list->list_memory + ((index-1) * list->element_size), (c - index + 1) * list->element_size);

    // insert new item last
    memmove(list->list_memory + (index * list->element_size), element, list->element_size);
    list->list_index++;
}

// tanken är att man ska hämta både new downsize och upsize här för att slippa
// skriva logik överallt för detta
// bara upsize impl än
size_t get_extended_size(size_t old_size, bool is_extending)
{
    if (is_extending)
    {
        return (size_t)(old_size * get_multiplyer(old_size));
    }
}

// alloc with new_size, new_size needs to be in item amount ( not memory )
// NOTE: 
// might have to change to malloc or calloc, did get undefined behvr once
// with realloc
size_t list_resize(List* list)
{
    printf("\nreallocating\n");

    size_t new_size = get_extended_size(list->capacity, true);
    uint8_t* temp = realloc(list->list_memory, new_size * list->element_size);
    // take pointer and set it to list_memory in case realloc moved memory
    list->list_memory = temp;

    list->list_index  = list_count(list);
    list->capacity = new_size;
    if (list->list_memory == NULL)
    {
        printf("list resize failed \n");
        return -1;
    }
    printf("\n--- list is resized to %d elements --- \n\n", list->capacity);
    return 1;
}


// get last item, removes it from list
OptionalPtr list_pop_back(List* list)
{
    OptionalPtr opt;
    size_t last_idx = list->list_index - 1;
    
    if (!valid_index(list, last_idx))
    {
        printf("index %d is invalid", last_idx);
        return opt;
    }

    opt = list_get(list, last_idx);
    
    if (!opt.has_value)
    {        
        return opt;
    }

    list_remove(list, last_idx);
    return opt;
}

// get first item, removes it from list
OptionalPtr list_pop_front(List* list)
{
    OptionalPtr opt;
    size_t first_idx = 0;
    
    if (!valid_index(list, first_idx))
    {
        printf("index %d is invalid", first_idx);
        return opt;
    }

    opt = list_get(list, first_idx);
    
    if (!opt.has_value)
    {
        printf("optional pointer is empty");
        return opt;
    }

    list_remove(list, first_idx);
    return opt;
}

// remove an item at index
bool list_remove(List* list, size_t index)
{
    if (!valid_index(list, index))
    {
        return false;
    }
    size_t c = list_count(list);

// ------------------------------------------------------------------------------------
//-------------------------------- Option 1:::::
// ------------------------------------------------------------------------------------
//    uint8_t* new_mem = malloc((list->capacity) * list->element_size);

    // for (int i = 0; i <c; i++)
    // {
    //     size_t get_addr, set_addr;

    //     if (i >= index)
    //     {
    //         get_addr = (i+1) * list->element_size;
    //         set_addr = i * list->element_size;
    //     }
    //     else
    //     {
    //         get_addr = i * list->element_size;
    //         set_addr = i * list->element_size;
    //     }

    //memmove(list->list_memory + set_addr, list->list_memory + get_addr, list->element_size);
    //}

    // ------------------------------------------------------------------------------------
    //-------------------------------- Option 2:::::
    // ------------------------------------------------------------------------------------
        // either this one, memmove the whole element_size block
        //moves before index
        if (index > 0)
            memmove(list->list_memory, list->list_memory, (index - 1) * list->element_size);
        // moves after index
        if (index < list->list_index-1)
            memmove(list->list_memory + (index * list->element_size),
                (list->list_memory + ((index+1) * list->element_size)),
                ((list->list_index-1) - index) * list->element_size);

    // ------------------------------------------------------------------------------------
    //-------------------------------- Option 3:::::
    // ------------------------------------------------------------------------------------

        // loop through mem_start - element_size ( for ele size 8 loop 0-8)
        // for (size_t mc = 0; mc <= 7; mc++)
        // {
        //     new_mem[set_addr + mc] = list->list_memory[get_addr + mc];
        // }
    // ------------------------------------------------------------------------------------    
    //}

    //free(list->list_memory);    
    //list->list_memory = new_mem;

    list->list_index--;
    return true;
}

// prints set values or all if (all)
void list_print(List* list, bool all)
{
    for (int i = 0; i<list->capacity; i++)
    {
        OptionalPtr opt = list_get(list, i);
        if (!all)
        {
            if (!opt.has_value)
            {
                break;
            }
        }
        printf("has_value: %d, opt value: %d\n", opt.has_value, opt.value);
    }
}

// get item
OptionalPtr list_get(List* list, size_t index)
{
    OptionalPtr opt;
    if (index < 0 || index >= list->list_index || list->empty)
    {
        opt.error = 1;
        opt.has_value = false;
        return opt;
    }

    uint8_t* temp_ptr = list->list_memory + (index * list->element_size);
    opt.value = (void*)temp_ptr;
    opt.has_value = true;
    return opt;
}

// is requested index valid
bool valid_index(List* list, size_t index)
{
    return !(index < 0 || index >= list->list_index || list->empty);
}

// true if resize needed
bool resize_check(List* list)
{
    return (list->list_index == list->capacity);
}

// allocated slots
size_t list_capacity(List* list)
{
    return list->capacity;
}

// list size by list slots occupied
size_t list_size(List* list)
{
    return list->list_index;
}

// is empty
size_t list_is_empty(List* list)
{
    return list->list_index == 0;
}

// checks values set instead of list->capacity
int list_count(List* list)
{
    for (int i = 0; i< list->capacity; i++)
    {
        OptionalPtr opt = list_get(list, i);
        if (!opt.has_value)
        {
            return i;
        }
    }
}

void list_destroy(List* list)
{
    free(list->list_memory);
    free(list);
}