//
//  main.cpp
//  AVL Tree
//
//  Created by Ira Xavier Porchia on 6/8/20.
//  Copyright © 2020 Ira Xavier Porchia. All rights reserved.
//
#include <iostream>
using namespace std;

class node;
class AVL;
const int MAX = 100;

template <class T>
class Queue{
private:
    int _head;
    int _tail;
public:
    T _data[MAX];

    Queue():
    _head(0), _tail(0){}

    bool isEmpty(){
        return _tail == _head;
    }

    bool isFull(){
        return (_tail + 1) % MAX == _head;
    }

    T dequeue(){
        if (isEmpty()) {
            return 0;
        }
        T i = _data[_head];
        _head = (_head + 1) % MAX;
        return i;
    }

    void enqueue(node* value){
        if (isFull()) {
            return;
        }
        _data[_tail] = value;
        _tail = (_tail + 1) % MAX;
    }

    T head(){
        return _data[_head];
    }

};

struct Trunk
{
    Trunk *prev;
    string str;

    Trunk(Trunk *prev, string str)
    {
        this->prev = prev;
        this->str = str;
    }
};

// Helper function to print branches of the binary tree
void showTrunks(Trunk *p)
{
    if (p == nullptr)
        return;

    showTrunks(p->prev);

    cout << p->str;
}

class node{
    friend class AVL;
private:
    int _data;
    node *_left;
    node *_right;
    int _height;
public:
    node(int data):
    _data(data), _left(nullptr), _right(nullptr), _height(1){}
};

class AVL{
private:
    node *_root;
public:
    AVL():
    _root(nullptr){}
    ~AVL(){
        if(_root == nullptr){
            return;
        }
        if(_root->_left != nullptr){
            delete _root->_left;
            _root->_left = nullptr;
        }
        if(_root->_right != nullptr){
            delete _root->_right;
            _root->_right = nullptr;
        }
        delete _root;
        _root = nullptr;
    }
    
    int height(node *root);
    int max(int a, int b);
    node *rightRotate(node *root);
    node *leftRotate(node *root);
    int getBalance(node *root);
    node *insert(int key);
    node *insertUtil(node *root, int key);
    void preOrder(node *root);
    void inOrder(node *root);
    void postOrder(node *root);
    void levelOrder(node *root);
    void printTree(node *root, Trunk *prev, bool isLeft);
    node *search(node *root, int data);
    node *root();
    node *minElement(node *root);
    void deleteMin(node *root);
    node *Delete(node *root, int data);
};

int AVL::max(int a, int b){
    return(a > b)? a : b;
}

int AVL::height(node *root){
    if(root == nullptr){
        return 0;
    }
    return root->_height;
}

node *AVL::rightRotate(node *root){
    node *x = root->_left;
    node *T2 = x->_right;
    x->_right = root;
    root->_left = T2;
    root->_height = max(height(root->_left), height(root->_right)) + 1;
    x->_height = max(height(x->_left), height(x->_right)) + 1;
    return x;
}

node *AVL::leftRotate(node *root){
    node *y = root->_right;
    node *T2 = y->_left;
    y->_left = root;
    root->_right = T2;
    root->_height = max(height(root->_left), height(root->_right)) + 1;
    y->_height = max(height(y->_left), height(y->_right)) + 1;
    return y;
}

