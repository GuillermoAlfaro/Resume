#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

int existTest(){
    List L = List();
    L.insertBefore(5);      // ( 5 |)
    L.insertBefore(10);     // ( 5 10 |)
    L.insertBefore(15);     // (5 10 15 |)
    L.insertAfter(20);      // (5 10 15 | 20)
    L.insertBefore(17);     // (5 10 15 17 | 20)
    L.movePrev();              // (5 10 15 | 17 20)
    L.moveNext();              // (5 10 15 17 | 20)
    L.eraseAfter();            // (5 10 15 17 |)
    L.eraseBefore();           // (5 10 15 |)
    L.setBefore(51);        // (5 10 51 |)
    L.movePrev();              // (5 10 | 51)
    L.insertAfter(42);      // (5 10 | 42 51)
    L.insertBefore(51);     // (5 10 51 | 42 51)
    L.movePrev();              // (5 10 | 51 42 51)
    L.setAfter(42);         // (5 10 | 42 42 51)
    L.movePrev();              // ( 5 | 10 42 42 51)
    L.insertBefore(51);     // (5 51 | 10 42 42 51)
    L.clear();                 // ()
    printf("length = %d\n", L.length());
    printf("Position of cursor = %d\n", L.position());
    std::cout << L.to_string();
    return 0;
}

int cleanUpTest(){
    List L = List();
    L.insertBefore(40);
    L.insertBefore(50);
    L.insertBefore(20);
    L.insertBefore(20);
    L.insertBefore(50);
    L.insertAfter(40);
    L.insertAfter(40);
    L.moveBack();
    L.insertAfter(70);
    L.moveBack();
    puts("Before Cleanup");
    std::cout << L.to_string();
    printf("Position of cursor = %d\n", L.position());
    L.cleanup();
    puts("\nAfter Cleanup");
    std::cout << L.to_string();
    printf("Position of cursor = %d\n", L.position());
    return 0;
}

int movingOnEmpty(){
    List L = List();
    L.moveFront();
    L.moveBack();

    return 0;
}

int concatTest(){
    List L = List();
    L.insertAfter(5);
    L.insertAfter(10);
    L.insertAfter(15);
    std::cout << L.to_string();
    List H = List();
    H.insertAfter(-5);
    H.insertAfter(0);
    std::cout << H.to_string();
    List cat = L.concat(H);
    std::cout << cat.to_string();

    return 0;

}

int equalstest(){
    List L = List();
    List G = List();

    L.insertBefore(5);
    L.insertBefore(20);
    L.insertAfter(40);
    L.insertBefore(10);

    G.insertBefore(5);
    G.insertBefore(20);
    puts("L List is");
    std::cout<<L;

    puts("\nG List is");
    std::cout<<G;

    puts("\nComparison result of L == G is");
    if(L.equals(G)){
        puts("TRUE");
    } else {
        puts("FALSE");
    }

    L = G;

    puts("\nafter L=G");
    puts("L List is");
    std::cout<<L;
    puts("\nComparison result of L == G is");
    if(L.equals(G)){
        puts("TRUE");
    } else {
        puts("FALSE");
    }

    return 0;
}

int peekFront(){
    List L = List();
    L.insertAfter(5);
    L.insertAfter(10);

    return 0;

}

int testets(){
    List A;
    List B = A;

    return 0;
}

int clean(){
    List A = List();
    A.insertAfter(1);
    A.insertAfter(2);
    A.insertAfter(3);
    A.insertAfter(1);
    A.insertAfter(2);
    A.insertAfter(1);
    A.cleanup();
    if (A.length() != 3)
        return 1;
    return 0;

}

int emptyClear(){
    List A = List();
    A.clear();
    if (A.length() != 0 || A.position() != 0){
        puts("returned 1");
        return 1;
    }
    return 0;

}

int findNextValue(){
    List A = List();
    A.insertAfter(77);
    A.insertAfter(55);
    A.insertAfter(1);
    A.insertAfter(34);
    A.insertAfter(1);
    A.insertAfter(77);
    A.insertAfter(77);
    A.insertAfter(34);
    if (A.findNext(99) != -1){
        puts("returned 1");
        return 1;
    }
    if (A.position() != A.length()){
        puts("returned 2");
        return 2;
    }
    A.moveFront();
    if (A.findNext(1) != 4) {
        puts("returned 3");
        return 3;
    }
    if (A.findNext(1) != 6){
        puts("returned 4");
        return 4;
    }

    if (A.findNext(1) != -1){
        puts("returned 5");
        return 5;
    }
    A.moveFront();
    A.cleanup();
    int found = A.findNext(1);
    if (found != 3){
        puts("\nreturned 6");
        return 6;
    }

    if (A.findNext(1) != -1){
        puts("returned 7");
        return 7;
    }
    puts("returned 0");
    return 0;

}

int cleanupPos(){
    List A = List();
    List B = List();
    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(2);
    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(3);
    A.insertBefore(1);
    A.findPrev(3);
    printf("Before cleanup\ncursor pos = %d\n", A.position());
    std::cout << A;
    A.cleanup();
    printf("\nAfter cleanup\ncursor pos = %d\n", A.position());
    std::cout << A;
    if (A.position() != 2){
        puts("\n1 was returned");
        return 1;
    }
    B.insertBefore(1);
    B.insertBefore(2);
    B.insertBefore(2);
    B.insertBefore(1);
    B.insertBefore(2);
    B.insertBefore(3);
    B.insertBefore(1);
    B.findPrev(3);
    B.movePrev();
    B.cleanup();
    if (B.position() != 2){
        puts("2 was returned");
        return 2;
    }
    puts("0 was returned");
    return 0;

}

int main(){
    //existTest();
    //cleanUpTest();
    //movingOnEmpty();
    //concatTest();
    //equalstest();
    //peekFront();
    //testets();
    //emptyClear();
    //clean();
    //findNextValue();
    cleanupPos();

}