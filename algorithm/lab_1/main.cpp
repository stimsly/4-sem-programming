#include <bits/stdc++.h>

using namespace std;

const int experements = 10;
const int n = 1000000;

class btree{
public:
    class node{
    public:
        node *left;
        node *right;
        int val;
        node(int val, node *left = nullptr, node *right = nullptr){
            this->val = val;
            this->left = left;
            this->right = right;
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
        if(this->head == nullptr) this->head = new node(a);
        else push(a, head);
    }


    void push(int a, node *root) {
        if(a > root->val) {
            if(root->right) push(a, root->right);
            else root->right = new node(a);
        } else {
            if(root->left) push(a, root->left);
            else root->left = new node(a);
        }
    }


    void print(){ print(head); };
    void print(node* root){
        if(root->left) print(root->left);
        cout << root->val << " ";
        if(root->right) print(root->right);
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
        node(int a){
            val = a;
            h = 1;
            left = right = nullptr;
        }
    };
    node* head;
public:
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
        else insert(head, a);
    }
    void insert(node* p, int k) // вставка ключа k в дерево с корнем p
    {
        if(p->val > k){
            if(p->left) insert(p->left, k);
            else p->left = new node(k);
        } else {
            if(p->right) insert(p->right, k);
            else p->right = new node(k);
        }
        balance(p);
    }
};

class time_calc{
    int max_time;
    int all_time;
    int min_time;
    int start_time;
    int finish_time;
    int count_experements;
public:
    time_calc(){
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
    void stop(){
        finish_time = clock();
        add(finish_time - start_time);
    };
    void show_results(){
        cout << "Min time is: " << min_time << "ms.\n";
        cout << "Average time is: " << all_time / count_experements << "ms.\n";
        cout << "Max time is: " << max_time << "ms.\n";
    }
};

int main() {

    time_calc t;

    for(int i = 0; i < experements; i++){
        // choice a tree
        //btree a;
        AVL a;
        //red_black_tree a;
        vector <int> data(n);
        for(int j = 0; j < n; j++) data[j] = rand();
        t.start();
        for(int j = 0; j < n; j++){
            a.push(data[j]);
        }
        t.stop();
    }
    t.show_results();
    return 0;
}
