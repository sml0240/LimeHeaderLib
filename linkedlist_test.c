#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
//#include "include/variant.h"
#include "include/linkedlist.h"
#include "include/optional.h"


typedef struct Test
{
    int id;
    char* name;
}Test;


int main()
{
    LinkedList* list = linkedlist_create();

    Test v1 = { .id = 1, .name = "v1" };
    Test v2 = { .id = 2, .name = "v2" };
    Test v3 = { .id = 3, .name = "v3" };
    Test v4 = { .id = 4, .name = "v4" };

    linkedlist_append(list, &v1);
    linkedlist_append(list, &v2);
    linkedlist_append(list, &v3);
    linkedlist_append(list, &v4);

    linkedlist_print(list);
    OptionalPtr opt1 = linkedlist_get(list, 1);
    if (opt1.has_value)
    {
        Test retv1 = *(Test*) opt1.value;
        printf(" retval id: %d, name: %s\n", retv1.id, retv1.name);
        int d_success = linkedlist_delete(list, 4);
        printf("delete success: %d\n", d_success);
    }
    linkedlist_print(list);
    //linkedlist_destroy(list);

    return 0;
}