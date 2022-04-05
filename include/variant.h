#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum Type { Empty, Char, Integer, u8, u32, u64, usize, i8, i32, i64, Float, String, Object }Type;
typedef enum VarError { VarCastError, VarTypeError, VarEmptyError }VarError;


typedef struct InnerVariantData 
{
    union
    {
        float f_value;
        int i_value;
        uint8_t u8_value;
        uint32_t u32_value;
        int32_t i32_value;
        uint64_t u64_value;
        int64_t i64_value;
        char c_value;
        char* s_value;
        size_t size_value;
        void* obj_value;
    };

}InnerVariantData;

typedef struct Variant
{
    InnerVariantData* i_data;
    size_t type;
} Variant;


// VarResult variant_result_ok(Variant* variant)
// {
//     VarResult res;
//     res.variant = variant;
//     res.result = OK;
//     return res;
// }


Variant* variant_create(void* value, size_t type)
{
    InnerVariantData* p_data = malloc(sizeof(InnerVariantData));
    Variant* variant = malloc(sizeof(Variant));
    variant->i_data = p_data;
    variant->i_data->obj_value = value;
    variant->type = type;
    return variant;
}

Variant* variant_create_empty()
{
    InnerVariantData* p_data = malloc(sizeof(InnerVariantData));
    Variant* variant = malloc(sizeof(Variant));
    variant->i_data = p_data;
    variant->type = Empty;
    return variant;
}

uint8_t* variant_as_bytes(Variant* variant)
{
    uint8_t* bytePtr = (uint8_t*) variant;
    printf("byteptr from var: %s", bytePtr);
    return bytePtr;
}

char* variant_as_str(Variant* variant)
{
    char* var_str = (char*) variant;
    printf("var as string: %s\n", var_str);
    return var_str;
}

// returns void*
void* variant_get_value(Variant* variant)
{
    return variant->i_data->obj_value;
}

Type variant_get_type(Variant* variant)
{
    return variant->type;
}

bool is_empty(Variant* variant)
{
    return (variant->type == Empty);
}

bool variant_can_unwrap(Variant* variant, Type type)
{
   return (variant->type == type);
}

void variant_destroy(Variant* variant)
{
    if (variant->type == Object)
        free(&variant->i_data->obj_value);

    free(variant);
}

int variant_get_as_int(Variant* variant)
{
    //int val = *(int*) variant->i_data->i_value;
    int val = *(int*) variant->i_data->obj_value;
    return val;
}

int8_t variant_get_as_i8(Variant* variant)
{
    //int8_t val = *(int8_t*) variant->i_data->u8_value;
    int8_t val = *(int8_t*) variant->i_data->obj_value;
    return val;
}

int32_t variant_get_as_i32(Variant* variant)
{
    //int32_t val = *(int32_t*) variant->i_data->i_value;
    int32_t val = *(int32_t*) variant->i_data->obj_value;
    return val;
}

int64_t variant_get_as_i64(Variant* variant)
{
    //int64_t val = *(int64_t*) variant->i_data->i32_value;
    int64_t val = *(int64_t*) variant->i_data->obj_value;
    return val;
}

uint8_t variant_get_as_u8(Variant* variant)
{
    //uint8_t val = *(uint8_t*) variant->i_data->u8_value;
    uint8_t val = *(uint8_t*) variant->i_data->obj_value;
    return val;
}

uint32_t variant_get_as_u32(Variant* variant)
{
    //uint32_t val = *(uint32_t*) variant->i_data->u32_value;
    uint32_t val = *(uint32_t*) variant->i_data->obj_value;
    return val;
}
uint64_t variant_get_as_u64(Variant* variant)
{
    //uint64_t val = (uint64_t) variant->i_data->u64_value;
    uint64_t val = *(uint64_t*) variant->i_data->obj_value;
    return val;
}

size_t variant_get_as_usize(Variant* variant)
{
    //size_t val = (size_t) variant->i_data->size_value;
    size_t val = *(size_t*) variant->i_data->obj_value;
    return val;
}

char* variant_get_as_str(Variant* variant)
{
    //return (char*) variant->i_data->s_value;

    char* val = (char*) variant->i_data->obj_value;
    return val;
}

float variant_get_as_float(Variant* variant)
{
   // float val = *(float*) variant->i_data->f_value;

    float val = *(float*) variant->i_data->obj_value;
    return val;
}

double variant_get_as_double(Variant* variant)
{
    //float val = (float) variant->i_data->f_value;
    float val = *(float*) variant->i_data->obj_value;
    return val;
}

