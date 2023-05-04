#include <bits/stdc++.h>

using namespace std;

const int experements = 100;
const int n = 10000;

class btree{
public:
    class node{
    public:
        node *left;
        node *right;
        int val;
        int h;
        node(int val, int h = 1, node *left = nullptr, node *right = nullptr){
            this->val = val;
            this->left = left;
            this->right = right;
            this->h = h;
            cout << val << " " << h << endl;
        }
        node(){
            left = nullptr;
            right = nullptr;
        }
    };
    node *head;
    btree(){
        head = nullptr;
    }

    void push(int a){
        if(head == nullptr) head = new node(a);
        else push(a, head, 2);
    }


    void push(int a, node *root, int h) {
        cout << a << " " << root << " " << h << endl;
        if(a > root->val) {
            cout << "right " << h << " ";
            if(root->right) push(a, root->right, h + 1);
            else root->right = new node(a, h);
        } else {
            cout << "left " << h << " ";
            if(root->left) push(a, root->left, h + 1);
            else root->left = new node(a, h);
        }
    }


    void print(){ print(head); };
    void print(node* root){
        if(root->left) print(root->left);
        cout << root->val << " ";
        if(root->right) print(root->right);
    }
    int hh(){return hh(head);};
    int hh(node* root){
        int ans = root->h;
        if(root->right) ans = max(ans, hh(root->right));
        if(root->left) ans = max(ans, hh(root->left));
        return ans;
    }

};

class red_black_tree{

};

class AVL{
    struct node{
        node* left;
        node* right;
        unsigned char h;
        int val;
        node(int a, int h2 = 0){
            val = a;
            h = h2;
            left = right = nullptr;
        }
    };
public:
    node* head;
    AVL() { head = nullptr; };

    unsigned char height(node* a) { return a ? a->h : 0; };
    int balance_factor(node* a) {return height(a->right) - height(a->left); };
    void up_height(node* a){
        int hl = height(a->left);
        int hr = height(a->right);
        a->h = max(hl, hr) + 1;
    }
    node* rotateright(node* p){
        node* q = p->left;
        p->left = q->right;
        q->right = p;
        up_height(p);
        up_height(q);
        return q;
    }
    node* rotateleft(node* q){
        node* p = q->right;
        q->right = p->left;
        p->left = q;
        up_height(p);
        up_height(q);
        return p;
    }
    node* balance(node* p)
    {
        up_height(p);
        if(balance_factor(p)==2 )
        {
            if( balance_factor(p->right) < 0 )
                p->right = rotateright(p->right);
            return rotateleft(p);
        }
        if( balance_factor(p)==-2 )
        {
            if( balance_factor(p->left) > 0  )
                p->left = rotateleft(p->left);
            return rotateright(p);
        }
        return p;
    }
    void push(int a){
        if(!head)head = new node(a);
        else push(a, head, 1);
    }
    void push(int a, node *root, int h) {
        cout << a << " " << root << " " << h << endl;
        if(a > root->val) {
            cout << "right " << h << " ";
            if(root->right) push(a, root->right, h + 1);
            else root->right = new node(a, h);
        } else {
            cout << "left " << h << " ";
            if(root->left) push(a, root->left, h + 1);
            else root->left = new node(a, h);
        }
        balance(root);
    }
    int hh(){return hh(head);};
    int hh(node* root){
        int ans = root->h;
        if(root->right) ans = max(ans, hh(root->right));
        if(root->left) ans = max(ans, hh(root->left));
        return ans;
    }
    void print(){ print(head); };
    void print(node* root){
        if(root->left) print(root->left);
        cout << root->val << " ";
        if(root->right) print(root->right);
    }
};

