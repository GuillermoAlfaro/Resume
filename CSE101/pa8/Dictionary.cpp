//************************************************
// Guillermo Alfaro
// Cruz ID: gualfaro
// PA8
//************************************************
#include "Dictionary.h"

#define NIL -2147483647
#define RED 1
#define BLACK 0
using namespace std;

Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = BLACK;
}

// Creates new dictionary
Dictionary::Dictionary() {
    current = root = nil = new Node("NIL", NIL);
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
    current = root = nil = new Node("NIL", NIL);
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary(){
    if (num_pairs == 0){
        delete(nil);
        return;
    }
    int num_save = num_pairs;
    Node *a;
    for (int i = 0; i < num_save; ++i) {
        a = findMin(root);
        if (a == root){
            a = findMax(root);
        }
        if (a->key == "NIL" && a->val == NIL){
            break;
        }
        //std::cout << "Node " << a->key << " : " << a->val << " was deleted\n";
        postOrderDelete(a);
        --num_pairs;
    }
    //std::cout << "END of dic\n\n";
    delete(nil);
}

// Helper functions -------------------------------------------------------

void Dictionary::inOrderString(std::string& s, Node* R) const{
    if (R != nil){
        inOrderString(s, R->left);
        s.append(R->key);
        s.append(" : ");
        s.append(::to_string(R->val));
        s.append("\n");
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderString(std::string& s, Node* R) const{
    if (R != nil){
        s += R->key + "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node *R, Node *N) {
    if (R != N){
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::postOrderDelete(Node* z){
    if (z->left == nil) {                 // case 1 or case 2.1 (right only)
        Transplant(z, z->right);
    }
    else if (z->right == nil)           // case 2.2 (left only)
        Transplant(z, z->left);
    else {                              // case 3
        Node *y = findMin(z->right);
        if (y->parent != z){
            Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    delete(z);
}

void Dictionary::Transplant(Node* u, Node* v){
    if (u->parent == nil) {               // if root is being deleted
        root = v;
    }
    else if (u == u->parent->left) {      // if a left child is being deleted
        u->parent->left = v;
    }
    else                                // else a right child is being deleted
        u->parent->right = v;
    if (v != nil) {                       // if v isn't nil
        v->parent = u->parent;
    }
}

Dictionary::Node* Dictionary::searchKey(Node* R, keyType k) const{
    if (R == nil || k == R->key)
        return R;
    else if (k < R->key)
        return searchKey(R->left, k);
    else
        return searchKey(R->right, k);

}

Dictionary::Node* Dictionary::searchVal(Node* R, valType v) const{
    if (R == nil || v == R->val){
        return R;
    } else {
        searchVal(R->left, v);
        searchVal(R->right, v);
    }
    return R;
}

Dictionary::Node* Dictionary::findMin(Node *X) {
    if (X->key == "NIL" && X->val == NIL){
        return X;
    }
    while (X->left != nil){
        X = X->left;
    }
    return X;
}

Dictionary::Node* Dictionary::findMax(Node *X) {
    if (X->key == "NIL" && X->val == NIL){
        return X;
    }
    while (X->right != nil){
        X = X->right;
    }
    return X;
}

Dictionary::Node* Dictionary::findNext(Node *X) {
    if (X->right != nil){
        return findMin(X->right);
    }
    Node* Y = X->parent;
    while (Y != nil && X == Y->right){
        X = Y;
        Y = Y->parent;
    }
    return Y;
}
// Figuring out why findPrev doesn't work but findNext Works
Dictionary::Node* Dictionary::findPrev(Node *X){
    if (X->left != nil){
        return findMax(X->left);
    }
    Node* Y = X->parent;
    while (Y != nil && X == Y->left){
        X = Y;
        Y = Y->parent;
    }
    return Y;
}


// RBT Helper Functions ---------------------------------------------------

void Dictionary::LeftRotate(Node* x){
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void Dictionary::RightRotate(Node *x) {
    Node* y = x->left;

    x->left = y->right;
    if (y->right != nil)
        y->right->parent = x;

    y->parent = x->parent;
    if (x->parent == nil)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

void Dictionary::RB_Insert(Node *z) {
    Node *x, *y = nil;
    x = root;
    while (x != nil){
        y = x;
        if (z->key == x->key){
            x->val = z->val;

            return;
        }
        if (z->key < x->key){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == nil){
        root = z;
    } else if (z->key < y->key){
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = RED;
    RB_InsertFixUp(z);
    ++num_pairs;
}

void Dictionary::RB_InsertFixUp(Node *z) {
    Node *y;
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else{
                if (z == z->parent->right){
                    z = z->parent;
                    LeftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left){
                    z = z->parent;
                    RightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node *u, Node *v) {
    if (u->parent == nil){
        root = v;
    } else if (u == u->parent->left){
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void Dictionary::RB_Delete(Node *z) {
    Node *x, *y = z;
    int OG_color = y->color;
    if (z->left == nil){
        x = z->right;
        RB_Transplant(z, z->right);
    } else if (z->right == nil){
        x = z->left;
        RB_Transplant(z, z->left);
    } else {
        y = findMin(z->right);
        OG_color = y->color;
        x = y->right;
        if (y->parent == z){
            x->parent = y;
        } else {
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (OG_color == BLACK){
        RB_DeleteFixUp(x);
    }
}

void Dictionary::RB_DeleteFixUp(Node *x) {
    Node *w;
    while (x != root && x->color == BLACK){
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(x->parent);
                x = root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}


// Access functions -------------------------------------------------------

int Dictionary::size() const {
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    if (nil != searchKey(root, k))
        return true;
    else
        return false;
}

valType& Dictionary::getValue(keyType k) const{
    Node* G = searchKey(root, k);
    if (G->key == "NIL" && G->val == NIL){
        throw std::logic_error("getValue(): key does not exist");
    }
    return G->val;
}

bool Dictionary::hasCurrent() const {
    if (current->key != "NIL" && current->val != NIL)
        return true;
    else
        return false;
}

keyType Dictionary::currentKey() const {
    if (hasCurrent())
        return current->key;
    else
        throw logic_error("currentKey(): current undefined\n");
}

valType& Dictionary::currentVal() const {
    if (hasCurrent())
        return current->val;
    else
        throw logic_error("currentVal(): current undefined\n");
}

// Manipulation procedures ------------------------------------------------

void Dictionary::clear(){
    int j = num_pairs;
    for (int i = 0; i < j; ++i) {
        Node *z = findMax(root);
        RB_Delete(z);
        --num_pairs;
    }
    current = nil;
    root = nil;
}

void Dictionary::setValue(keyType k, valType v){
    Node *z = new Node(k, v);
    z->parent = nil;
    z->right = nil;
    z->left = nil;
    RB_Insert(z);
}

void Dictionary::remove(keyType k) {
    if (contains(k)){
        Node *R;
        R = searchKey(root, k);
        if (R->key != "NIL" && R->val != NIL){
            if (current->key == k){
                current = nil;
            }
            RB_Delete(R);
            delete(R);
            --num_pairs;
        }
    }
    //throw std::logic_error("remove() key does not exist\n");
}

void Dictionary::begin() {
    current = findMin(root);
}

void Dictionary::end(){
    current = findMax(root);
}

void Dictionary::next(){
    if (hasCurrent()){
        if (findMax(root) != current)
            current = findNext(current);
        else
            current = nil;
    }

}

void Dictionary::prev() {
    if (hasCurrent()){
        if (findMin(root) != current)
            current = findPrev(current);
        else
            current = nil;
    }

}

// Other Functions -------------------------------------------------------

std::string Dictionary::to_string() const {
    std::string s;
    inOrderString(s, root);
    return s;
}

std::string Dictionary::pre_string() const {
    std::string s;
    preOrderString(s, root);
    return s;
}

bool Dictionary::equals(const Dictionary &D) const {
    if (num_pairs != D.num_pairs){
        return false;
    }
    std::string a, b;
    a = this->to_string();
    b = D.to_string();
    if (a == b){
        return true;
    }
    return false;
}

// Overloaded Operators --------------------------------------------------

std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary &D) {
    if (this != &D){    // not self assignment
        // make a copy of D
        Dictionary temp = D;

        // then swap the copy's fields with fields of this
        std::swap(nil, temp.nil);
        std::swap(current, temp.current);
        std::swap(root, temp.root);
        std::swap(num_pairs, temp.num_pairs);
    }

    // return with the new data installed
    return *this;

    // the copy, if there is one, is deleted upon return
}