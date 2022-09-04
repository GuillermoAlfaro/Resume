#include "metric.h"
#include "ht.h"
#include "bf.h"
#include "parser.h"
#include "metric.h"
#include "speck.h"
#include "ctype.h"

#include <regex.h>
#include <string.h>
#include <inttypes.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD "[a-zA-Z]+"

extern uint32_t noiselimit; // Number of noise words to filter out.

typedef struct Text Text;

struct Text {
    HashTable *ht;
    BloomFilter *bf;
    uint32_t word_count;
};

int total_count(Text *text1, Text *text2) {
    HashTableIterator *tone = hti_create(text1->ht);
    HashTableIterator *ttwo = hti_create(text2->ht);

    int count = 0;
    Node *iter = ht_iter(tone);
    while (iter != NULL) {
        count += 1;
        iter = ht_iter(tone);
    }

    iter = ht_iter(ttwo);
    while (iter != NULL) {
        count += 1;
        iter = ht_iter(ttwo);
    }
    hti_delete(&tone);
    hti_delete(&ttwo);
    return count;
}

char **array(Text *text1, Text *text2, int *unique) {
    int count = total_count(text1, text2), pointer = 0;
    char **arr = (char **) malloc(count * sizeof(char *));
    HashTableIterator *one = hti_create(text1->ht);
    HashTableIterator *two = hti_create(text2->ht);
    Node *iter = ht_iter(one);
    while (iter != NULL) {
        arr[pointer] = (char *) malloc(sizeof(char) * sizeof(iter->word));
        arr[pointer] = iter->word;
        iter = ht_iter(one);
        pointer += 1;
    }
    iter = ht_iter(two);
    while (iter != NULL) {
        if (!bf_probe(text1->bf, iter->word)) {
            arr[pointer] = (char *) malloc(sizeof(char) * sizeof(iter->word));
            arr[pointer] = iter->word;
            pointer += 1;
        }
        iter = ht_iter(two);
    }

    *unique = pointer;
    return arr;
}

Text *text_create(FILE *infile, Text *noise) {
    Text *var = (Text *) malloc(sizeof(Text));
    var->ht = ht_create(1 << 19);
    var->bf = bf_create(1 << 21);
    if (var->ht == NULL || var->bf == NULL) {
        return NULL;
    }
    var->word_count = 0;

    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return NULL;
    }
    char *word = NULL;
    while ((word = next_word(infile, &re)) != NULL) {
        for (int i = 0; word[i]; ++i) {
            word[i] = tolower(word[i]);
        }
        if (noise != NULL) {
            if (!bf_probe(noise->bf, word)) {
                bf_insert(var->bf, word);
                ht_insert(var->ht, word);
                var->word_count += 1;
            }
        } else {
            if (!bf_probe(var->bf, word)) {
                bf_insert(var->bf, word);
            }
            ht_insert(var->ht, word);
            var->word_count += 1;
        }
    }
    return var;
}

void text_delete(Text **text) {
    ht_delete(&(*text)->ht);
    bf_delete(&(*text)->bf);
    (*text)->ht = NULL;
    (*text)->bf = NULL;
    free(*text);
    *text = NULL;
}

double text_dist(Text *text1, Text *text2, Metric metric) {
    int size;
    char **arr = array(text2, text1, &size);
    uint32_t t1_vector[size], t2_vector[size], t1_total = 0, t2_total = 0;
    Node *t1_node, *t2_node;
    for (int i = 0; i < size; ++i) {
        t1_node = ht_lookup(text1->ht, arr[i]);
        t2_node = ht_lookup(text2->ht, arr[i]);
        if (t1_node != NULL) {
            t1_vector[i] = t1_node->count;
            t1_total += t1_node->count;
        } else {
            t1_vector[i] = 0;
        }
        if (t2_node != NULL) {
            t2_vector[i] = t2_node->count;
            t2_total += t2_node->count;
        } else {
            t2_vector[i] = 0;
        }
    }

    double distance = 0;
    for (int i = 0; i < size; ++i) {
        float a = t1_vector[i], b = t1_total, c = t2_vector[i], d = t2_total;
        if (metric == MANHATTAN) {
            distance += fabs(a / b - c / d);
        }
        if (metric == EUCLIDEAN) {
            distance += pow(a / b - c / d, 2);
        }
        if (metric == COSINE) {
            distance += a / b * c / d;
        }
    }
    if (metric == EUCLIDEAN) {
        distance = sqrtf(distance);
    }
    if (metric == COSINE) {
        distance = 1 - distance;
    }
    //printf("%s = %f\n", metric_names[metric], distance);
    return distance;
}

double text_frequency(Text *text, char *word) {
    Node *temp = ht_lookup(text->ht, word);
    if (temp != NULL) {
        double count = temp->count, wordcount = text->word_count;
        return count / wordcount;
    }
    return 0;
}

bool text_contains(Text *text, char *word) {
    if (bf_probe(text->bf, word)) {
        return true;
    }
    return false;
}

void text_print(Text *text) {
    //bf_print(text->bf);
    ht_print(text->ht);
    printf("Word count is: %" PRIu32 "\n", text->word_count);
}
