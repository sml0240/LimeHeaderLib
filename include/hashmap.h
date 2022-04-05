#pragma once
#include "optional.h"
#include "hashing/murmurhash.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 100
#define MAX_KEY_LEN 255

typedef enum HashMapReturnValue {HASHMAP_ENTRY, HASHMAP_ENTRY_POSITION, HASHMAP_VALUE, HASHMAP_KEY, HASHMAP_KEYS, HASHMAP_HAS_KEY, HASHMAP_VALUES,}HashMapReturnValue;


typedef struct HashData
{
    uint32_t seed;
}HashData;

typedef struct Entry
{
    //int64_t hash;
    char* key;
    void*   value;
    uint32_t hash;
    struct Entry* next;
} Entry;

typedef struct Bucket
{
    bool empty;
    struct Entry* entry;
    size_t entry_count;
} Bucket;

typedef struct HashMap
{
    int32_t size;
    int32_t entry_count;
    struct Bucket* map;
    HashData* hash_data;

} HashMap;


HashMap* hashmap_create();

bool hashmap_add(HashMap* hashmap, char* key, void* value);
OptionalPtr hashmap_remove(HashMap* hashmap, char* key);
OptionalPtr hashmap_get(HashMap* hashmap, char* key);
OptionalPtr hashmap_find(Entry* entry, uint32_t hash, HashMapReturnValue retval);
bool hashmap_resize_check(HashMap* hashmap);
//resize is not in use, just there if will be implemented
bool hashmap_resize(HashMap* hashmap, bool increment);

bool hashmap_has_key(HashMap* hashmap, char* key);
int hashmap_count(HashMap* hashmap);

// bucket functions
int bucket_count(Bucket* bucket);
bool valid_bucket_index(Bucket* bucket, int index);
Entry* bucket_get_entry_at_position(Bucket* bucket, int position);


int64_t get_hash(char* key);
uint32_t get_hash_murmur(char* key, uint32_t seed);

void clean_entry(Entry* entry);
void clean_bucket(Bucket* bucket);



HashMap* hashmap_create()
{
    HashMap* hashmap;
    hashmap = malloc(sizeof(HashMap));
    if (hashmap == NULL)
    {
        fprintf(stderr, "HashMap init failed\n");
        return NULL;
    }
    hashmap->size = HASHMAP_SIZE;
    hashmap->map = malloc(sizeof(Bucket) * HASHMAP_SIZE);
    if (hashmap->map == NULL)
    {
        fprintf(stderr, "HashMap init failed\n");
        return NULL;
    }
    Bucket empty_bucket = {.empty = true, .entry = NULL};

    for (size_t i = 0; i < HASHMAP_SIZE; i++)
    {
        memcpy(hashmap->map + i, &empty_bucket, sizeof(Bucket));
    }
    // hashing data creation
    HashData* hdata = malloc(sizeof(HashData));
    uint32_t seed = 0;
    hdata->seed = seed;
    hashmap->hash_data = hdata;

    return hashmap;
}

bool hashmap_add(HashMap* hashmap, char* key, void* value)
{

    uint32_t hash = get_hash_murmur(key, hashmap->hash_data->seed);//get_hash(key);
    int32_t index = hash % HASHMAP_SIZE;
    Entry new_entry = {.key = key, .hash = hash, .value = value, .next = NULL};
    
    if (hashmap->map[index].empty == true)
    {
        void* new_entry_mem = malloc(sizeof(Entry));
        memcpy(new_entry_mem, &new_entry, sizeof(Entry));
        hashmap->map[index].empty = false;
        hashmap->map[index].entry = new_entry_mem;

        hashmap->map[index].entry_count++;
        hashmap->entry_count++;

        printf("bucket entry_count on add: %d", hashmap->map[index].entry_count);
        return true;
    }

    Entry* old_entry = hashmap->map[index].entry;

    while (old_entry->next != NULL && old_entry->hash == hash)
        old_entry = old_entry->next;

    if (old_entry->hash == hash)
        return false;

    void* new_entry_mem = malloc(sizeof(Entry));
    if (new_entry_mem == NULL)
    {
        fprintf(stderr ,"Adding to hashmap failed\n");
        return false;
    }
    memcpy(new_entry_mem, &new_entry, sizeof(Entry));
    old_entry->next = new_entry_mem;
    
    printf("bucket entry_count on add: %d", hashmap->map[index].entry_count);
    hashmap->map[index].entry_count++;
    hashmap->entry_count++;
    
    return true;
}

