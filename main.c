#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "waste.h"

int main() {
    setupCityHierarchy();
    int choice;

    while (1) {
        printf("\n==== SMART WASTE MANAGEMENT SYSTEM ====\n");
        printf("1. Add Waste Collection Request\n");
        printf("2. Display Pending Requests\n");
        printf("3. Collect Waste from Zones\n");
        printf("4. Find Route to a Location\n");
        printf("5. Display Total Waste in City\n");
        printf("6. Delete a Waste Collection Request\n");
        printf("7. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        char location[30];
        int amount;

        switch (choice) {
            case 1:
                printf("Enter Location (e.g., Ward1/HouseA): ");
                scanf("%s", location);
            
                printf("Enter Waste Amount (kg): ");
                scanf("%d", &amount);
            
                // First validate if location exists in tree
                struct Node *node = findNode(root, location);
            
                if (node == NULL) {
                    printf("⚠ Error: Location '%s' does not exist in city hierarchy. Request NOT added.\n", location);
                    break;  // Do NOT add request
                }
            
                // Now it is safe to enqueue and update tree
                enqueueRequest(location, amount);
                node->wasteAmount += amount;
            
                printf("Your request for %s (%d kg) added successfully!\n", location, amount);
                break;


            case 2:
                displayRequests();
                break;

            case 3:
                printf("Collecting all waste from city...\n");
                collectWaste(root);
                while (front != NULL) {
                    struct Request *temp = front;
                    front = front->next;
                    free(temp);
                }
                rear = NULL;
                printf("Waste collection complete!\n");
                break;

            case 4:
                printf("Enter Location to find route: ");
                scanf("%s", location);
                printf("Path from root to %s: ", location);
                if (!printPath(root, location)) printf("Location not found!\n");
                else printf("\n");
                break;

            case 5:
                printf("Total Uncollected Waste in the City: %d kg\n", calculateTotalWaste(root));
                break;

            case 6:
                printf("Enter location to delete request: ");
                scanf("%s", location);
                deleteRequest(location);
                break;

            case 7:
                printf("Exiting program. Thank you!\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
