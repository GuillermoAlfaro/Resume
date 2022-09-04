#include<iostream>
#include <sstream>
#include<string>
#include <stdexcept>
#include <cmath>
#include <cstdlib>
#include "List.h"
#include "BigInteger.h"

#define base 1000000000
#define power 9


// Class Constructors & Destructors ----------------------------------------

BigInteger::BigInteger(){
    signum = 0;
    digits = List();
}

BigInteger::BigInteger(std::string s){
    if ((s.empty())){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    if (s[0] == '-'){
        signum = -1;
        s.erase(0, 1);
    } else if (s[0] == '+'){
        signum = 1;
        s.erase(0, 1);

    } else if (s[0] == '0'){
        for (char& c : s) {
            if (c != '0'){
                goto JUMP;
            }
        }
        signum = 0;
    } else if (!std::isdigit(s[0])) {
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
    } else {
        JUMP: signum = 1;
    }
    long ceil = s.length()/power + (s.length() % power != 0);
    int remainder = s.length() - power*(s.length()/power);

    if (s.length() <= power)
        remainder = s.length();
    if (remainder == 0){
        remainder = power;
    }
    for (int i = 0; i < ceil; ++i) {
        if (i == 0){
            std::string dd = s.substr(0, remainder);
            ListElement G = std::stol(dd, nullptr, 10);
            if (G == 0){
                continue;
            }
            digits.insertBefore(G);
            continue;
        }
        int T = remainder+power*(i-1);
        std::string dd;
        dd = s.substr(T, power);
        ListElement G = std::stol(dd, nullptr, 10);
        if (digits.length()==0 && G == 0){
            continue;
        }
        digits.insertBefore(G);
    }
}

BigInteger::BigInteger(const BigInteger& N){
    signum = N.signum;
    List L = N.digits;
    if (L.length() == 0){
        return;
    }
    L.moveFront();
    for (int i = 0; i < L.length(); ++i) {
        ListElement G = L.moveNext();
        digits.insertBefore(G);
    }
}


// Access functions --------------------------------------------------------

int BigInteger::sign() const {
    return signum;
}

int BigInteger::compare(const BigInteger& N) const {
    if (this == &N){
        return 0;
    }
    List nList = N.digits;
    List tList = digits;
    if (digits.length() > nList.length()){
        return 1;
    }
    if (digits.length() < nList.length()){
        return -1;
    }
    if (signum > N.signum){
        return 1;
    }
    if (signum < N.signum){
        return -1;
    }
    if (signum == N.signum && signum == 0){
        return 0;
    }
    nList.moveFront();
    tList.moveFront();
    for (int i = 0; i < nList.length(); ++i) {
        ListElement T = tList.moveNext();
        ListElement NN = nList.moveNext();
        if (T > NN){
            if (signum == 1){
                return 1;
            } else {
                return -1;
            }
        }
        if (T < NN){
            if (signum == 1){
                return -1;
            }
            return 1;
        }
    }
    return 0;


}


// Manipulation procedures -------------------------------------------------

void BigInteger::makeZero(){
    signum = 0;
    digits.clear();
}

void BigInteger::negate() {
    if (signum == 0){
        return;
    }
    if (signum == 1){
        signum = -1;
        return;
    } else if (signum == -1){
        signum = 1;
        return;
    }
    return;
}


// Helper functions --------------------------------------------------------

ListElement normalizeList(List& L){
    L.moveBack();
    ListElement overFlow;
    for (int i = 1; i < L.length()+1; ++i) {
        ListElement H = L.movePrev();
        overFlow = H/base;
        if (overFlow > 0 && L.position()!=0){
            ListElement I = L.peekPrev();
            L.setBefore(I+overFlow);
            L.setAfter(H-base*overFlow);
        }
        if (overFlow > 0 && L.position()==0){
            L.insertBefore(overFlow);
            L.setAfter(H-base*overFlow);
        }
//        if (overFlow == 0 && L.position()!=0 && H < 0){
//            ListElement I = L.peekPrev();
//            L.setBefore(I-1);
//            L.setAfter(base+H);
//        }
    }
    return overFlow;
}

ListElement digitsCompare(List& A, List& B){
    if (A.length() > B.length()){
        return 1;
    }
    if (A.length() < B.length()){
        return -1;
    }
    A.moveFront();
    B.moveFront();
    for (int i = 0; i < A.length(); ++i) {
        ListElement AA = A.moveNext();
        ListElement BB = B.moveNext();
        if (AA > BB){
            return 1;
        }
        if (AA < BB){
            return -1;
        }
    }
    return 0;
}

void negateList(List& L){
    L.moveFront();
    for (int i = 0; i < L.length(); ++i) {
        ListElement G = L.moveNext();
        L.setBefore(-G);
    }
}

void shiftList(List& L, int p){
    for (int i = 0; i < p; ++i) {
        L.insertBefore(0);
    }
}

void sumList(List& S, List A, List B, int sgn){
    List AA = A;
    S.clear();
    int len;
    if (AA.length() > B.length()){
        len = AA.length();
    } else {
        len = B.length();
    }
    if (AA.length()!=0){
        AA.moveBack();
    }
    if (B.length()!=0){
        B.moveBack();
    }
    for (int i = 0; i < len; ++i) {
        ListElement aElement;
        ListElement bElement;
        if (AA.position() != 0){
            aElement = AA.movePrev();
        } else {
            aElement = 0;
        }
        if (B.position() != 0){
            bElement = B.movePrev();
        } else {
            bElement = 0;
        }
        S.insertAfter(aElement + (bElement * sgn));
    }

}

std::string List_to_string(List& digits, char g) {
    std::string s = "";
    List tList = digits;
    tList.moveFront();

    s += g;
    for (int i = 0; i < tList.length(); ++i) {
        ListElement G = tList.moveNext();
        if (G < 0){
            G = -G;
        }
        int digits = log10(G) + 1;
        int absOfDMN = abs(digits - power);
        if (G == 0){
            absOfDMN = power-1;
        }
        for (int j = 0; j < absOfDMN; ++j) {
            s += "0";
        }
        s += std::to_string(G);
    }
    return s;
}


// BigInteger Arithmetic operations ----------------------------------------

BigInteger BigInteger::add(const BigInteger& N) const{
    List L = List();
    List tList = digits;
    List nList = N.digits;
    int len;
    if (tList.length() > nList.length())
        len = tList.length();
    else
        len = nList.length();
    if (signum == 0){
        return N;
    }
    if (N.signum == 0){
        return *this;
    }
    bool negatedN = false;
    bool negatedT = false;
    if (N.signum == -1){
        negateList(nList);
        negatedN = true;
    }
    if (signum == -1){
        negateList(tList);
        negatedT = true;
    }
    tList.moveBack();
    nList.moveBack();
    for (int i = 0; i < len; ++i) {
        ListElement T;
        ListElement Nn;
        if (tList.position() != 0){
            T = tList.movePrev();
        } else {
            T = 0;
        }
        if (nList.position() != 0){
            Nn = nList.movePrev();
        } else {
            Nn = 0;
        }
        L.insertAfter(T+Nn);
    }
    if (negatedN){
        negateList(nList);
    }
    if (negatedT){
        negateList(tList);
    }
    normalizeList(L);
    std::string s;
    if (signum == -1 && N.signum == -1){
        s = List_to_string(L, '-');

    } else {
        s = List_to_string(L, '+');
    }
    BigInteger B = BigInteger(s);
    if (signum == -N.signum){
        int comp = digitsCompare(tList, nList);
        if (comp == 0){
            B.signum = 0;
        } else if (comp == 1){
            B.signum = 1;
        } else {
            B.signum = -1;
        }
    }
    return B;
}

BigInteger BigInteger::sub(const BigInteger& N) const {
    List nList = N.digits;
    List tList = digits;
    BigInteger T = *this;
    T.signum = signum;
    BigInteger copyN = N;
    copyN.signum = -N.signum;
    BigInteger result = T.add(copyN);
    if (this->compare(N) == -1){
        result.signum = -1;
    }
    if (signum == N.signum){
        List tList = digits;
        int comp = digitsCompare(tList, nList);
        if (comp == 0){
            result.signum = 0;
        } else if (comp == -1){
            result.signum = -1;
        } else {
            result.signum = 1;
        }
    }


    return result;
}

BigInteger BigInteger::mult(const BigInteger& N) const{
    List L = List();
    List running_total = List();
    List tList = digits;
    List nList = N.digits;
    bool nLever = false;
    bool bLever = false;
    if (N.signum == -1){    // B List negated
        //negateList(nList);
        nLever = true;
    }
    if (signum == -1){      // A List negated
        //negateList(tList);
        bLever = true;
    }
    if (N.signum == 0 || signum == 0){
        BigInteger zero = BigInteger("0");
        return zero;
    }
    for (int i = 1; i < tList.length()+1; ++i) {
        ListElement tElement = tList.movePrev();
        for (int j = 0; j < nList.length(); ++j) {
            ListElement nElement = nList.movePrev();
            L.insertAfter(tElement*nElement);
        }
        normalizeList(L);
        sumList(running_total, running_total, L, 1);
        normalizeList(running_total);
        L.clear();
        shiftList(L, i);
        L.moveFront();
        nList.moveBack();
    }
    std::string s;
    if ((!bLever && !nLever) || (bLever && nLever)){
        s = List_to_string(running_total, '+');
    } else {
        s = List_to_string(running_total, '-');
    }
    BigInteger ret = BigInteger(s);


    return ret;
}


// Other Functions ---------------------------------------------------------

std::string BigInteger::to_string() {
    std::string s = "";
    List tList = digits;
    if (tList.length() == 0){
        signum = 0;
        s += "0";
        return s;
    }
    tList.moveFront();
    if (signum == -1){
        s += "-";
    } else if (signum == 0){
        s += "0";
        return s;
    }
    for (int i = 0; i < tList.length(); ++i) {
        ListElement G = tList.moveNext();
        int digits = log10(G) + 1;
        int absOfDMN = abs(digits - power);
        if (G == 0){
            absOfDMN = power - 1;
        }
        if (tList.position() != 1){
            for (int j = 0; j < absOfDMN; ++j) {
                s += "0";
            }
        }
        s += std::to_string(G);
    }

    return s;
}

std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream << N.to_string();
}

bool operator==( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0){
        return true;
    } else {
        return false;
    }
}

bool operator<( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1){
        return true;
    } else {
        return false;
    }
}

bool operator<=( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1 || A.compare(B) == 0)
        return true;
    else
        return false;
}

bool operator>( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1)
        return true;
    else
        return false;
}

bool operator>=( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1 || A.compare(B) == 0)
        return true;
    else
        return false;
}

BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
}

BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    BigInteger G = A.add(B);
    A = G;
    return A;
}

BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
}

BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    int comp = A.compare(B);
    A = A.sub(B);
    A.signum = comp;
    return A;
}

BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
}

BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A.mult(B);
    return A;
}