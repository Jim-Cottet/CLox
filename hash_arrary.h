#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Token.h"
#include <malloc.h>

#define SIZE 20

typedef struct {
    int key;
    Token *token;
} Item;

Item hash_array[SIZE];

int hash_code(int key) {
    return key % SIZE;
}

void insert_hash(int key, Token *token)
{
    Item *item = (Item*)malloc(sizeof(Item)); 
    item->token = token;
    item->key = key;
    // Generate Hash
    int hash_index = hash_code(key);
    // Traverse hash table
    while (hash_array[hash_index].token != NULL && hash_array[hash_index].key != -1) {
        ++hash_index;
        hash_index %= SIZE;
    }
    hash_array[hash_index] = *item;
}

 Item delete_hash(Item item) {
    int key = item.key;
    int hash_index = hash_code(key);
    while (hash_array[hash_index].token != NULL && hash_array[hash_index].key != -1) {

        ++hash_index;
        hash_index %= SIZE;
    }
    Item temp;
    temp.key = -1;
    temp.token = NULL;
    return temp;
}

void display_hash_table()
{
    for (int i = 0; i < SIZE; i++) 
    {
        if (hash_array[i].token != NULL) 
            printf(" (%d, %s, %d) ", hash_array[i].key, hash_array[i].token->lexeme, hash_array[i].token->line);
    }
    printf("\n");
}





