#include "menus.h"

#include "equipment.h"

#include <stdio.h>

void reportsMenu(EquipmentList *equipmentList) {
    int choice;
    do {
        printf("==========================================\n");
        printf("            Reports\n");
        printf("==========================================\n");
        printf("1. Generate Equipment Report\n");
        printf("2. Back to Main Menu\n");
        printf("==========================================\n");
        printf("Enter your choice (1-2): \n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();  // Consume the newline character left in the buffer

        switch(choice) {
            case 1: {
                // Generate report logic here
                Report report;
                generateReport(equipmentList, &report);
                break;
            }
            case 2:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 2);
}