// removes and returns removed value
OptionalPtr hashmap_remove(HashMap* hashmap, char* key)
{
    //int64_t hash = get_hash(key);
    OptionalPtr ret_opt;
    opt_set_empty(&ret_opt);

    uint32_t hash = get_hash_murmur(key, hashmap->hash_data->seed);
    int32_t index = hash % HASHMAP_SIZE;

    if (hashmap->map[index].empty)
    {
        printf("hashmap index %d is empty", index);
        return ret_opt;

        //return false;
    }

    Entry* entry = hashmap->map[index].entry;
    OptionalPtr opt = hashmap_find(entry, hash, HASHMAP_ENTRY_POSITION);


    if (opt.has_value)
    {
        bool has_prev = false;
        bool has_next = false;

        size_t e_idx = (size_t) opt.value;
        int bc = bucket_count(&hashmap->map[index]);
        Entry* curr_entry = bucket_get_entry_at_position(&hashmap->map[index], e_idx);
        Entry* prev_entry;
        Entry* next_entry;

        printf("bucket count: %d, hmap count: %d\n" ,bc, hashmap->entry_count);
        printf("prev e_idx: %d current e_idx: %d, next e_idx: %dn", e_idx-1, e_idx, e_idx + 1);
        if (e_idx > 0)
        {
            has_prev = true;
            prev_entry = bucket_get_entry_at_position(&hashmap->map[index], e_idx-1);
        }
        else
            printf("e_idx is not greater than 0\n");
        
        if (curr_entry->next != NULL)
        {
            printf("next entry does exist\n");
            has_next = true;
            next_entry = bucket_get_entry_at_position(&hashmap->map[index], e_idx+1);
        }

        if (has_prev && has_next)
        {
            printf("has prev and next!\n");
            prev_entry->next = next_entry;
        }
        else if (has_prev && !has_next)
        {
            printf("has prev and not next!\n");
            prev_entry->next = NULL;
        }

        opt_set_value(&ret_opt, curr_entry->value);

        clean_entry(curr_entry);
        hashmap->map[index].entry_count--;
        hashmap->entry_count--;
        
        if (hashmap->map[index].entry_count == 0)
            hashmap->map[index].empty = true;

        return ret_opt;
    }
    return ret_opt;
}

// gets hashmap entry if exists
OptionalPtr hashmap_get(HashMap* hashmap, char* key)
{
    OptionalPtr opt = {.value = NULL, .has_value = false};

    uint32_t hash = get_hash_murmur(key, hashmap->hash_data->seed); // get_hash(key);
    int32_t index = hash % HASHMAP_SIZE;

    if (hashmap->map[index].empty)
        return opt;

    Entry* entry = hashmap->map[index].entry;

    return hashmap_find(entry, hash, HASHMAP_VALUE);
}

// sketchy with opt creation in both hmap_find and hmap_get
// using this for remove and get functions
OptionalPtr hashmap_find(Entry* entry, uint32_t hash, HashMapReturnValue retval_type)
{
    OptionalPtr opt = {.value = NULL, .has_value = false};

    int count = 0;
    if (entry->hash == hash)
    {
        opt.has_value = true;
        switch (retval_type)
        {
            //{HASHMAP_ENTRY, HASHMAP_VALUE, HASHMAP_KEY}HashMapReturnValue
            case HASHMAP_ENTRY:
                opt.value = (void*) entry;
                break;
            case HASHMAP_ENTRY_POSITION:
                opt.value = (void*) count;
                break;
            case HASHMAP_VALUE:
                opt.value = entry->value;
                break;
            case HASHMAP_HAS_KEY:
                break;
        }
    }

    while (entry->next != NULL)
    {
        entry = entry->next;
        if (entry->hash == hash)
        {
            opt.has_value = true;
            return opt;
            switch (retval_type)
            {
            case HASHMAP_ENTRY:
                opt.value = (void*) entry;
                break;
            case HASHMAP_ENTRY_POSITION:
                opt.value = (void*) count;
                break;
            case HASHMAP_VALUE:
                opt.value = entry->value;
                break;
            case HASHMAP_HAS_KEY:
            // returns only opt.has_value = true
                break;
            }
        }
        count++;
    }
    return opt;
}

bool hashmap_has_key(HashMap* hashmap, char* key)
{
    uint32_t hash = get_hash_murmur(key, hashmap->hash_data->seed); // get_hash(key);
    int32_t index = hash % HASHMAP_SIZE;
    Entry* entry = hashmap->map[index].entry;

    OptionalPtr opt = hashmap_find(entry, hash, HASHMAP_HAS_KEY);
    return opt.has_value;
}


// counts all entrys in hashmap
int hashmap_count(HashMap* hashmap)
{
    int c = 0;

    for (int i = 0; i < hashmap->size; i++)
    {
        Bucket* bucket = &hashmap->map[i];
        if (!bucket->empty)
        {
            c += bucket_count(bucket);
        }
    }
    return c;
}

Entry* get_bucket_entrys(Bucket* bucket)
{
    int c = 0;

    int ec = bucket->entry_count;
    Entry* entry_arr[ec];

    Entry* entry = bucket->entry;

    entry_arr[c] = entry;

    while (entry->next != NULL)
    {
        entry = entry->next;
        c++;
        entry_arr[c] = entry;
    }
    return *entry_arr;
}

