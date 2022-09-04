#include <iostream>
#include <sstream>
#include <string>
#include "List.h"

void shuffle(List& D){
    int len = D.length();
    int left = len/2;
    List DCopy = D;
    List temp = List();
    bool lever = false;
    DCopy.moveFront();
    D.moveFront();
    for (int i = 0; i < left; ++i) {
        DCopy.moveNext();
    }
    for (int i = 0; i < len; ++i) {
        if (!lever){
            temp.insertBefore(DCopy.moveNext());
            lever = true;
            continue;
        }
        if (lever){
            temp.insertBefore(D.moveNext());
            lever = false;
            continue;
        }
    }
    D = temp;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <input integer>}\n", argv[0]);
        exit(1);
    }
    int num = std::stoi(argv[1]);
    List L = List();
    int counter = 0;
    printf("deck size       shuffle count\n");
    printf("------------------------------\n");
    for (int i = 0; i < num; ++i) {
        L.insertBefore(i);
        List LCopy = L;
        shuffle(L);
        counter += 1;

        while (L.equals(LCopy) != true){
            shuffle(L);
            counter += 1;
        }
        if (i+1 < 10){
            printf(" %d               %d\n", i+1, counter);
        } else {
            printf(" %d              %d\n", i+1, counter);
        }

        counter = 0;
    }

    return 0;
}