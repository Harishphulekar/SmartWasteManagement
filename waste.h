#ifndef WASTE_H
#define WASTE_H

// ------------------------------- STRUCTURES -------------------------------

struct Node {
    char name[30];
    int wasteAmount;
    struct Node *firstChild;
    struct Node *nextSibling;
};

struct Request {
    char location[30];
    int wasteAmount;
    struct Request *next;
};

// ------------------------------- GLOBALS ----------------------------------

extern struct Request *front;
extern struct Request *rear;

// ------------------------------- FUNCTION DECLARATIONS --------------------

// Queue functions
void enqueueRequest(char location[], int amount);
void displayRequests();
void deleteRequest(char location[], struct Node *root);

// Tree functions
struct Node* createNode(char name[]);
struct Node* setupCityHierarchy();
struct Node* findNode(struct Node *root, char name[]);
void collectWaste(struct Node *node);
int printPath(struct Node *node, char location[]);
int calculateTotalWaste(struct Node *node);

#endif
