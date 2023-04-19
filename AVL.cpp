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

void delete_tree0(Node* node){
    if (node==nullptr){return;}
    delete_tree0(node->left);
    delete_tree0(node->right); 
    delete node;
}

void delete_tree(Node* node){
    if (node->left != nullptr){
        delete_tree0(node->left);
    }
    if (node->right != nullptr){
        delete_tree0(node->right);
    }
    delete node;
    node = nullptr;
}


void erase(Node* node, int key){
    //if ()
}

int main(){
    Node* root = new Node;
    insert(root, 1);
    insert(root, 2);
    insert(root, 3);
    insert(root, 4);
    insert(root, 5);
    insert(root, 6);
    insert(root, 7);
    insert(root, 8);
    insert(root, 9);
    insert(root, 10);
    cout << root->height << endl;
    traverse(root, "in");
    return 0;
}