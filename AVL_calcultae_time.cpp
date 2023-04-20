#include <random>
#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

int const N = 1000000;

std::mt19937 gen(static_cast<unsigned>(time(nullptr)));

struct Node{
    int key;
    Node* left = nullptr;
    Node* right = nullptr;
    int height;
};


int height(Node* Node){
    if (Node == nullptr){
        return 0;
    }
    return Node->height;
}


Node* right_turn(Node* y){
    Node* x = y->left;
    Node* T2 = x->right;
 
    x->right = y;
    y->left = T2;
 
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}
 

Node* left_turn(Node* x){
    Node* y = x->right;
    Node* T2 = y->left;
 
    y->left = x;
    x->right = T2;
 
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
 
    return y;
}
 

int get_balance(Node* N){
    if (N == nullptr){
        return 0;
    }
    return height(N->left) - height(N->right);
}
 
Node* insert(Node* node, int key){
    if (node == nullptr){
        Node* node = new Node;
        node->key = key;
        node->height = 1;
        return node;
    }
    if (key <= node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;
 
    node->height = 1 + max(height(node->left),
                           height(node->right));
 
    int balance = get_balance(node);
 
    if (balance > 1 && key < node->left->key)
        return right_turn(node);
 
    if (balance < -1 && key > node->right->key)
        return left_turn(node);
 
    if (balance > 1 && key > node->left->key)
    {
        node->left = left_turn(node->left);
        return right_turn(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = right_turn(node->right);
        return left_turn(node);
    }

    return node;
}

Node*  min_node(Node* node){
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}


Node*  max_node(Node* node){
    Node* current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}
 

Node* delete_node(Node* root, int key){
    if (root == nullptr){
        return root;
    }
 
    if (key < root->key){
        root->left = delete_node(root->left, key);
    }

    else if(key > root->key){
        root->right = delete_node(root->right, key);
    }

    else{
        if((root->left == nullptr) or (root->right == nullptr)){
            Node* temp = root->left ?
                         root->left :
                         root->right;
 
            if (temp == nullptr){
                temp = root;
                root = nullptr;
            }
            else
            *root = *temp;
            free(temp);
        }
        else{
            Node* temp = min_node(root->right);
            root->key = temp->key;
            root->right = delete_node(root->right, temp->key);
        }
    }
 
    if (root == nullptr)
    return root;
 
    root->height = 1 + max(height(root->left), height(root->right));
 
    int balance = get_balance(root);
 
    if (balance > 1 and get_balance(root->left) >= 0)
        return right_turn(root);
 
    if (balance > 1 and get_balance(root->left) < 0){
        root->left = left_turn(root->left);
        return right_turn(root);
    }
 
    if (balance < -1 and
        get_balance(root->right) <= 0)
        return left_turn(root);
 
    if (balance < -1 and
        get_balance(root->right) > 0)
    {
        root->right = right_turn(root->right);
        return left_turn(root);
    }
 
    return root;
}


void print(Node* node, string order){
    if (node==nullptr){return;}
    if (order == "pre"){
        cout << node->key << " ";
        print(node->left, order);
        print(node->right, order);
    }
    if (order == "in"){
        print(node->left, order);
        cout << node->key << " ";
        print(node->right, order);
    }
    if (order == "post"){
        print(node->left, order);
        print(node->right, order); 
        cout << node->key << " ";
    }
}


void delete_tree(Node* &node){
    if (node != nullptr){
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }
    node = nullptr;
}

Node* find(Node* node, int value){
    Node* tmp = node;
    if(tmp == nullptr)
        return nullptr;
    else if(tmp->key == value)
        return tmp;
    else if(tmp->key > value)
        return find(tmp->left, value);
    else
        return find(tmp->right, value);
}


int dice(int min, int max){
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

void generate_array(int (&array)[N], int n){
    for(int i = 0; i < n; i++){
        array[i] = dice(0, 10000);
    }
}

void make_tree(Node* &root, int (&array)[N], int n){
    for(int i = 0; i < n; i++){
        root = insert(root, array[i]);
    }
}

void calculate_insert_time(int (&array)[N], Node* &root, int sizes_number, int size_step, int rep_number){
    remove("insert_time(n).txt");
    std::ofstream out;
    out.open("insert_time(n).txt", std::ios::app);
    for(int i = 1; i < sizes_number + 1; i++){
        int size = i * size_step;
        out << size << " ";

        int random_arr[N];
        generate_array(random_arr, N);
        generate_array(array, size);
        make_tree(root, array, size);
        auto begin = std::chrono::steady_clock::now();
        for(int j = 0; j < rep_number; j++){
            insert(root, random_arr[j]);
            delete_node(root, random_arr[j]);
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        
        delete_tree(root);

        out << time_span.count() << endl;

    }
    out.close(); 
}

void calculate_find_time(int (&array)[N], Node* &root, int sizes_number, int size_step, int rep_number){
    remove("find_time(n).txt");
    std::ofstream out;
    out.open("find_time(n).txt", std::ios::app);
    for(int i = 1; i < sizes_number + 1; i++){
        int size = i * size_step;
        out << size << " ";

        int random_arr[N];
        generate_array(random_arr, N);        
        generate_array(array, size);
        make_tree(root, array, size);
        auto begin = std::chrono::steady_clock::now();
        for(int j = 0; j < rep_number; j++){
            find(root, random_arr[j]);
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        
        delete_tree(root);

        out << time_span.count() << endl;

    }
    out.close(); 
}

int main(){
    int array[N];
    Node* root = nullptr;
    calculate_insert_time(array, root, 50, 8, 100000);
    //calculate_find_time(array, root, 100, 10, 100000);
}