#include "limits.h"
#include "rbt.h"
#include "user.h"
#include "proc.h"

// Function to Create a New RBT Node
struct RBTNode *createRBTNode(struct proc *p) {
    struct RBTNode *newNode = (struct RBTNode *) malloc(sizeof(struct RBTNode));
    if (newNode == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    newNode->process = p;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->color = RED;  // New nodes are always red
    return newNode;
}

// Function to Insert Process in RBT
void insertProcess(struct RBTNode **root, struct proc *p) {
    struct RBTNode *newNode = createRBTNode(p);
    if (newNode == NULL) {
        // Handle node creation failure
        return;
    }

    if (*root == NULL) {
        // If the tree is empty, the new node becomes the root
        newNode->color = BLACK; // Root is always black
        *root = newNode;
    } else {
        // Perform regular BST insertion
        struct RBTNode *current = *root;
        struct RBTNode *parent = NULL;

        while (current != NULL) {
            parent = current;
            if (p->vruntime < current->process->vruntime) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Set the parent of the new node
        newNode->parent = parent;

        if (p->vruntime < parent->process->vruntime) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        // Fix the RBT after insertion
        fixInsertRBT(root, newNode);
    }
}

void fixInsertRBT(struct RBTNode **root, struct RBTNode *k) {
    // Fix the tree by rotating and changing colors
    while (k != *root && k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            struct RBTNode *uncle = k->parent->parent->right;

            // Case : 1
            // The uncle of k is also red
            if (uncle != NULL && uncle->color == RED) {
                k->parent->color = BLACK;
                uncle->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                // Case : 2
                // k is right child of its parent
                // Left-rotate
                if (k == k->parent->right) {
                    k = k->parent;
                    rotateLeft(root, k);
                }

                // Case : 3
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rotateRight(root, k->parent->parent);
            }
        } else {
            // Similar to "if" clause
            // ...
        }
    }

    (*root)->color = BLACK; // Root should always be black
}

void rotateLeft(struct RBTNode **root, struct RBTNode *x) {
    struct RBTNode *y = x->right; // Set y

    x->right = y->left; // Turn y's left subtree into x's right subtree
    if (y->left != NULL) {
        y->left->parent = x;
    }

    y->parent = x->parent; // Link x's parent to y

    if (x->parent == NULL) { // x is root
        *root = y;
    } else if (x == x->parent->left) { // x is left child
        x->parent->left = y;
    } else { // x is right child
        x->parent->right = y;
    }

    y->left = x; // Put x on y's left
    x->parent = y;
}

void rotateRight(struct RBTNode **root, struct RBTNode *y) {
    struct RBTNode *x = y->left; // Set x

    y->left = x->right; // Turn x's right subtree into y's left subtree
    if (x->right != NULL) {
        x->right->parent = y;
    }

    x->parent = y->parent; // Link y's parent to x

    if (y->parent == NULL) { // y is root
        *root = x;
    } else if (y == y->parent->right) { // y is right child
        y->parent->right = x;
    } else { // y is left child
        y->parent->left = x;
    }

    x->right = y; // Put y on x's right
    y->parent = x;
}

// Remove Process with Lowest Runtime

struct proc *getProcess(struct RBTNode **root) {
    // Find the process with the lowest virtual runtime
    struct RBTNode *minNode = findMin(*root);

    if (minNode == NULL) return NULL;

    struct proc *proc = minNode->process;

    // Remove this node from RBT
    deleteNode(root, minNode);

    return proc;
}

// Find the node with the minimum value
struct RBTNode *findMin(struct RBTNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Function to delete a node from the RBT
void deleteNode(struct RBTNode **root, struct RBTNode *z) {
    struct RBTNode *y = z;
    struct RBTNode *x;
    bool originalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        rbTransplant(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        rbTransplant(root, z, z->left);
    } else {
        y = findMin(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbTransplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (originalColor == BLACK) {
        fixDeleteRBT(root, x);
    }
}

void rbTransplant(struct RBTNode **root, struct RBTNode *u, struct RBTNode *v) {
    if (u->parent == NULL) {
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != NULL) {
        v->parent = u->parent;
    }
}


// Function to fix the RBT after deletion
void fixDeleteRBT(struct RBTNode **root, struct RBTNode *x) {
    while (x != *root && getColor(x) == BLACK) {
        if (x == x->parent->left) {
            struct RBTNode *w = x->parent->right;

            if (getColor(w) == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(root, x->parent);
                w = x->parent->right;
            }

            if (getColor(w->left) == BLACK && getColor(w->right) == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (getColor(w->right) == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(root, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(root, x->parent);
                x = *root;
            }
        } else {
            struct RBTNode *w = x->parent->left;

            if (getColor(w) == BLACK) {
                w->color = RED;
                x->parent->color = BLACK;
                rotateRight(root, x->parent);
                w = x->parent->left;
            }

            if (getColor(w->right) == RED && getColor(w->left) == RED) {
                w->color = BLACK;
                x = x->parent;
            } else {
                if (getColor(w->left) == RED) {
                    w->right->color = RED;
                    w->color = BLACK;
                    rotateLeft(root, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = RED;
                w->right->color = RED;
                rotateRight(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

bool getColor(struct RBTNode* node) {
    if (node == NULL) {
        return BLACK;
    }
    return node->color;
}

int calculateWeight(int niceValue) {
    int weight = MAX_WEIGHT - (niceValue * WEIGHT_FACTOR);

    // Ensuring weight is within a sensible range
    if (weight < MIN_WEIGHT) {
        weight = MIN_WEIGHT;
    }

    return weight;
}

void updateVirtualRuntime(struct proc* p) {
    if (p == NULL) {
        return; // Safety check to ensure the process is valid
    }

    int weight = calculateWeight(p->nice); // Assuming 'nice' is the nice value of the process
    if (weight <= 0) {
        weight = 1; // To prevent division by zero or negative weight
    }

    // Increment virtual runtime by time quantum scaled by weight
    p->vruntime += TIME_QUANTUM * weight;

    // Optional: Check for overflow of vruntime
    if (p->vruntime < 0) {
        p->vruntime = INT_MAX; // Resetting to the max value to prevent overflow
    }
}