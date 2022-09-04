#include<iostream>
#include"List.h"

List::Node::Node(ListElement x){
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Creates new List
List::List(){
    pos_cursor = 0;
    num_elements = 0;
    beforeCursor = frontDummy = new Node(-2147483647);
    afterCursor = backDummy = new Node(2147483647);

}

// Takes in address of List and Copies it.
List::List(const List& L){
    frontDummy = new Node(-2147483647);
    backDummy = new Node(2147483647);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;

    Node* N;
    for(N = L.frontDummy->next; N != L.backDummy; N=N->next){
        if (N == NULL){
            break;
        }
        insertBefore(N->data);
    }
    num_elements = L.num_elements;
}

// Destroys List
List::~List(){
    if (num_elements == 0){
        delete(frontDummy);
        delete(backDummy);
        return;
    }
    moveBack();
    Node*N;
    Node *temp;
    for (N =backDummy; N!=frontDummy; ) {
        temp = N->prev;
        delete(N);
        N=temp;
    }
    delete(frontDummy);
}

int List::length() const{
    return num_elements;
}

ListElement List::front() const{
    if (num_elements==0){
        throw std::length_error("List: front(): empty list");
    }
    return(frontDummy->next->data);
}

ListElement List::back() const{
    if (num_elements==0){
        throw std::length_error("List: back(): empty list");
    }
    return(backDummy->prev->data);
}

int List::position() const{
    return(pos_cursor);
}

ListElement List::peekNext() const{
    if (pos_cursor==num_elements){
        throw std::range_error("List: peekNext(): cursor at back");
    }
    return(afterCursor->data);
}

ListElement List::peekPrev() const {
    if (pos_cursor==0){
        throw std::range_error("List: peekPrev(): cursor at front");
    }
    return(beforeCursor->data);
}

void List::clear(){
    if (num_elements == 0){
        return;
    }
    moveFront();
    int len = length();
    for (int i = 0; i < len; ++i) {
        eraseAfter();
    }
}

void List::moveFront() {
    if (num_elements == 0){
        throw std::length_error("List: front(): empty list");
    }
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
}

void List::moveBack(){
    if (num_elements == 0){
        throw std::length_error("List: back(): empty list");
    }
    pos_cursor = length();
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
}

ListElement List::moveNext() {
    if (pos_cursor == length()+1){
        throw std::range_error("List: moveNext(): cursor at back");
    }
    if (pos_cursor < length()){
        beforeCursor = afterCursor;
        afterCursor = beforeCursor->next;
        pos_cursor += 1;
        return beforeCursor->data;
    } else {
        return -2147483647;
    }
}

ListElement List::movePrev() {
    if (beforeCursor == frontDummy){
        throw std::range_error("List: movePrev(): cursor at front");
    }
    if (pos_cursor <= length()){
        afterCursor = beforeCursor;
        beforeCursor = afterCursor->prev;
        pos_cursor -= 1;
        return afterCursor->data;
    }
    return 2147483647;
}

void List::insertAfter(ListElement x) {
    num_elements += 1;
    Node* N = new Node(x);
    beforeCursor->next = N;
    N->prev = beforeCursor;
    afterCursor->prev = N;
    N->next = afterCursor;
    afterCursor = N;
}

void List::insertBefore(ListElement x) {
    num_elements += 1;
    pos_cursor += 1;
    Node *N = new Node(x);

    beforeCursor->next = N;
    N->prev = beforeCursor;
    afterCursor->prev = N;
    N->next = afterCursor;
    beforeCursor = N;
}

void List::setAfter(ListElement x){
    if (afterCursor == backDummy){
        throw std::range_error("List: setAfter(): cursor at back");
    }
    if (pos_cursor<length()){
        afterCursor->data = x;
    }
}

void List::setBefore(ListElement x){
    if (beforeCursor == frontDummy){
        throw std::range_error("List: setBefore(): cursor at front");
    }
    if (pos_cursor<=length()){
        beforeCursor->data = x;
    }
}

void List::eraseAfter(){
    if (afterCursor == backDummy){
        throw std::range_error("List: eraseAfter(): cursor at back");
    }
    Node *newAfter = afterCursor->next;
    newAfter->prev = beforeCursor;
    beforeCursor->next = newAfter;
    afterCursor = newAfter;
    num_elements -= 1;
}

void List::eraseBefore(){
    if (beforeCursor == frontDummy){
        throw std::range_error("List: eraseBefore(): cursor at front");
    }
    beforeCursor->prev->next = afterCursor;
    afterCursor->prev = beforeCursor->prev;
    beforeCursor = beforeCursor->prev;
    num_elements -= 1;
    pos_cursor -= 1;
}

int List::findNext(ListElement x){
    ListElement G;
    while (pos_cursor < length()){
        try{
            G = moveNext();
        }catch(std::range_error const&){
            return -1;
        }
        if (G == x){
            return pos_cursor;
        }
    }

    return -1;
}

int List::findPrev(ListElement x){
    ListElement G;
    while (pos_cursor >= 0){
        try{
            G = movePrev();
        }catch(std::range_error const&){
            return -1;
        }

        if (G == x){
            return pos_cursor;
        }
    }
    moveFront();
    return -1;
}


void List::cleanup(){
    int len = length();
    int OGPos = pos_cursor;
    int pos = pos_cursor;
    for (int i = 0; i < len+1; ++i) {
        moveFront();
        ListElement G;
        for (int j = 0; j < pos; ++j) {
            G = moveNext();
        }
        if (i == len){
            break;
        }
        while (pos_cursor < length()){
            ListElement next = moveNext();
            if (next == G){
                eraseBefore();
            }
        }
        if (pos == length()){
            pos = 0;
        }
        pos += 1;
    }
    if (OGPos == 0){
        moveFront();
    }
}

List List::concat(const List &L) const{
    List newList = List();
    Node* N;
    for(N = backDummy->prev; N != frontDummy; N = N->prev){
        newList.insertAfter(N->data);
    }
    newList.moveBack();
    for (N = L.backDummy->prev; N != L.frontDummy; N = N->prev){
        newList.insertAfter(N->data);
    }
    newList.moveFront();
    return newList;
}

std::string List::to_string() const{
    std::string s = "(";
    Node* N;

    for (N=frontDummy->next; N!=backDummy; N=N->next){
        s += std::to_string(N->data);
        if (N->next == backDummy){
            break;
        }
        s += ", ";
    }
    s += ")";
    return s;
}

bool List::equals(const List &R) const {
    if (length() != R.length()){
        return false;
    }
    Node *N;
    Node *H = R.frontDummy;
    for (N = frontDummy->next; N != backDummy; N = N->next){
        H = H->next;
        if (H->data != N->data){
            return false;
        }
    }
    return true;
}

std::ostream& operator<<( std::ostream& stream, const List& L ){
    return stream << L.to_string();
}

bool operator==( const List& A, const List& B ){
    return A.equals(B);
}

List& List::operator=( const List& L ){
    if( this != &L ){
        List temp = L;
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }
    return *this;
}
