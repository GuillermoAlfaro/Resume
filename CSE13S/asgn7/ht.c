#include "node.h"
#include "salts.h"
#include "speck.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct HashTable HashTable;

typedef struct HashTableIterator HashTableIterator;

struct HashTable {
    uint64_t salt[2]; // Salt to use for the hash function.
    uint32_t size; // Number of slots in the hash function.
    Node **slots; // Array of hash table items.
};

struct HashTableIterator {
    HashTable *table; // Hash table to iterate over.
    uint32_t slot; // Current slot the iterator is on.
};

HashTableIterator *hti_create(HashTable *ht) {
    HashTableIterator *var = (HashTableIterator *) malloc(sizeof(HashTableIterator));
    var->table = ht;
    var->slot = 0;
    return var;
}
// Deletes all space made for HTI.
void hti_delete(HashTableIterator **hti) {
    free((*hti));
    *hti = NULL;
}
// Iterates over an HT till a node that isn't NULL is found.
Node *ht_iter(HashTableIterator *hti) {
    Node *temp;
    for (int i = hti->slot; i < hti->table->size; ++i) {
        temp = hti->table->slots[i];
        if (temp != NULL) {
            hti->slot += 1;
            return temp;
        }
        hti->slot += 1;
    }
    hti->slot = 0;
    return NULL;
}

// Creates space for HT and its Node array.
HashTable *ht_create(uint32_t size) {
    HashTable *var = (HashTable *) malloc(sizeof(HashTable));
    if ((var->slots = (Node **) malloc(size * sizeof(Node))) == NULL) {
        return NULL;
    }
    var->size = size;
    var->salt[0] = SALT_HASHTABLE_LO;
    var->salt[1] = SALT_HASHTABLE_HI;
    return var;
}
// Frees all the allocation made in ht_create.
void ht_delete(HashTable **ht) {
    free((*ht)->slots);
    (*ht)->slots = NULL;
    free(*ht);
    *ht = NULL;
}
// Returns size of HT.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}
// Searches if a word is in HT.
// If it's not there NULL is returned.
Node *ht_lookup(HashTable *ht, char *word) {
    uint32_t position = hash(ht->salt, word) % ht->size;
    for (int i = position; i < ht->size; ++i) {
        if (ht->slots[i] != NULL) {
            if (strcmp(ht->slots[i]->word, word) == 0) {
                return ht->slots[i];
            }
        } else {
            return NULL;
        }
    }
    return NULL;
}
// Inserts a word into hashtable.
Node *ht_insert(HashTable *ht, char *word) {
    Node *insert = ht_lookup(ht, word);
    if (insert != NULL) { // First looks up if word already exists.
        insert->count += 1; // If so then increments its count by one.
        return insert;
    }
    // Else creates new node with word and attempts to add to HT.
    uint32_t position = hash(ht->salt, word) % ht->size;
    insert = node_create(word);
    insert->count = 1;
    for (int i = position; i < ht->size; ++i) {
        if (ht->slots[i] == NULL) { // If the space at i is NULL then it's safe to insert.
            ht->slots[i] = insert;
            return insert;
        }
    }
    node_delete(&insert);
    return NULL;
}
// Iterates over entire HT printing each node.
void ht_print(HashTable *ht) {
    HashTableIterator *iterator = hti_create(ht);
    Node *to_be_printed = ht_iter(iterator);
    while (to_be_printed != NULL) {
        node_print(to_be_printed);
        to_be_printed = ht_iter(iterator);
    }
    hti_delete(&iterator);
}
