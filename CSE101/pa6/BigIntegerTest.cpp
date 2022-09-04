#include "BigInteger.h"
#include "List.h"

#define base 1000000000
#define power 9


int stringTest(){
    std::string s = "-652348563075823";
    printf("First character = %c\n", s[0]);
    std::string dd = s.substr(1,9);
    ListElement G = std::stol(s, nullptr, 10);
    ListElement D = std::stol(dd, nullptr, 10);
    printf("G = %ld\n", G);
    printf("D = %ld\n", D);
    return 0;
}

int bigIN(){
    BigInteger G = BigInteger("6523485630758234007488392857982374523487612398700554");
    BigInteger B = G;
    std::cout << (G==B) << '\n';

    std::cout << G;


    return 0;
}

int addTest(){
    BigInteger B = BigInteger("999999999");
    BigInteger G = BigInteger("9999999999");
    BigInteger BG = B.add(G);
    std::cout << BG;
    return 0;
}

int listTest(){
    List L = List();
    L.insertBefore(5);

    printf("L position = %d\n", L.position());

    return 0;
}

int testNegate(){
    List L = List();

    L.insertBefore(8);
    L.insertBefore(10);
    L.insertBefore(-20);

    L.moveFront();
    for (int i = 0; i < L.length(); ++i) {
        ListElement G = L.moveNext();
        L.setBefore(-G);
    }

    std::cout << L << " L negated\n";

    return 0;
}

int subTest(){
    BigInteger B = BigInteger("999999999");
    BigInteger G = BigInteger("999999999");

    BigInteger BplusG = B.add(G);
    std::cout << BplusG << " B plus G\n";
    BigInteger BminG = B.sub(G);
    std::cout << BminG << " B minus G\n";
    return 0;
}

int mulTest(){
    BigInteger B = BigInteger("9163574346363325007361058");
    BigInteger G = BigInteger("4597814412658653960738664");

    BigInteger BtimesG = B.mult(G);
    std::cout << BtimesG << " <---B * G\n";

    return 0;
}

int bignegtest(){
    BigInteger B = BigInteger("-0041085449");
    std::cout << B << " Big int test\n";
    return 0;
}

int mulTestMixSign(){
    BigInteger A = BigInteger("123456");
    BigInteger B = BigInteger("-123456");

    BigInteger AB = A.mult(B);
    std::cout << AB << "\n";


    return 0;
}

int operatorTest(){
    BigInteger A = BigInteger("0");
    BigInteger B = BigInteger("500");
    std::cout << A+B << " A + B\n";
    std::cout << A-B << " A - B\n";
    std::cout << A*B << " A * B\n";


    return 0;
}

int truncateprob(){
    BigInteger A = BigInteger("9063574346363325007361058");
    BigInteger B = BigInteger("+4597814412658653960738664");

    std::cout << B << " <-- B\n";
    std::cout << A-B << " MY A - B \n";
    printf("4465759933704671046622394 CORECCT ANSWER\n\n");

    return 0;
}

int lineTest(){
BigInteger A, B, C, D;

/*
     * Adding numbers fall into one of 4 cases, denote pos = positive number,
     * neg = negative number
     *
     * pos + pos = pos
     *
     * pos + neg = 0
     *           < 0
     *           > 0
     *
     * neg + pos = 0
     *           < 0
     *           > 0
     *
     * neg + neg = neg
     */
    A = BigInteger("+111122223333");
    B = BigInteger("+222211110000");

    // pos + pos = pos
    D = BigInteger("+333333333333");
    C = A + B;

    if (!(C == D))
        return 1;

    // add a positive and a negative integer
    //-> pos + neg = 0
    B = BigInteger("-111122223333");
    C = A + B;
    if (C.sign() != 0)
        return 2;

    //-> pos + neg > 0
    B = BigInteger("-110122223333");
    D = BigInteger("1000000000");
    C = A + B;
    if (C.sign() != 1)
        return 31;
    if (!(C == D))
        return 32;

    //-> pos + neg < 0
    B = BigInteger("-112122223333");
    D = BigInteger("-1000000000");
    C = A + B;

    if (C.sign() != -1)
        return 41;
    if (!(C == D))
        return 42;

    //-> neg + neg = neg
    A = BigInteger("-221211110000");
    D = BigInteger("-333333333333");
    C = A + B;
//    std::cout << A << " A \n";
//    std::cout << B << " B\n";
    std::cout << C << " C\n";
    std::cout << D << " D\n";
    printf("!(C == D) = %d\n", !(C == D));
    if (!(C == D))
        return 5;

    return 0;
}

int addAssigntest(){
    BigInteger A, B, C, D;
    A = BigInteger("-221211110000");
    B = BigInteger("-112122223333");
    D = BigInteger("-333333333333");
    A += B;

    if (!(A == D))
        return 5;

    return 0;
}

int subAssignTest(){
    BigInteger A, B, C, D;
    A = BigInteger("-111122223333");
    B = BigInteger("-112122223333");
    D = BigInteger("1000000000");
    A -= B;
    if (A.sign() != 1)
        return 21;
    if (!(A == D))
        return 22;

    // neg - neg < 0
    A = BigInteger("-111122223333");
    B = BigInteger("-110122223333");
    D = BigInteger("-1000000000");
    A -= B;

    if (A.sign() != -1)
        return 31;
    if (!(A == D))
        return 32;

    // neg - pos = neg
    A = BigInteger("-111122223333");
    B.negate();
    D = BigInteger("-221244446666");
    A -= B;

    if (A.sign() != -1)
        return 41;
    if (!(A == D))
        return 42;
    return 0;
}






int main(){
    //stringTest();
    //bigIN();
    //addTest();
    //listTest();
    //testNegate();
    //subTest();
    //mulTest();
    //bignegtest();
    //mulTestMixSign();
    //operatorTest();
    //truncateprob();
    int num = subAssignTest();
	if (num != 0) {
		printf("%d was returned\n", num);
}

}
