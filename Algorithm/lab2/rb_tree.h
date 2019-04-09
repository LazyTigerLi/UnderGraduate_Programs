//rb_tree.h
#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>

enum color{red,black};

struct interval
{
    int low;
    int high;
};

struct treenode
{
    treenode *left;
    treenode *right;
    treenode *par;
    color c;
    int max;
    interval i;
};

class RB_tree
{
public:
    treenode *root;
    RB_tree();
    ~RB_tree();
    static void RB_insert(RB_tree *T,treenode *z);
    static void display(treenode *node, int depth);
    static void RB_delete(RB_tree *T,treenode *z);
    static treenode *interval_search(RB_tree *T,interval i);

private:
    treenode *nil;
    static void left_rotate(RB_tree *T,treenode *x);
    static void right_rotate(RB_tree *T,treenode *y);
    static void RB_insert_fixup(RB_tree *T,treenode *z);
    static void RB_transplant(RB_tree *T,treenode *u,treenode *y);
    static treenode *tree_minimum(treenode *x);
    static void RB_delete_fixup(RB_tree *T,treenode *x);
    static void update_max(RB_tree *T,treenode *x);
};

RB_tree::RB_tree()
{
    nil = new treenode;
    nil->c = black;
    nil->max = -1;
    nil->par = nullptr;
    nil->left = nullptr;
    nil->right = nullptr;
    root = nil;
}

RB_tree::~RB_tree()
{}

int max(int x1,int x2,int x3)
{
    if(x1 >= x2 && x1 >= x3)return x1;
    if(x2 >= x1 && x2 >= x3)return x2;
    return x3;
}

void RB_tree::update_max(RB_tree *T,treenode *x)
{
    treenode *temp = x;
    while(temp != T->nil)
    {
        temp->max = max(temp->i.high,temp->left->max,temp->right->max);
        temp = temp->par;
    }
}

void RB_tree::left_rotate(RB_tree *T, treenode *x)
{
    treenode *y = x->right;
    x->right = y->left;
    if(y->left != T->nil)
        y->left->par = x;
    y->par = x->par;
    if(x->par == T->nil)
        T->root = y;
    else if(x == x->par->left)
        x->par->left = y;
    else x->par->right = y;
    y->left = x;
    x->par = y;
    RB_tree::update_max(T,x);
}

void RB_tree::right_rotate(RB_tree *T, treenode *y)
{
    treenode *x = y->left;
    y->left = x->right;
    if(x->right != T->nil)
        x->right->par = y;
    x->par = y->par;
    if(y->par == T->nil)
        T->root = x;
    else if(y == y->par->left)
        y->par->left = x;
    else y->par->right = x;
    x->right = y;
    y->par = x;
    RB_tree::update_max(T,y);
}

void RB_tree::RB_insert_fixup(RB_tree *T, treenode *z)
{
    while(z->par->c == red)
    {
        if(z->par == z->par->par->left)
        {
            treenode *y = z->par->par->right;
            if(y->c == red)
            {
                z->par->c = black;
                y->c = black;
                z->par->par->c = red;
                z = z->par->par;
            }
            else
            {
                if(z == z->par->right)
                {
                    z = z->par;
                    RB_tree::left_rotate(T,z);
                }
                z->par->c = black;
                z->par->par->c = red;
                RB_tree::right_rotate(T,z->par->par);
            }
        }
        else
        {
            treenode *y = z->par->par->left;
            if(y->c == red)
            {
                z->par->c = black;
                y->c = black;
                z->par->par->c = red;
                z = z->par->par;
            }
            else
            {
                if(z == z->par->left)
                {
                    z = z->par;
                    RB_tree::right_rotate(T,z);
                }
                z->par->c = black;
                z->par->par->c = red;
                RB_tree::left_rotate(T,z->par->par);
            }
        }
    }
    T->root->c = black;
}

void RB_tree::RB_insert(RB_tree *T, treenode *z)
{
    treenode *y = T->nil;
    treenode *x = T->root;
    while(x != T->nil)
    {
        y = x;
        if(z->i.low < x->i.low)
            x = x->left;
        else x = x->right;
    }
    z->par = y;
    if(y == T->nil)
        T->root = z;
    else if(z->i.low < y->i.low)
        y->left = z;
    else y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->c = red;

    RB_tree::update_max(T,z);
    RB_tree::RB_insert_fixup(T,z);

}