int AVL::getBalance(node *root){
    if(root == nullptr){
        return 0;
    }
    return height(root->_left) - height(root->_right);
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
node *AVL::insert(int key){
    return insertUtil(_root, key);
}

node *AVL::insertUtil(node *root, int key){
    /* 1. Perform the normal BST insertion */
    if(root == nullptr){
        root = new node(key);
        _root = root;
        return root;
    }
    
    if(key < root->_data){
        root->_left = insertUtil(root->_left, key);
    }else if(key > root->_data){
        root->_right = insertUtil(root->_right, key);
    }else{
        return root;
    }
    
    /* 2. Update height of this ancestor node */
    root->_height = 1 + max(height(root->_left), height(root->_right));
    
    /* 3. Get the balance factor of this ancestor
    node to check whether this node became
    unbalanced */
    int balance = getBalance(root);
    
    // If this node becomes unbalanced, then
       // there are 4 cases
    
    // Left Left Case
      if (balance > 1 && key < root->_left->_data)
          return rightRotate(root);
    
      // Right Right Case
      if (balance < -1 && key > root->_right->_data)
          return leftRotate(root);
    
      // Left Right Case
      if (balance > 1 && key > root->_left->_data)
      {
          root->_left = leftRotate(root->_left);
          return rightRotate(root);
      }  
    
      // Right Left Case
      if (balance < -1 && key < root->_right->_data)
      {
          root->_right = rightRotate(root->_right);
          return leftRotate(root);
      }
    
    _root = root;
    return root;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void AVL::preOrder(node *root){
    if(root != nullptr){
        cout << root->_data << " ";
        preOrder(root->_left);
        preOrder(root->_right);
    }
}

void AVL::inOrder(node *root){
    if(root != nullptr){
        inOrder(root->_left);
        cout << root->_data << " ";
        inOrder(root->_right);
    }
}

void AVL::postOrder(node *root){
    if(root != nullptr){
        postOrder(root->_left);
        postOrder(root->_right);
        cout << root->_data << " ";
    }
}

void AVL::levelOrder(node *root){
    if(root == nullptr){
        return;
    }
    Queue<node *> queue;
    queue.enqueue(root);
    while(!queue.isEmpty()){
        node *presentNode = queue.dequeue();
        cout << presentNode->_data << " ";
        if(presentNode->_left != nullptr){
            queue.enqueue(presentNode->_left);
        }
        if(presentNode->_right != nullptr){
            queue.enqueue(presentNode->_right);
        }
    }
}

void AVL::printTree(node *root, Trunk *prev, bool isLeft)
{
    if (root == nullptr)
        return;

    string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);

    printTree(root->_left, trunk, true);

    if (!prev)
        trunk->str = "---";
    else if (isLeft)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else
    {
        trunk->str = "`---";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    cout << root->_data << endl;

    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(root->_right, trunk, false);
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
node *AVL::root(){
    return _root;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
node *AVL::search(node *root, int data){
    if(root == nullptr){
        cout << "Number is not in there." << endl << endl;
        return root;
    }
    if(data == root->_data){
        cout << "Value found: " << data << endl << endl;
        return root;
    }else if(data < root->_data){
        root->_left = search(root->_left, data);
    }else if(data > root->_data){
        root->_right = search(root->_right, data);
    }else{
        cout << "Number is not in there." << endl << endl;
    }
    return root;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
node *AVL::minElement(node *root){
    if(root == nullptr || (root->_left == nullptr && root->_right == nullptr)){
        return root;
    }
    if(root->_left == nullptr){
        return root;
    }
    node *current = root;
    while(current->_left != nullptr){
        current = current->_left;
    }
    return current;
}

void AVL::deleteMin(node *root){
    if(root->_left == nullptr){
        root = root->_right;
        return;
    }
    node *p = root->_left;
    node *q = root;
    while(p->_left != nullptr){
        q = p;
        p = p->_left;
    }
    q->_left = p->_right;
}

node *AVL::Delete(node *root, int data){
    // STEP 1: PERFORM STANDARD BST DELETE
    if(root == nullptr){
        cout << "Number is not in there." << endl << endl;
        return root;
    }
    if(data < root->_data){
        root->_left = Delete(root->_left, data);
    }else if(data > root->_data){
        root->_right = Delete(root->_right, data);
    }else{
        if(root->_left != nullptr && root->_right != nullptr){
            node *temp = root;
            node *replace = minElement(temp->_right);
            root->_data = replace->_data;
            root->_right = Delete(root->_right, replace->_data);
        }else if(root->_left != nullptr){
            root = root->_left;
        }else if(root->_right != nullptr){
            root = root->_right;
        }else{
            root = nullptr;
        }
    }
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    if(root == nullptr){
        return root;
    }
    root->_height = 1 + max(height(root->_left), height(root->_right));
    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = getBalance(root);
    
    // If this node becomes unbalanced, then
       // there are 4 cases
    
    // Left Left Case
      if (balance > 1 && getBalance(root->_left) >= 0)
          return rightRotate(root);
    
      // Right Right Case
      if (balance < -1 && getBalance(root->_right) <= 0)
          return leftRotate(root);
    
      // Left Right Case
      if (balance > 1 && getBalance(root->_left) < 0)
      {
          root->_left = leftRotate(root->_left);
          return rightRotate(root);
      }
    
      // Right Left Case
      if (balance < -1 && getBalance(root->_right) > 0)
      {
          root->_right = rightRotate(root->_right);
          return leftRotate(root);
      }
    return root;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int main(int argc, const char * argv[]) {
    AVL tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);
    tree.insert(80);
    tree.insert(90);
    tree.preOrder(tree.root());
    cout << endl << endl;
    tree.inOrder(tree.root());
    cout << endl << endl;
    tree.postOrder(tree.root());
    cout << endl << endl;
    tree.levelOrder(tree.root());
    cout << endl << endl;
    tree.printTree(tree.root(), nullptr, false);
    cout << endl << endl;
    tree.search(tree.root(), 10);
    tree.Delete(tree.root(), 60);
    tree.printTree(tree.root(), nullptr, false);
    cout << endl << endl;
    return 0;
}
