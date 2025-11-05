#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "waste.h"

// ------------------------------- GLOBAL VARIABLES -------------------------
struct Node *root = NULL;
struct Request *front = NULL;
struct Request *rear = NULL;

// ------------------------------- QUEUE FUNCTIONS -------------------------

void enqueueRequest(char location[], int amount) {
    struct Request *newNode = (struct Request*)malloc(sizeof(struct Request));
    strcpy(newNode->location, location);
    newNode->wasteAmount = amount;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }

    printf("Your request for %s (%d kg) added successfully!\n", location, amount);
}

void displayRequests() {
    if (front == NULL) {
        printf("No pending waste collection requests.\n");
        return;
    }

    printf("\n--- Pending Requests ---\n");
    struct Request *temp = front;
    while (temp != NULL) {
        printf("Location: %-15s | Waste: %d kg\n", temp->location, temp->wasteAmount);
        temp = temp->next;
    }
}

void deleteRequest(char location[]) {
    if (front == NULL) {
        printf("No requests to delete.\n");
        return;
    }

    struct Request *temp = front, *prev = NULL;

    while (temp != NULL && strcmp(temp->location, location) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Request for %s not found.\n", location);
        return;
    }

    if (temp == front) {
        front = front->next;
        if (front == NULL) rear = NULL;
    } else {
        prev->next = temp->next;
        if (temp == rear) rear = prev;
    }

    free(temp);
    printf("Request for %s has been deleted successfully.\n", location);
}

// ------------------------------- TREE FUNCTIONS --------------------------

struct Node* createNode(char name[]) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->name, name);
    newNode->wasteAmount = 0;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

void setupCityHierarchy() {
    root = createNode("Central Facility");

    struct Node *north = createNode("North Zone");
    struct Node *south = createNode("South Zone");

    root->firstChild = north;
    north->nextSibling = south;

    struct Node *ward1 = createNode("Ward1");
    struct Node *ward2 = createNode("Ward2");
    north->firstChild = ward1;
    ward1->nextSibling = ward2;

    struct Node *houseA = createNode("HouseA");
    struct Node *houseB = createNode("HouseB");
    ward1->firstChild = houseA;
    houseA->nextSibling = houseB;

    struct Node *houseC = createNode("HouseC");
    ward2->firstChild = houseC;

    struct Node *ward3 = createNode("Ward3");
    south->firstChild = ward3;

    struct Node *houseD = createNode("HouseD");
    ward3->firstChild = houseD;
}

struct Node* findNode(struct Node *root, char name[]) {
    if (root == NULL) return NULL;
    if (strcmp(root->name, name) == 0) return root;

    struct Node *found = findNode(root->firstChild, name);
    if (found != NULL) return found;

    return findNode(root->nextSibling, name);
}

void collectWaste(struct Node *node) {
    if (node == NULL) return;

    struct Node *child = node->firstChild;
    while (child != NULL) {
        collectWaste(child);
        child = child->nextSibling;
    }

    if (node->wasteAmount > 0) {
        printf("Collecting %d kg of waste from %s\n", node->wasteAmount, node->name);
        node->wasteAmount = 0;
    }
}

int printPath(struct Node *node, char location[]) {
    if (node == NULL) return 0;

    if (strcmp(node->name, location) == 0) {
        printf("%s", node->name);
        return 1;
    }

    struct Node *child = node->firstChild;
    while (child != NULL) {
        if (printPath(child, location)) {
            printf(" -> %s", node->name);
            return 1;
        }
        child = child->nextSibling;
    }
    return 0;
}

int calculateTotalWaste(struct Node *node) {
    if (node == NULL) return 0;

    int total = node->wasteAmount;
    struct Node *child = node->firstChild;
    while (child != NULL) {
        total += calculateTotalWaste(child);
        child = child->nextSibling;
    }
    return total;
}