bool variant_set_value(Variant* variant, void* value, Type type)
{
    switch (type)
    {
        case Empty:
        {
            break;
        }
        case Char:
        {
            variant->i_data->c_value = *(char*) value;
            break;
        }
        case Integer:
        {
            //variant->i_data->obj_value = *(int*) value;
            variant->i_data->obj_value = value;
            break;
        }
        case u8:
        {
            //variant->i_data->u8_value = *(uint8_t*) value;
            variant->i_data->obj_value = value;
            break;
        }
        case i8:
        {
            //variant->i_data->u8_value = *(uint8_t*) value;
            variant->i_data->obj_value = value;
            break;
        }
        case u32:
        {
            variant->i_data->obj_value = value;
            //variant->i_data->u32_value = *(uint8_t*) value;
            break;
        }
        case i32:
        {
            variant->i_data->obj_value = value;
            //variant->i_data->i_value = *(int32_t*) value;
            break;
        }
        case u64:
        {
            variant->i_data->obj_value = value;
            //variant->i_data->i64_value = *(uint64_t*) value;
            break;
        }
        case i64:
        {
            variant->i_data->obj_value = value;
            //variant->i_data->i64_value = *(int64_t*) value;
            break;
        }
        case Float:
        {
            variant->i_data->obj_value = value;
            //variant->i_data->f_value = *(float*) value;
            break;
        }

        case String:
        {
            variant->i_data->obj_value = value;
            //char* str = (char*) value;
            //variant->i_data->s_value = strcpy(variant->i_data->s_value, str);
            break;
        }
        default:
            printf("Error: could not set value\n");
            return false;
    }
    
    variant->type = type;
    return true;
}

// void* get_value(Variant* variant)
// {
//     switch (variant->type)
//     {
//         case Empty:
//         {
//             break;
//         }

//         case Char:
//         {
//             return variant_get_as_u8(variant);

//             //variant->i_data->c_value = *(char*) value;
//             break;
//         }

//         case Integer:
//         {
//             return variant_get_as_int(variant);

//             //variant->i_data->i_value = *(int*) value;
//             break;
            
//         }
//         case u8:
//         {
//             return variant_get_as_u8(variant);

//             //variant->i_data->u8_value = *(uint8_t*) value;
//             break;
//         }
//         case u32:
//         {
//             return variant_get_as_u32(variant);

//             //variant->i_data->u32_value = *(uint8_t*) value;
//             break;
//         }
//         case i32:
//         {
//             return variant_get_as_i32(variant);

//             //variant->i_data->i_value = *(int32_t*) value;
//             break;
//         }
//         case u64:
//         {
//             return variant_get_as_u64(variant);
//             //variant->i_data->i64_value = *(uint64_t*) value;
//             break;
//         }
//         case i64:
//         {
//             return variant_get_as_i64(variant);

//             //variant->i_data->i64_value = *(int64_t*) value;
//             break;
//         }
//         case Float:
//         {
//             variant_get_as_float(variant);
//             //variant->i_data->f_value = *(float*) value;
//             break;
//         }
//         case String:
//         {
//             return variant_get_as_str(variant);
//             //char* str = (char*) value;
//             //variant->i_data->s_value = strcpy(variant->i_data->s_value, str);
//             break;
//         }
//         default:
//             printf("no match!\n");
//     }
// }

int* variant_get_valid_casts(Type type)
{
    int* arr = calloc(10 , sizeof(int));
    switch (type)
    {
        case Empty:
        {
            arr[0] = Empty;
            break;
        }
        case Char:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case Integer:
        {
            arr[0] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[1] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = String; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = Float; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case i8:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case u8:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case i32:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case u32:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case i64:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        case u64:
        {
            //arr[0] = i8;
            // arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            // arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            // arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }

        case Float:
        {
            arr[0] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[1] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }

        case String:
        {
            arr[0] = i8;
            arr[1] = u8; // U32, I32, U64, I64, INTEGER, STRING};
            arr[2] = u32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[3] = i32; // U32, I32, U64, I64, INTEGER, STRING};
            arr[4] = u64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[5] = i64; // U32, I32, U64, I64, INTEGER, STRING};
            arr[6] = Integer; // U32, I32, U64, I64, INTEGER, STRING};
            arr[7] = usize; // U32, I32, U64, I64, INTEGER, STRING};
            arr[8] = String; // U32, I32, U64, I64, INTEGER, STRING};
            break;
        }
        default:
        {
            arr[1] = Empty;
        }
    }
    return arr;
}

// EMPTY, CHAR, INTEGER, FLOAT, STRING, OBJECT
bool variant_can_cast(Variant* variant, Type type)
{
    int* valid_arr;
    int ctype;
    bool in_type_ok = false;

    for (int i = Empty; i <= Object; i++)
    {
        if (type == i)
        {
            in_type_ok = true;
            break;
        }
    }
    if (!in_type_ok)
    {
        return false;
    }
    ctype = type;
    
    valid_arr = variant_get_valid_casts(ctype);
    for (int i = 0; i<10; i++)
    {
        if (valid_arr[i] == type)
            return true;
    }
    return false;
}

// EMPTY, CHAR, INTEGER, FLOAT, STRING, OBJECT
void variant_print(Variant* variant)
{
    switch (variant->type)
    {
        case Empty:
            break;
        case Char:
            printf("Variant.CHAR\n");
            break;
        case Integer:
        {
            int i = variant_get_as_int(variant);
            printf("Variant.INTEGER\n");
            printf("variant value: %d\n", i);
            break;
        }
        case i8:
        {
            int8_t i = variant_get_as_i8(variant);
            printf("Variant.INTEGER\n");
            printf("variant value: %d\n", i);
            break;
        }
        case Float:
        {
            float f = variant_get_as_float(variant);
            printf("variant value %f\n", f);
            break;
        }
        case String:
        {
            char* s = variant_get_as_str(variant);
            printf("Variant.STRING\n");
            printf("variant value: %s\n", s);
            break;
        }
    }
}