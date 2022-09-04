#include "pq.h"
#include "text.h"
#include "parser.h"
#include "bf.h"
#include "bv.h"
#include "node.h"
#include "ht.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int opt = 0, k = 5, l = 100;
    Metric metric;
    bool e, m, c, h;
    FILE *database = fopen("lib.db", "r");
    FILE *noise = fopen("noise.txt", "r");
    while ((opt = getopt(argc, argv, "emchd:n:k:l:")) != -1) {
        switch (opt) {
        case 'e': e = true; break;
        case 'm': m = true; break;
        case 'c': c = true; break;
        case 'h': h = true; break;
        case 'k': k = strtoul(optarg, NULL, 10); break;
        case 'l': l = strtoul(optarg, NULL, 10); break;
        case 'd':
            fclose(database);
            database = fopen(optarg, "r");
            break;
        case 'n':
            fclose(noise);
            noise = fopen(optarg, "r");
            break;
        default: break;
        }
    }

    if (h) {
        puts("SYNOPSIS");
        puts("   Identifies the author of some anonymous text passed through stdin.");
        puts("   Basically computes k-nearest neighbors, where k = 1 by default.");
        puts("   The number of noise words that are removed is tunable.");
        puts("   Uses either Euclidean, Manhattan, or cosine distance as metric.");
        puts("");
        puts("USAGE");
        puts("   ./identify [OPTIONS]");
        puts("");
        puts("OPTIONS");
        puts("   -d database     Database of authors and texts [default: lib.db]");
        puts("   -k matches      Set number of top matches to display [default: 5]");
        puts("   -n noisefile    Set file of words to ignore [default: noise.txt]");
        puts("   -l limit        Set noise word limit [default: 100]");
        puts("   -e              Set distance metric as Euclidean distance [default]");
        puts("   -m              Set distance metric as Manhattan distance");
        puts("   -c              Set distance metric as cosine distance");
        puts("   -v              Enable verbose printing of program run");
        puts("   -h              Display program help and usage");
        return 1;
    }
    if (database == NULL) {
        perror("Failed to open -d");
    }
    if (noise == NULL) {
        perror("Failed to open -n");
    }
    if (e) {
        metric = EUCLIDEAN;
    } else if (m) {
        metric = MANHATTAN;
    } else if (c) {
        metric = COSINE;
    }
    if (k < 1) {
        puts("K can't be less than 0");
        return 1;
    }
    if (l < 1) {
        puts("L can't be less than 0");
        return 1;
    }

    // Noise word Text
    char cd[256];
    FILE *lwords = fopen("lwords", "w+");
    if (lwords != NULL) {
        for (int i = 0; i < l; ++i) {
            fgets(cd, 256, noise);
            fputs(cd, lwords);
        }
    }

    fseek(lwords, 0, SEEK_SET);

    Text *txt_noise = text_create(lwords, NULL);

    // Anonymous text

    Text *anon = text_create(stdin, txt_noise);

    // Reads number of author/text pairs and creates PQ
    char *line = malloc(256);

    char *filepath = (char *) malloc(256);
    fgets(line, 256, database);
    PriorityQueue *pq = pq_create(atoi(line));
    FILE *text;

    Text *new_text;

    while (1) {
        char *author = (char *) malloc(256);
        fgets(author, 256, database);
        fgets(filepath, 256, database);
        if (feof(database)) {
            break;
        }
        strtok(filepath, "\n");
        strtok(author, "\n");
        text = fopen(filepath, "r+");
        if (text != NULL) {
            new_text = text_create(text, txt_noise);
            double distance = text_dist(new_text, anon, metric);
            enqueue(pq, author, distance);
            //printf("%s %f\n", author, distance);
        }
        fclose(text);
    }

    double distsance;
    printf("Top %d, metric: %s, noise limit: %d\n", k, metric_names[metric], l);
    for (int i = 0; i < k; ++i) {
        char *newaut = malloc(256);
        dequeue(pq, &newaut, &distsance);
        printf("%d) %s [%.15f]\n", i + 1, newaut, distsance);
    }

    text_delete(&new_text);
    text_delete(&txt_noise);
    text_delete(&anon);
    fclose(lwords);

    return 0;
}
