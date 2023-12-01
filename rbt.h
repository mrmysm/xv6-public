#include "stdbool.h"
#include "stddef.h"



#define RED true
#define BLACK false
#define TIME_QUANTUM 10
#define MAX_WEIGHT 1024
#define WEIGHT_FACTOR 10
#define MIN_WEIGHT 1

struct RBTNode {
    struct proc *process;
    struct RBTNode *left, *right, *parent;
    bool color; // Red (true) or Black (false)
};

struct RBTNode *createRBTNode(struct proc *p);
void insertProcess(struct RBTNode **root, struct proc *p);
void fixInsertRBT(struct RBTNode **root, struct RBTNode *k);
int calculateWeight(int niceValue);
void rotateLeft(struct RBTNode **root, struct RBTNode *x);
void rotateRight(struct RBTNode **root, struct RBTNode *y);
struct proc *getProcess(struct RBTNode **root);
struct RBTNode *findMin(struct RBTNode *node);
void deleteNode(struct RBTNode **root, struct RBTNode *z);
void rbTransplant(struct RBTNode **root, struct RBTNode *u, struct RBTNode *v);
void fixDeleteRBT(struct RBTNode **root, struct RBTNode *x);
bool getColor(struct RBTNode* node);
void updateVirtualRuntime(struct proc* p);