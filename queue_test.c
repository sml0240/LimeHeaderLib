#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "include/queue.h"
#include "include/variant.h"
#include "include/optional.h"
#include "include/variant_linkedlist.h"


int main()
{
    float varval1 = 10;
    int varval2 = 20;
    Variant* var1 = variant_create(&varval1, i8);
    Variant* var2 = variant_create(&varval2, Integer);

    bool valid =  variant_can_cast(var1, Integer);
    bool valid2 =  variant_can_cast(var2, i8);

    printf(" variant1 can cast = %d\n", valid);
    printf(" variant2 can cast = %d\n", valid2);

    int8_t nv = 111;
    variant_set_value(var1, &nv, i32);

    VariantLinkedList* list = variant_linkedlist_create();

    variant_linkedlist_append(list, var1);
    variant_linkedlist_append(list, var2);

    printf("var 1: \n");
    variant_print(var1);

    OptionalPtr opt1 = variant_linkedlist_get(list, 0);
    if (opt1.has_value)
    {
        Variant* retvar1 = (Variant*) opt1.value;
        if (variant_can_cast(retvar1, Integer))
        {
            printf("variant1 return: %d\n", variant_get_as_i32(retvar1));
            variant_print(var1);
        }
    }

    OptionalPtr opt2 = variant_linkedlist_get(list, 1);
    if (opt2.has_value)
    {
        Variant* retvar2 = (Variant*) opt2.value;
        if (variant_can_cast(retvar2, Float))
        {
            float fvar = variant_get_as_float(retvar2);

            printf("variant2 should be: %f\n",(float) varval2);
            printf("variant2 return: %f\n", &fvar);
        }
    }

    variant_linkedlist_destroy(list);

    return 0;
}

// int main()
// {
//     char* var1val = "var1val";

//     Variant* var1 = variant_create(10, INTEGER);
//     Variant* var2 = variant_create(20, INTEGER);
//     Variant* var3 = variant_create(30, INTEGER);
//     Queue* queue = queue_create(10, sizeof(Variant));

//     queue_append(queue, &var1);
//     queue_append(queue, &var2);
//     queue_append(queue, &var3);
    
//     //queue_print_mem(queue);
//     //queue_print_mem(queue);
//     //queue_print_mem(queue);
//     //Result res2 = queue_get_next(queue);
    
//     Result res = queue_pop(queue);
//     if (res.result == OK)
//     {
//         Variant* item = (Variant*) res.value;
//         variant_print(item);
//         int varstr = variant_get_as_int(item);
//         printf("variant1 int: %d\n", varstr);
//     }
//     else
//     {
//         printf("result is error : %d\n", res.error);
//     }

//     Result res2 = queue_pop(queue);
//     if (res2.result == OK)
//     {
//         Variant* item2 = (Variant*) res2.value;
//         variant_print(item2);
//         int varstr2 = variant_get_as_int(item2);
//         printf("variant2 int: %d\n", varstr2);
//     }
//     else
//     {
//         printf("result is error : %d\n", res2.error);
//     }
//     Result res3 = queue_pop(queue);
//     if (res3.result == OK)
//     {
//         Variant* item3 = (Variant*) res3.value;
//         variant_print(item3);
//         int varstr3 = variant_get_as_int(item3);
//         printf("variant3 int: %d\n", varstr3);
//     }
//     else
//     {
//         printf("result is error : %d\n", res3.error);
//     }

// //    Variant item = *(Variant*) p_item;

    
//     // if (&item == NULL)
//     // {
//     //     printf("item is null!");
//     // }

//     variant_destroy(var1);
//     variant_destroy(var2);
//     variant_destroy(var3);
//     printf("queue size: %d\n", queue_size(queue));
//     free(queue);
//     return 0;

// }