#include "common.h"

#define SIZE 16

typedef struct Item {
    char *key;
    TokenType keyword;
    struct Item *next;
} Item;

typedef struct {
    Item **items;
} HashTable;

HashTable* create_table() {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->items = (Item **)malloc(SIZE * sizeof(Item *));
    for (int i = 0; i < SIZE; i++) {
        table->items[i] = NULL;
    }
    return table;
}

unsigned int hash(const char *key) {

    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (; i < key_len; i++) {
        value = value * 37 + key[i];
    }
    value = value % SIZE;
    return value;
}

void insert(HashTable *table, const char *key, TokenType type) {
    unsigned int slot = hash(key);
    Item *item = table->items[slot];

    Item *entry = (Item *)malloc(sizeof(Item));
    entry->key = strdup(key);
    entry->keyword = type;

    entry->next = item;
    table->items[slot] = entry;
}


TokenType lookup(HashTable *table, const char *key) {
    unsigned int slot = hash(key);
    Item *item = table->items[slot];
    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item->keyword;
        }
        item = item->next;
    }
    printf("key %s not found\n", key);
    return TOKEN_EOF; // Not found
}






