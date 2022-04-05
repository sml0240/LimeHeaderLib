#include "include/list.h"
#include "include/hashmap.h"
#include "include/variant.h"


void print_variant(Variant* variant)
{
    printf("variant type: %d, variant value: %d\n", variant->type, variant->value);
}

int main()
{
    // ---- HASHMAP -----
    HashMap* hmap = hashmap_create();
    
    // size_t amt = 5;
    // //char keys[amt];

    // for (int i = 0; i <= amt; i++)
    // {
    //     int var_val = i * 10;
    //     Variant* var = variant_create(&var_val, INTEGER);
    //     printf("variant set value %d\n", var_val);
    //     //printf("test.x %d, test.y %d", test.x, test.y);
    //     char temp[8] = "entry";
        
    //     int n = sprintf(temp, "entry%d", i);
    //     char* buffer = temp;
    //     //char num[1];
    //     //sprintf(buffer, "entry%d", i);
    //     //strcat(buffer, num);
    //     //printf("num: %s\n", num);
    //     //strcpy(keys[i], buffer);
    //     //printf("buffer after sprintf: [%s] size: %d \n", buffer, n);
    //     //keys[i] = temp;

    //     hashmap_add(hmap, buffer, var);
    //     OptionalPtr opt = hashmap_get(hmap, buffer);
    //     printf("buffer is: %s\n", buffer);

    //     if (opt.has_value)
    //     {
    //         Variant variant = *(Variant*) opt.value;
    //         print_variant(&variant);
    //         if (variant_can_unwrap(&variant, INTEGER))
    //         {
    //             int variant_value = (int*) variant_get_value(&variant);
    //             printf("variant unwrap value: %d\n", variant_value);
    //         }
    //     }

    //     //printf( "temp %s\n, keys[%d] = %s\n", temp, i, keys[i]);
    //     //assert(buffer == keys[i]);
    // }
    

    float varval1 = 10.532;
    int varval2 = 20;
    char* varval3 = "hallelulja";
    char* varval4 = "e4 value string";
    
    Variant* var1 = variant_create(&varval1, FLOAT);
    Variant* var2 = variant_create(&varval2, INTEGER);
    Variant* var3 = variant_create(&varval3, STRING);
    
    // Variant* var4 = variant_create(&varval4, STRING);
    //uint8_t* var_as_byte = variant_as_bytes(var1);
    char* var_as_str = variant_as_str(var1);
    
    char* key1 = "entry1";
    char* key2 = "entry2";
    char* key3 = "entry3";

    // char* key4 = "entry4";
    //char* keys[5] = {"entry1", "entry2", "entry3", "entry4", "entry5"};

    hashmap_add(hmap, key1, var1);
    hashmap_add(hmap, key2, var2);
    hashmap_add(hmap, var_as_str, var3);
    hashmap_add(hmap, key3, varval3);

    // hashmap_add(hmap, key3, var3);
    //hashmap_remove(hmap, "entry2");
    uint8_t* keys = hashmap_keys(hmap);
    int karr_size = sizeof(keys) / sizeof(keys[0]);
    printf("karr size is: %d\n", karr_size);
    int ik = 0;
    for (ik; ik < karr_size*8; ik+=8)
    {
        char str[8];
        strcpy(str, (char*) keys+ik);
        printf("key: %s\n", str);
    }
    free(keys);

    OptionalPtr opt1 = hashmap_get(hmap, "entry5");
    
    // OptionalPtr opt2 = hashmap_get(hmap, "entry6");
    // OptionalPtr opt3 = hashmap_get(hmap, var_as_str);
    // OptionalPtr opt4 = hashmap_get(hmap, "entry7");
    // // OptionalPtr opt3 = hashmap_get(hmap, "entry3");

    // if (opt1.has_value)
    // {
    //     Variant var1 = *(Variant*) opt1.value;
    //     print_variant(&var1);
    //     if (variant_can_unwrap(&var1, FLOAT))
    //     {
    //         float var1uv = variant_get_as_float(&var1);
    //         printf("var 1 uv value: %f\n", var1uv);
    //     }
    // }
    // else
    // {
    //     printf("\nopt 1 has no value!!\n");
    // }

    // if (opt2.has_value)
    // {
    //     Variant var2 = *(Variant*) opt2.value;
    //     print_variant(&var2);
    //     if (variant_can_unwrap(&var2, INTEGER))
    //     {
    //         int var2uv = variant_get_as_int(&var2);
    //         printf("var 2 uv value: %d\n", var2uv);
    //     }
    // }
    // else
    // {
    //     printf("\nopt 2 has no value!!\n");
    // }


    // if (opt3.has_value)
    // {
    //     Variant var3 = *(Variant*) opt3.value;
    //     if (variant_can_unwrap(&var3, STRING))
    //     {
    //         char* var3uv = variant_get_as_str(&var3);
    //         printf("var 3 uv value: %s\n", var3uv);
    //     }
    // }
    // else
    // {
    //     printf("\nopt 3 has no value!!\n");
    // }

    
    int ele_count = hashmap_count(hmap);
    printf("ele count at end: %d\n", ele_count);
    hashmap_destroy(hmap);
    printf("hmap destroyed\n");
    
    return 0;
    

}