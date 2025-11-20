#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "waste.h"

// ------------------------------- GLOBAL VARIABLES -------------------------

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


void deleteRequest(char location[], struct Node *root) {
    if (front == NULL) {
        printf("No requests to delete.\n");
        return;
    }

    struct Request *temp = front, *prev = NULL;

    // Find the FIRST matching request
    while (temp != NULL && strcmp(temp->location, location) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Request for %s not found.\n", location);
        return;
    }

    // Remove from queue
    if (temp == front) {
        front = front->next;
        if (front == NULL) rear = NULL;
    } else {
        prev->next = temp->next;
        if (temp == rear) rear = prev;
    }

    // FIX: Subtract ONLY this request's waste
    struct Node *node = findNode(root, location);
    if (node != NULL) {
        node->wasteAmount -= temp->wasteAmount;
        if (node->wasteAmount < 0)
            node->wasteAmount = 0;   // Safety check
    }

    printf("Request for %s has been deleted successfully.\n", location);

    free(temp);
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

struct Node* setupCityHierarchy() {
    // Root node
    struct Node *root_local = createNode("Central Facility");

    // Zones
    struct Node *north = createNode("North Zone");
    struct Node *south = createNode("South Zone");

    root->firstChild = north;
    north->nextSibling = south;

    // ========== NORTH ZONE ==========
    // Ward1
    struct Node *ward1 = createNode("Ward1");
    // Ward2
    struct Node *ward2 = createNode("Ward2");
    north->firstChild = ward1;
    ward1->nextSibling = ward2;

    // Ward1 Houses
    struct Node *houseA = createNode("HouseA");
    struct Node *houseB = createNode("HouseB");
    struct Node *houseC = createNode("HouseC");
    ward1->firstChild = houseA;
    houseA->nextSibling = houseB;
    houseB->nextSibling = houseC;

    // Ward2 Houses
    struct Node *houseD = createNode("HouseD");
    struct Node *houseE = createNode("HouseE");
    struct Node *houseF = createNode("HouseF");
    ward2->firstChild = houseD;
    houseD->nextSibling = houseE;
    houseE->nextSibling = houseF;

    // ========== SOUTH ZONE ==========
    // Ward3
    struct Node *ward3 = createNode("Ward3");
    // Ward4
    struct Node *ward4 = createNode("Ward4");
    south->firstChild = ward3;
    ward3->nextSibling = ward4;

    // Ward3 Houses
    struct Node *houseG = createNode("HouseG");
    struct Node *houseH = createNode("HouseH");
    struct Node *houseI = createNode("HouseI");
    ward3->firstChild = houseG;
    houseG->nextSibling = houseH;
    houseH->nextSibling = houseI;

    // Ward4 Houses
    struct Node *houseJ = createNode("HouseJ");
    struct Node *houseK = createNode("HouseK");
    struct Node *houseL = createNode("HouseL");
    ward4->firstChild = houseJ;
    houseJ->nextSibling = houseK;
    houseK->nextSibling = houseL;
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