char* hashmap_keys(HashMap* hashmap)
{

    uint8_t* key_arr = malloc(hashmap->entry_count * 8);

    for (int i = 0; i < hashmap->size; i++)
    {
        Bucket* bucket = &hashmap->map[i];

        if (!bucket->empty)
        {
            int c = i;
            printf("found a bucket not empty! \n");
            Entry* entry = bucket->entry;
            memcpy(key_arr +(i), entry->key, 8);
            //printf("key arr after added key, %s, \n", &key_arr);
            while (entry->next != NULL)
            {
                c++;
                entry = entry->next;
                memcpy(key_arr + (c*8), entry->key, 8);
            }
        }
    }
    return key_arr;
}


char* get_entry_key(Entry* entry)
{
    return entry->key;
}
void* get_entry_value(Entry* entry)
{
    return entry->value;
}

// shit
void search_and_destroy(HashMap* hashmap)
{
    for (int i = 0; i < hashmap->size; i++)
    {
        Bucket* bucket = &hashmap->map[i];
        if (!bucket->empty)
        {
            int c = bucket_count(bucket);
            if (c > 0)
            {
                Entry* entry = bucket->entry;
                printf("entry found value %d next: %d ", entry->value, entry->next);
                clean_bucket(bucket);
            }
        }
    }
}

// ------------- Resize functions ---------------

// ---------------- not in use ------------------
bool hashmap_resize_check(HashMap* hashmap)
{
    return (((float) hashmap->size - (float) hashmap->entry_count) / hashmap->size) <= 0.3;
}

bool hashmap_resize(HashMap* hashmap, bool increment)
{
    size_t new_size;
    if (increment)
    {
        new_size = (hashmap->size * 1.5);
    }
    
    Bucket* bck = realloc(hashmap->map, new_size);
    if (bck == NULL)
    {
        return false;
    }
    
    hashmap->map = bck;
    return false;
}

//----------------------------------------------------------
//------------------ Bucket functions ----------------------
//----------------------------------------------------------

int bucket_count(Bucket* bucket)
{   
    int c = 0;
    if (bucket->empty)
    {
        return c;
    }
    c++;
    Entry* entry = bucket->entry;
    while (entry->next != NULL)
    {
        entry = entry->next;
        c++;
    }
    return c;
}

bool valid_bucket_index(Bucket* bucket, int index)
{
    return (bucket->entry_count >= index);
    // int bc = bucket_count(bucket);
    // return (index <= bc);
}

Entry* bucket_get_entry_at_position(Bucket* bucket, int position)
{
    int count = 0;
    Entry* entry = bucket->entry;

    while (entry->next != NULL)
    {
        entry = entry->next;
        if ( count == position)
        {
            break;
        }
        count++;
    }
    return entry;
}

//----------------------------------------------------------
//------------------ Dealloc functions ---------------------
//----------------------------------------------------------

void hashmap_destroy(HashMap* hashmap)
{   
    //Bucket* bck = &hashmap->map;
    
    for (int i = 0; i < hashmap->size; i++)
    {
        //printf("bucket iter: %d\n", i);
        Bucket* bucket = &hashmap->map[i];
        if (!bucket->empty)
        {
            clean_bucket(bucket);
        }
    }

    int hmap_count = hashmap_count(hashmap);
    //printf("hmap count after delete: %d\n", hmap_count);
    free(hashmap->map);
    //printf("here after free hashmap->map!\n");

    free(hashmap->hash_data);
    //printf("here after free hashmap->hash_data!\n");
    free(hashmap);
    //printf("here at destroy hashmap end!\n");
}

void clean_bucket(Bucket* bucket)
{
    //printf("in clean bucket!\n");
    if (bucket->entry != NULL)
    {
        Entry* entry = bucket->entry;

        for (int count = bucket->entry_count; count > 0; count--)
        {
            entry = bucket_get_entry_at_position(bucket, count);
            clean_entry(entry);
            bucket->entry_count--;
        }
        bucket->entry = NULL;
        bucket->empty = true;
    }
    
    //printf("at end of clean bucket!\n");
}

void clean_entry(Entry* entry)
{
    //printf("in clean entry!\n");

    if (entry == NULL)
    {
        printf("entry is null in clean_entry\n");
    }
    else
    {
        free(entry);
        printf("freed entry!\n");
        entry = NULL;
    }

   // printf("end of clean entry!\n");
    //printf("entry = %d\n", entry);

}
//----------------------------------------------------------
//-------------------- Hash functions ----------------------
//----------------------------------------------------------
int64_t get_hash(char* key)
{
    char c = *key;
    int64_t hash = 5381;

    for (size_t i = 0; c != '\0' && i < 255; i++)
    {
        hash = ((hash << 5) + hash) + c;
        c = *(++key);
    }
    return hash = hash < 0 ? -hash : hash;
}

uint32_t get_hash_murmur(char* key, uint32_t seed)
{
    uint32_t hash;
    hash = murmurhash(key, (uint32_t) strlen(key), seed);
    return hash;
}