struct Node_RBT
{
    bool is_search_second = false;
    int data;
    Node_RBT* left;
    Node_RBT* right;
    Node_RBT* parent;
    char color;
    Node_RBT(Node_RBT* left, Node_RBT* right, Node_RBT* parent, char color) {
        this->data = NULL;
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->color = color;
    }
    Node_RBT(int data, Node_RBT* left, Node_RBT* right, Node_RBT* parent, char color) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->color = color;
    }

    Node_RBT* NILL = new Node_RBT(NULL, NULL, NULL, 'b');


    int height(Node_RBT* root) {
        if (root == NILL)
            return 0;
        int hl = height(root->left);
        int hr = height(root->right);
        return max(hl, hr) + 1;
    }



    Node_RBT* search(Node_RBT* root, int key) {
        Node_RBT* tmp = NULL, * tmp1 = NULL;
        if (root == NILL) {
            return NULL;
        }
        else
        if (root->data == key) {
            tmp = root->right;
            is_search_second = false;
            while (tmp != NILL) {
                if (tmp->data == key) {
                    is_search_second = true;
                    tmp1 = tmp;
                }
                tmp = tmp->right;
            }
            if (is_search_second)
                return tmp1;
            else
                return root;
        }
        else if (key < root->data) {
            return search(root->left, key);
        }
        else {
            return search(root->right, key);
        }
    }

    void left_rotate(Node_RBT* node, Node_RBT*& root) {
        Node_RBT* x, * y;
        x = node;
        y = node->right;
        if (y->left != NULL) {
            x->right = y->left;
            if (y->left != NILL)
                y->left->parent = x;
        }
        //else
        //x->right = NULL;
        if (x->parent == NULL) {
            root = y;
            y->parent = NULL;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
            y->parent = x->parent;
        }
        else {
            x->parent->right = y;
            y->parent = x->parent;
        }
        y->left = x;
        x->parent = y;
    }

    void right_rotate(Node_RBT* node, Node_RBT*& root) {
        Node_RBT* x, * y;
        y = node;
        x = node->left;
        if (x->right != NULL) {
            y->left = x->right;
            if (x->right != NILL)
                x->right->parent = y;
        }
        //else
        //y->left = NULL;
        if (y->parent == NULL) {
            root = x;
            x->parent = NULL;
        }
        else if (y == y->parent->right) {
            y->parent->right = x;
            x->parent = y->parent;
        }
        else {
            y->parent->left = x;
            x->parent = y->parent;
        }
        x->right = y;
        y->parent = x;
    }

    void add_balance_RBT(Node_RBT*& root, Node_RBT* elem, int time = 0) {
        time++;
        Node_RBT* NewNode = elem, * p, * Gp;
        p = NewNode->parent;
        if (p == NULL) {
            return;
        }
        Gp = p->parent;
        if (Gp == NULL)
            return;
        if (p->color == 'b') {
            return;
        }
        if (p == Gp->left) {
            if (Gp->right->color == 'r') {
                p->color = Gp->right->color = 'b';
                Gp->color = 'r';
                NewNode = Gp;
                p = NewNode->parent;
                if (p != NULL) {
                    Gp = p->parent;
                }
            }
            else if (NewNode == p->right) {
                NewNode = p;
                left_rotate(p, root);
                p = NewNode->parent;
                if (p != NULL) {
                    Gp = p->parent;
                }

            }
            else {
                p->color = 'b';
                Gp->color = 'r';
                right_rotate(Gp, root);
                p = NewNode->parent;
                if (p != NULL) {
                    Gp = p->parent;
                }
            }
        }
        else {
            if (Gp->left->color == 'r') {
                p->color = Gp->left->color = 'b';
                Gp->color = 'r';
                NewNode = Gp;
                p = NewNode->parent;
                if (p != NULL) {
                    Gp = p->parent;
                }
            }
            else if (NewNode == p->left) {
                NewNode = p;
                right_rotate(p, root);
                p = NewNode->parent;
                if (p != NULL) {
                    Gp = p->parent;
                }
            }
            else {
                p->color = 'b';
                Gp->color = 'r';
                left_rotate(Gp, root);
                p = NewNode->parent;
                if (p != NULL) {
                    Gp = p->parent;
                }
            }
        }
        if (p != NULL && NewNode->parent->color != 'b')
            add_balance_RBT(root, NewNode, time);
        root->color = 'b';

    }
    void push(int a) {push(a, )};
    void push(int elem, Node_RBT*& root, int time = 0){
        Node_RBT* NewNode;
        NewNode = new Node_RBT(elem, NILL, NILL, NULL, 'r');
        Node_RBT* y = nullptr;
        Node_RBT* x = root;
        while (x != NILL) {
            y = x;
            if (NewNode->data < x->data) {
                x = x->left;
                time++;
            }
            else {
                x = x->right;
                time++;
            }
        }
        NewNode->parent = y;
        if (y == nullptr) {
            root = NewNode;
        }
        else if (NewNode->data < y->data) {
            y->left = NewNode;
        }
        else {
            y->right = NewNode;
        }
        if (y == NULL)
            NewNode->color = 'b';
        if (NewNode->parent != NULL && NewNode->parent->color != 'b') {
            add_balance_RBT(root, NewNode, time);
        }
    }
};



class calc{
    int max_time;
    int all_time;
    int min_time;
    int start_time;
    int finish_time;
    int count_experements;
    int max_h = 0;
    int min_h = INT_MAX;
    int s_h = 0;
public:
    calc(){
        count_experements = 0;
        max_time = 0;
        all_time = 0;
        min_time = 1000000;
    }
    void add(int current){
        max_time = max(max_time, current);
        min_time = min(min_time, current);
        all_time += current;
        count_experements++;
    }
    void start(){
        start_time = clock();
    };
    void stop(int h){
        s_h += h;
        max_h = max(max_h, h);
        min_h = min(min_h, h);
        finish_time = clock();
        add(finish_time - start_time);
    };
    void show_results(){
        cout << "Min time is: " << min_time << "ms.\n";
        cout << "Average time is: " << all_time / count_experements << "ms.\n";
        cout << "Max time is: " << max_time << "ms.\n";
        cout << "Min height is: " << min_h << ".\n";
        cout << "Average height is: " << s_h / count_experements << ".\n";
        cout << "Max height is: " << max_h << ".\n";
    }
};

int main() {

    calc t;

    for(int i = 0; i < experements; i++){
        // choice a tree
        //btree a;
        //AVL a;
        Node_RBT a;
        vector <int> data(n);
        for(int j = 0; j < n; j++) data[j] = rand();
        t.start();
        for(int j = 0; j < n; j++){
            cout << data[j] << endl;
            a.push(data[j]);
            cout << endl;
            int x;
            cin >> x;
        }
        a.balance(a.head);
        t.stop(a.hh());
    }
    t.show_results();
    return 0;
}