void RB_tree::display(treenode *node,int depth)
{
    for(int i = 0; i < depth; i++)std::cout<<'\t';
    if(!node->left && !node->right)
    {
        std::cout<<"nil black"<<std::endl;
        return;
    }
    std::cout<<'['<<node->i.low<<','<<node->i.high<<"] | "<<node->max
            <<' '<<((node->c == black) ? "black" : "red")<<std::endl;
    display(node->left,depth + 1);
    display(node->right,depth + 1);
}

void RB_tree::RB_transplant(RB_tree *T, treenode *u, treenode *v)
{
    if(u->par == T->nil)
        T->root = v;
    else if(u == u->par->left)
        u->par->left = v;
    else u->par->right = v;
    v->par = u->par;
}

treenode* RB_tree::tree_minimum(treenode *x)
{
    while(x->left->left && x->left->right)      //x->left != T.nil
        x = x->left;
    return x;
}

void RB_tree::RB_delete_fixup(RB_tree *T, treenode *x)
{
    while(x != T->root && x->c == black)
    {
        if(x == x->par->left)
        {
            treenode *w = x->par->right;
            if(w->c == red)
            {
                w->c = black;
                x->par->c = red;
                RB_tree::left_rotate(T,x->par);
                w = x->par->right;
            }
            if(w->left->c == black && w->right->c == black)
            {
                w->c = red;
                x = x->par;
            }
            else
            {
                if(w->right->c == black)
                {
                    w->left->c = black;
                    w->c = red;
                    RB_tree::right_rotate(T,w);
                    w = x->par->right;
                }
                w->c = x->par->c;
                x->par->c = black;
                w->right->c = black;
                RB_tree::left_rotate(T,x->par);
                x = T->root;
            }
        }
        else
        {
            treenode *w = x->par->left;
            if(w->c == red)
            {
                w->c = black;
                x->par->c = red;
                RB_tree::right_rotate(T,x->par);
                w = x->par->left;
            }
            if(w->left->c == black && w->right->c == black)
            {
                w->c = red;
                x = x->par;
            }
            else
            {
                if(w->left->c == black)
                {
                    w->right->c = black;
                    w->c = red;
                    RB_tree::left_rotate(T,w);
                    w = x->par->left;
                }
                w->c = x->par->c;
                x->par->c = black;
                w->left->c = black;
                RB_tree::right_rotate(T,x->par);
                x = T->root;
            }
        }
    }
    x->c = black;
}

void RB_tree::RB_delete(RB_tree *T, treenode *z)
{
    treenode *y = z;
    treenode *x;
    color y_original_color = y->c;
    if(z->left == T->nil)
    {
        x = z->right;
        RB_tree::RB_transplant(T,z,z->right);
        RB_tree::update_max(T,z->par);
    }
    else if(z->right == T->nil)
    {
        x = z->left;
        RB_tree::RB_transplant(T,z,z->left);
        RB_tree::update_max(T,z->par);
    }
    else
    {
        y = RB_tree::tree_minimum(z->right);
        y_original_color = y->c;
        x = y->right;
        if(y->par == z)
            x->par = y;
        else
        {
            RB_tree::RB_transplant(T,y,y->right);
            y->right = z->right;
            y->right->par = y;
        }
        RB_tree::RB_transplant(T,z,y);
        y->left = z->left;
        y->left->par = y;
        y->c = z->c;
        RB_tree::update_max(T,x->par);
    }
    delete z;
    if(y_original_color == black)
        RB_tree::RB_delete_fixup(T,x);
}

treenode* RB_tree::interval_search(RB_tree *T,interval i)
{
    treenode *x = T->root;
    while(x != T->nil && (i.high < x->i.low || i.low > x->i.high))
    {
        if(x->left != T->nil && x->left->max >= i.low)
            x = x->left;
        else x = x->right;
    }
    return x;
}

#endif // RB_TREE_H
