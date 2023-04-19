#include <iostream>
#include <string>
using namespace std;

struct Node{
    int key;
    int flag_left_or_right = 0;
    /*
    0 - root or nullptr
    1 - left
    2 - right
    */
    Node* left = nullptr;
    Node* right = nullptr;
    Node* papa = nullptr;
    int height = 0;
    int dif;
};

int height_set(Node* node)
{
    if(node->right == nullptr and node->left == nullptr)
    {
        return 1;
    }
    else if(node->right == nullptr and node->left != nullptr)
    {
        return height_set(node->left) + 1;
    }
    else if(node->left == nullptr and node->right != nullptr)
    {
        return height_set(node->right) + 1;
    }
    else
    {
        int r = height_set(node->right);
        int l = height_set(node->left);
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

int height_return(Node* p) {
	return p ? p->height : 0;
}

int bfactor(Node* node) {
    int p = 0;
    if (node != nullptr) {
        p = height_return(node->right) - height_return(node->left);
    }
	return p;
}

Node* small_left_turn(Node* p)
{
    Node* papa_mem = p->papa;
    Node* q = p->right;
    Node* s = q->left;
    q->flag_left_or_right = p->flag_left_or_right;
    p->flag_left_or_right = 1;
    s->flag_left_or_right = 2;
    p->papa = q;
    q->left = p;
    p->right = s;
    q->papa = papa_mem;
    return q;
}

Node* small_right_turn(Node* q)
{
    Node* papa_mem = q->papa;
    Node* s = q->left;
    Node* C = s->right;
    s->flag_left_or_right = q->flag_left_or_right;
    q->flag_left_or_right = 2;
    C->flag_left_or_right = 1;
    q->papa = s;
    s->right = q;
    q->left = C;
    s->papa = papa_mem;
    return s;
}

Node* big_left_turn(Node* p)
{
    p->right = small_right_turn(p->right);
    return small_left_turn(p);
}

Node* big_right_turn(Node* p)
{
    p->left = small_left_turn(p->left);
    return small_right_turn(p);
}

void balance_node(Node* node)
{
    if(node != nullptr)
    {
        Node* p = node->papa;
        if(node->right->height - node->left->height == 2)
        {
            if(node->right->right->height >= node->right->left->height)
            {
                if(node->flag_left_or_right == 2)
                    p->right = small_left_turn(node);
                else if(node->flag_left_or_right == 1)
                    p->left = small_left_turn(node);
            }
            else if(node->right->right->height < node->right->left->height)
            {
                if(node->flag_left_or_right == 2)
                    p->right = big_left_turn(node);
                else if(node->flag_left_or_right == 1)
                    p->left = big_left_turn(node);
            }
        }
        else if(node->left->height - node->right->height == 2)
        {
            if(node->left->left->height >= node->left->right->height)
            {
                if(node->flag_left_or_right == 2)
                    p->right = small_right_turn(node);
                else if(node->flag_left_or_right == 1)
                    p->left = small_right_turn(node);
            }
            else if(node->left->left->height < node->left->right->height)
            {
                if(node->flag_left_or_right == 2)
                    p->right = big_right_turn(node);
                else if(node->flag_left_or_right == 1)
                    p->left = big_right_turn(node);
            }
        }
    }
}

void insert(Node* root, int value){
    Node* new_node = new Node;
    new_node->key = value;
    Node* tmp_1 = root;
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
    if(tmp_2 == nullptr)
        root = new_node;
    else if(new_node->key < tmp_2->key)
    {
        tmp_2->left = new_node;
        new_node->flag_left_or_right = 1;
    }
    else
    {
        tmp_2->right = new_node;
        new_node->flag_left_or_right = 2;
    }
    new_node->height = 1;
    tmp_2 = tmp_2->papa;
	while(tmp_2 != nullptr)
	{
		balance_node(tmp_2);
        height_set(tmp_2);
		tmp_2 = tmp_2->papa;
	}
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

int main(){
    Node* root = new Node;
    root->key = 15;
    root->height = 4;
    root->dif = 1;

    root->left = new Node;
    root->left->papa = root;
    root->left->height = 3;
    root->left->key = 10;
    root->left->dif = 0;

    root->left->left = new Node;
    root->left->left->papa = root->left;
    root->left->left->height = 1;
    root->left->left->key = 9;
    root->left->left->dif = 0;

    root->left->right = new Node;
    root->left->right->papa = root->left;
    root->left->right->height = 2;
    root->left->right->key = 13;
    root->left->right->dif = 0;

    root->left->right->right = new Node;
    root->left->right->right->papa = root->left->right;
    root->left->right->right->height = 1;
    root->left->right->key = 14;
    root->left->right->dif = 0;

    root->right = new Node;
    root->right->papa = root;
    root->right->height = 1;
    root->right->key = 25;
    root->right->dif = 0;
    //delete_tree(root);
    balance_node(root);
    cout << root->key << endl;
    return 0;
}