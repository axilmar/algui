#ifndef ALG_TREE_H
#define ALG_TREE_H


/**
 * A tree data structure.
 */
typedef struct ALG_TREE {
    ///parent.
    struct ALG_TREE* parent;

    ///previous sibling.
    struct ALG_TREE* prev;

    ///next sibling.
    struct ALG_TREE* next;

    ///first child.
    struct ALG_TREE* first;

    ///last child.
    struct ALG_TREE* last;
} ALG_TREE;


/**
 * Checks if the given tree 'b' is within the given tree 'a'.
 * @param a tree to check if it contains 'b'.
 * @param b tree to check if it is contained in 'a'.
 * @return non-zero if 'a' contains 'b', zero otherwise.
 */
int alg_tree_contains(ALG_TREE* a, ALG_TREE* b);


/**
 * Initializes a tree structure.
 * @param tree tree structure to initialize.
 */
void alg_init_tree(ALG_TREE* tree);


/**
 * Returns the root node of the tree.
 * @param tree tree to get the root of.
 * @return the root of the tree.
 */
ALG_TREE* alg_get_root_tree(ALG_TREE* tree);


/**
 * Inserts a tree node into another tree node.
 * @param parent parent tree node.
 * @param child child tree node.
 * @param pos position; if 0, then child is inserted as first; if -1, then child is inserted as last; other positions in-between; negative positions count from last to first.
 */
void alg_insert_tree(ALG_TREE* parent, ALG_TREE* child, int pos);


/**
 * Removes a tree node from its parent, if it has a parent.
 * @param tree tree node to remove from its parent.
 */
void alg_remove_tree(ALG_TREE* tree);


#endif //ALG_TREE_H

