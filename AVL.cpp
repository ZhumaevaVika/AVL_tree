#include <iostream>
#include <string>
using namespace std;

struct Node
{
    int key;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* papa = nullptr;
    int height = 0;
};

int height_set_recursive(Node* node)
{
    if(node->right == nullptr and node->left == nullptr)
        return 1;
    else if(node->right == nullptr and node->left != nullptr)
        return height_set_recursive(node->left) + 1;
    else if(node->left == nullptr and node->right != nullptr)
        return height_set_recursive(node->right) + 1;
    else
    {
        int r = height_set_recursive(node->right);
        int l = height_set_recursive(node->left);
        if(l > r)
        {
            return l + 1;
        }
        else
        {
            return r + 1;
        }
    }
}

void height_set(Node* node)
{
    node->height = height_set_recursive(node);
}

int get_height(Node* p) {
	return p ? p->height : 0;
}

Node* small_left_turn(Node* p)
{
    Node* papa_mem = p->papa;
    Node* q = p->right;
    Node* s = q->left;
    p->papa = q;
    q->left = p;
    p->right = s;
    q->papa = papa_mem;
    if(papa_mem != nullptr)
    {
        if(papa_mem->left == p)
            papa_mem->left = q;
        else
            papa_mem->right = q;
    }
    return q;
}

Node* small_right_turn(Node* q)
{
    Node* papa_mem = q->papa;
    Node* s = q->left;
    Node* C = s->right;
    q->papa = s;
    s->right = q;
    q->left = C;
    s->papa = papa_mem;
    if(papa_mem != nullptr)
    {
        if(papa_mem->left == q)
            papa_mem->left = s;
        else
            papa_mem->right = s;
    }
    return s;
}

Node* big_left_turn(Node* p)
{
    small_right_turn(p->right);
    return small_left_turn(p);
}

Node* big_right_turn(Node* p)
{
    small_left_turn(p->left);
    return small_right_turn(p);
}

Node* balance_node(Node* node)
{
    if(node != nullptr)
    {
        if(get_height(node->right) - get_height(node->left) == 2)
        {
            if(get_height(node->right->right) >= get_height(node->right->left))
                return small_left_turn(node);
            else if(get_height(node->right->right) < get_height(node->right->left))
                return big_left_turn(node);
        }
        else if(get_height(node->left) - get_height(node->right) == 2)
        {
            if(get_height(node->left->left) >= get_height(node->left->right))
                return small_right_turn(node);
            else if(get_height(node->left->left) < get_height(node->left->right))
                return big_right_turn(node);
        }
        return node;
    }
    return node;
}

void insert(Node* &root, int value){
    Node* new_node = new Node;
    new_node->key = value;
    Node* tmp_1 = root;
    if(root->height == 0)
    {
        root = new_node;
        root->height = 1;
    }
    else
    {
        Node* tmp_2 = nullptr;
        while (tmp_1 != nullptr)
        {
            tmp_2 = tmp_1;
            if(new_node->key < tmp_1->key)
                tmp_1 = tmp_1->left;
            else
                tmp_1 = tmp_1->right;
        }
        new_node->papa = tmp_2;
        if(new_node->key < tmp_2->key)
            tmp_2->left = new_node;
        else
            tmp_2->right = new_node;
        new_node->height = 1;
        tmp_2 = new_node;
        while(tmp_2 != nullptr)
        {
            if(tmp_2->papa == nullptr)
                root = balance_node(tmp_2);
            else
                balance_node(tmp_2);
            height_set(tmp_2);
            tmp_2 = tmp_2->papa;
        }
    }
}

void set_height_to_all_tree(Node* node){
    if (node != nullptr){
        set_height_to_all_tree(node->left);
        set_height_to_all_tree(node->right);
        height_set(node);
    }
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

void traverse(Node* node, string order){
    if (node==nullptr){return;}
    if (order == "pre"){
        cout << node->key << " ";
        traverse(node->left, order);
        traverse(node->right, order);
    }
    if (order == "in"){
        traverse(node->left, order);
        cout << node->key << " ";
        traverse(node->right, order);
    }
    if (order == "post"){
        traverse(node->left, order);
        traverse(node->right, order); 
        cout << node->key << " ";
    }
}

void delete_tree(Node* &node){
    if (node != NULL){
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }
    node = NULL;
}

int min(Node* node){
    Node* current = node;
    while (current->left != nullptr){
        current = current->left;
    }
    return current->key;
}

Node* min_node(Node* node){
    Node* current = node;
    while (current->left != nullptr){
        current = current->left;
    }
    return current;
}

int max(Node* node){
    Node* current = node;
    while (current->right != nullptr){
        current = current->right;
    }
    return current->key;
}

void swap(Node* &node1, Node* &node2){
    if ((node1->papa!=nullptr)and(node1->left!=nullptr)and(node1->right!=nullptr)
    and(node2->papa!=nullptr)and(node2->left!=nullptr)and(node2->right!=nullptr)){
    auto papa1 = node1->papa;
    auto sonl1 = node1->left;
    auto sonr1 = node1->right;
    node1->papa = node2->papa;
    node1->left = node2->left;
    node1->right = node2->right;
    node2->papa = papa1;
    node2->left = sonl1;
    node2->right = sonr1;
    }
}


void erase(Node* &node, int key){
    if (node!=nullptr){
        if ((node->key == key)and(node->left == nullptr)and(node->right == nullptr)){
            delete node;
            node = NULL;
            return;
        }
        else if ((node->key == key)and((node->left != nullptr)or(node->right != nullptr))){
            if ((node->right != nullptr)and(node->left == nullptr)){
                auto tmp = node->right;
                tmp->papa = node->papa;
                delete node;
                tmp->papa->right = tmp;
                node = NULL;
                return;
            }
            else if ((node->left != nullptr)and(node->right == nullptr)){
                auto tmp = node->left;
                tmp->papa = node->papa;
                delete node;
                tmp->papa->left = tmp;
                node = NULL;
                return;
            }
            else{
                auto tmp = min_node(node->right);
                int tmp_val = node->key;
                node->key = tmp->key;
                tmp->key = tmp_val;
                erase(node, key);
            }
        }
        if (key<node->key){
            erase(node->left, key);
        }
        else if (key>node->key){
            erase(node->right, key);
        }
    }
}

int main(){
    string order;
    cin >> order;
    int key;
    //cin >> key;
    Node* root = new Node;
    root->key = 15;
    root->height = 3;

    root->left = new Node;
    root->left->papa = root;
    root->left->key = 10;
    root->left->height = 2;

    root->left->left = new Node;
    root->left->left->papa = root->left;
    root->left->left->key = 9;
    root->left->left->height = 1;

    root->left->right = new Node;
    root->left->right->papa = root->left;
    root->left->right->key = 13;
    root->left->right->height = 1;

    root->right = new Node;
    root->right->papa = root;
    root->right->key = 25;
    root->right->height = 2;

    root->right->right = new Node;
    root->right->right->papa = root->right;
    root->right->right->key = 30;
    root->right->right->height = 1;
    traverse(root, order);
    cout << endl;
    erase(root, 25);
    //root = nullptr;
    //insert(root, 15);   
    traverse(root, order);
    //cout << find(root, key);
    return 0;
}