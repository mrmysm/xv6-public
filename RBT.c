#define RED true
#define BLACK false

struct RBTNode {
    struct proc* process;
    struct RBTNode *left, *right, *parent;
    bool color;
};
Function to Create a New RBT Node

struct RBTNode* createRBTNode(struct proc* p) {
    struct RBTNode* newNode = (struct RBTNode*)malloc(sizeof(struct RBTNode));
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

void insertProcess(struct RBTNode** root, struct proc* p) {
    struct RBTNode* newNode = createRBTNode(p);
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
        struct RBTNode* current = *root;
        struct RBTNode* parent = NULL;

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