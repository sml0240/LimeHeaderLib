#include "include/list.h"
#include "include/hashmap.h"
#include <assert.h>
//#include "include/common_macros.h"

typedef struct Test
{
    int x;
    int y;

} Test;


void final_print(List* list)
{
    for (size_t i = 0; i < list->capacity; i++)
    {
        OptionalPtr opt;
        opt = list_get(list, i);

        if (opt.has_value)
        {
            Test test = *(Test*) opt.value;
            printf("x: %d, y: %d\n", test.x, test.y);
        }
    }
}

void opt_print(OptionalPtr *opt)
{
    if (opt->has_value)
    {
        Test test = *(Test*) opt->value;
        printf("optprint:  x: %d, y: %d\n", test.x, test.y);
    }
}

int main()
{
    size_t s = 8;

    List* list = list_create(5, sizeof(Test));
    for (size_t i = 0; i<s; i++)
    {
        printf("iter append: %d\n", i);
        Test v = {i, i};
        list_append(list, &v);
    }
    final_print(list);

    Test v2 = {50, 50};
    list_insert(list, &v2, 5);
    list_remove(list, 2);
    printf("\n after insert \n");
    OptionalPtr opt = list_get(list, 5);
    opt_print(&opt);
    final_print(list);
    list_destroy(list);
    return 0;
}



// int main()
// {
//     // ---- HASHMAP -----
//     HashMap* hmap = hashmap_create();
//     size_t amt = 5;
//     char keys[amt][8];

//     for (int i = 0; i <= amt; i++)
//     {
//         Test test;

//         test.x = i;
//         test.y = i;
//         printf("test.x %d, test.y %d", test.x, test.y);
//         char temp[8] = "entry";
//         //int n = sprintf(temp, "entry", i);
//         char* buffer = temp;
//         char num[1];
//         sprintf(buffer, "entry%d", i);
//         //strcat(buffer, num);
//         printf("num: %s\n", num);
//         strcpy(keys[i], buffer);
//         //printf("buffer after sprintf: [%s] size: %d \n", buffer, n);
//         //keys[i] = temp;
//         hashmap_add(hmap, buffer, &test);
        
//         printf( "temp %s\n, keys[%d] = %s\n", temp, i, keys[i]);
//         //assert(buffer == keys[i]);
//     }
//     for (int i = 0; i <= amt; i++)
//     {
//         printf(" key %d in key is: [%s] ", i, keys[i]);
//     }


//     int ks = (sizeof(keys) / sizeof(keys[0]));
//     printf("keys size: %d\n", ks);
//     for (int i = 0; i < ks; i++)
//     {
//         char key[8];
//         // = keys[i];
//         strcpy(key, keys[i]);
//         printf(" key from keys : %s\n", key);
//         OptionalPtr opt;
//         opt = hashmap_get(hmap, key);

//         if (opt.has_value)
//         {
//             Test ret = *(Test*)opt.value;
//             printf("ret x:%d y:%d\n", ret.x, ret.y);
//         }
//         else
//         {
//             printf(" no ret value \n");
//         }
//     }
//     OptionalPtr opt1;
//     // OptionalPtr opt2;
//     opt1 = hashmap_get(hmap, "entry0");
//     // opt2 = hashmap_get(hmap, k2);


//     if (opt1.has_value)
//     {
//         Test ret = *(Test*)opt1.value;
//         printf("ret x:%d y:%d", ret.x, ret.y);
//     }

//     // if (opt2.has_value)
//     // {
//     //     Test ret2 = *(Test*)opt2.value;
//     //     printf("ret x:%d y:%d", ret2.x, ret2.y);
//     // }
//     free(hmap);
//     return 0;
// }
    // ------ LIST -------
    // size_t s = 8;

    // List* list = list_create(5, sizeof(Test));
    // for (size_t i = 0; i<s; i++)
    // {
    //     printf("iter append: %d\n", i);
    //     Test v = {i, i};
    //     list_append(list, &v);
    // }

    // printf("\nlist item count: %d\n", list_count(list));

    //final_print(list);
    // list_remove(list, 2);
    // //final_print(list);
    // Test v2 = {100, 100};
    // list_append(list, &v2);
    // final_print(list);
    // OptionalPtr opt = list_pop_back(list);
    // opt_print(&opt);
    // OptionalPtr o = list_pop_front(list);
    // final_print(list);
    // list_destroy(list);
    //hashmap_destroy(hmap);

//}