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
    if(tmp_2 == NULL)
        root = new_node;
    else if(new_node->key < tmp_2->key)
        tmp_2->left = new_node;
    else
        tmp_2->right = new_node;
    /*tmp_2 = tmp_2->papa;
		while(tmp_2 != nullptr)
		{
				balance_node(tmp_2);
				tmp_2 = tmp_2->papa;
		}*/
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
    string order;
    cin >> order;
    int key;
    //cin >> key;
    Node* root = new Node;
    root->key = 15;
    root->dif = 1;

    root->left = new Node;
    root->left->papa = root;
    root->left->key = 10;
    root->left->dif = 0;

    root->left->left = new Node;
    root->left->left->papa = root->left;
    root->left->left->key = 9;
    root->left->left->dif = 0;

    root->left->right = new Node;
    root->left->right->papa = root->left;
    root->left->right->key = 13;
    root->left->right->dif = 0;

    root->right = new Node;
    root->right->papa = root;
    root->right->key = 25;
    root->right->dif = 0;
    traverse(root, order);
    delete_tree(root);
    //root = nullptr;
    //insert(root, 15);   
    traverse(root, order);
    //cout << find(root, key);
    return 0;
}