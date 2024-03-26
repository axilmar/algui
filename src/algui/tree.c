#include <assert.h>
#include "algui/tree.h"


//check containment
int alg_tree_contains(ALG_TREE* a, ALG_TREE* b) {
    //check params
    assert(a);
    assert(b);

    //check if a is reachable from b
    for (; b; b = b->parent) {
        if (b == a) {
            return 1;
        }
    }

    //not contained
    return 0;
}


//init tree
void alg_init_tree(ALG_TREE* tree) {
    assert(tree);
    tree->parent = NULL;
    tree->prev = NULL;
    tree->next = NULL;
    tree->first = NULL;
    tree->last = NULL;
}


//get root
ALG_TREE* alg_get_root_tree(ALG_TREE* tree) {
    assert(tree);
    for (; tree->parent; tree = tree->parent);
    return tree;
}


//get position
int alg_get_tree_position(ALG_TREE* tree) {
    assert(tree);
    int r = -1;
    for (; tree; ++r, tree = tree->prev);
    return r;
}


//insert child
void alg_insert_tree(ALG_TREE* parent, ALG_TREE* child, int pos) {
    //check params; parent must not be in the tree of child
    assert(parent);
    assert(child);
    assert(!alg_tree_contains(child, parent));

    ALG_TREE* prev;
    ALG_TREE* next;

    //find prev-next from first to last child
    if (pos >= 0) {
        next = parent->first;
        for (; next && pos > 0; next = next->next, --pos);
        prev = next ? next->prev : parent->last;
    }

    //else find prev-next from last to first child
    else {
        prev = parent->last;
        for (; prev && pos < -1; prev = prev->prev, ++pos);
        next = prev ? prev->next : parent->first;
    }

    //link prev
    if (prev) {
        prev->next = child;
    }
    else {
        parent->first = child;
    }

    //link next
    if (next) {
        next->prev = child;
    }
    else {
        parent->last = child;
    }

    //setup child
    child->parent = parent;
    child->prev = prev;
    child->next = next;
}


//remove child from its parent
void alg_remove_tree(ALG_TREE* tree) {
    //check params
    assert(tree);

    //no parent
    if (!tree->parent) {
        return;
    }

    //unlink from previous
    if (tree->prev) {
        tree->prev->next = tree->next;
    }
    else {
        tree->parent->first = tree->next;
    }

    //unlink from next
    if (tree->next) {
        tree->next->prev = tree->prev;
    }
    else {
        tree->parent->last = tree->prev;
    }

    //reset fields
    tree->parent = tree->prev = tree->next = NULL;
}
