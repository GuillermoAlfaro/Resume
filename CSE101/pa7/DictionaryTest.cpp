#include "Dictionary.h"
using namespace std;

int dicInsertion(){
    Dictionary D = Dictionary();
    D.setValue("hammer", 911);
    D.setValue("ring", 512);
    D.setValue("name", 661);
    D.setValue("ring", 1000);
    D.begin();
    D.remove("ring");
    printf("Size = %d\n", D.size());
    printf("hasCurrent = %d\n\n", D.hasCurrent());
    std::cout << "(" << D.currentKey() << " , " << D.currentVal() << ")\n";
    std::cout << D << "\n";
    return 0;
}

int equalsTest(){
    Dictionary A = Dictionary();
    Dictionary B = Dictionary();

    A.setValue("hammer", 4);
    A.setValue("knives", 6);
    A.setValue("cups", 9);
    A.setValue("spoons", 2);

    B = A;
    puts("equals test");
    std::cout << "A == B result: " << (A == B) << "\n";
    std::cout << "A\n" << A;
    std::cout << "\nB\n" << B;
    B.setValue("hammer", 6);
    std::cout << "A == B result: " << (A == B) << "\n";
    return 0;
}

int copyTest(){

    Dictionary A = Dictionary();
    A.setValue("cup", 20);
    A.setValue("shots", 80);
    A.setValue("food", 15);

    Dictionary B = A;
    B.setValue("cup", 20000);
    std::cout << "A\n" << A;
    std::cout << "\nB\n" << B;
    return 0;
}

int removeTest(){
    Dictionary A = Dictionary();

    A.setValue("one", 1);
    A.setValue("two", 2);
    A.setValue("three", 3);

    Dictionary B = Dictionary();
    B.setValue("one", 1);
    B.setValue("two", 2);
    B.setValue("three", 3);

    std::string d, c;

    for(B.begin(); B.hasCurrent(); B.next()){
        d = B.currentKey();
        c = B.currentVal();
        cout << "("+d+", " << c << ") ";
    }
    puts("\n");

//    for(A.end(); A.hasCurrent(); A.prev()){
//        s = A.currentKey();
//        x = A.currentVal();
//        cout << "("+s+", " << x << ") ";
//        ++i;
//        if (i == 15)
//            break;
//    }

    return 0;
}

int clientTestParts(){
    string S[] = {"one", "two", "foo", "bar", "happy", "is", "three", "par"};

    Dictionary A;
    Dictionary B;

    cout << endl;

    // insert some pairs into A
    for(int i=0; i<8; i++){
        A.setValue(S[i], i+1);
    }

    // call operator=()
    B = A;

    cout << "A.size() = " << A.size() << endl  << A << endl;
    cout << "B.size() = " << B.size() << endl  << B << endl;

    B.setValue("bar", 9);

    // call copy constructor
    Dictionary C = B;

    cout << "B.size() = " << B.size() << endl  << B << endl;
    cout << "C.size() = " << C.size() << endl  << C << endl;

    // check operator==()
    cout << "A==B is " << (A==B?"true":"false") << endl;
    cout << "B==C is " << (B==C?"true":"false") << endl;
    cout << "C==A is " << (C==A?"true":"false") << endl << endl;



    // perform alterations on A
    cout << A.getValue("happy") << endl;
    A.getValue("happy") *= 10; // change the value associated with "happy"
    cout << A.getValue("happy") << endl << endl;

    A.remove("one");
    A.remove("two");

    // check state of A

    cout << "A.size() = " << A.size() << endl  << A << endl;
    cout << A.pre_string() << endl;

    // do forward iteration on B
//    for(B.begin(); B.hasCurrent(); B.next()){
//        s = B.currentKey();
//        x = B.currentVal();
//        cout << "("+s+", " << x << ") ";
//    }
    return 0;
}

int NonEmptyClear(){
    Dictionary A = Dictionary();
    A.setValue("c", 3);
    A.setValue("b", 2);
    A.setValue("a", 1);
    A.begin();
    A.clear();
    std::cout << A << " A\n";
    if (A.size() != 0) {
        return 1;
    }
    try {
        A.currentKey();
        return 2;
    } catch (logic_error const &) {
    }
    return 0;
}

int Begin_value(){
    Dictionary A = Dictionary();
    A.setValue("d", 1);
    A.setValue("b", 5);
    A.setValue("c", 16);
    A.setValue("f", 176);
    A.setValue("e", 3214);

    A.begin();
    if (A.currentVal() != 5 || A.currentKey() != "b")
        return 1;
    A.remove("b");
    A.begin();
    if (A.currentVal() != 16 || A.currentKey() != "c")
        return 2;
    return 0;
}

int main(){
    //dicInsertion();
    //equalsTest();
    //copyTest();
    //removeTest();
    //clientTestParts();
//    int i = NonEmptyClear();
//    printf("i = %d\n", i);
    int p = Begin_value();
    printf("p = %d\n", p);
    return 0;
}