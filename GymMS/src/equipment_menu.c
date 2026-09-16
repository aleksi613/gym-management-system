#include "menus.h"

#include "date_utils.h"
#include "equipment.h"

#include <stdio.h>
#include <string.h>

void equipmentManagementMenu(EquipmentList *equipmentList, int *nextEquipmentID) {
    int choice;
    do {
        printf("==========================================\n");
        printf("        Equipment Management\n");
        printf("==========================================\n");
        printf("1. Add New Equipment\n");
        printf("2. List All Equipment\n");
        printf("3. Update Equipment Status\n");
        printf("4. Delete Equipment\n");
        printf("5. Back to Main Menu\n");
        printf("==========================================\n");
        printf("Enter your choice (1-5): \n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1-5.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();  // Consume the newline character left in the buffer

        switch(choice) {
            case 1: {
                // Add new equipment
                Equipment newEquipment;
                newEquipment.id = (*nextEquipmentID)++; // Assign a unique ID

                // Input Equipment Name
                while (1) {
                    printf("Enter equipment name: ");
                    fgets(newEquipment.name, sizeof(newEquipment.name), stdin);
                    newEquipment.name[strcspn(newEquipment.name, "\n")] = '\0';
                    if (strlen(newEquipment.name) < 1) {
                        printf("Equipment name cannot be empty.\n");
                    } else {
                        break;
                    }
                }

                // Input Total Quantity
                while (1) {
                    printf("Enter total quantity: ");
                    if (scanf("%d", &newEquipment.totalQuantity) != 1 || newEquipment.totalQuantity <= 0) {
                        printf("Invalid input. Please enter a positive number.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    getchar();
                    break;
                }

                if (newEquipment.totalQuantity == 1) {
                    // Ask if the equipment is functional
                    int functionalChoice;
                    while (1) {
                        printf("Is the equipment functional?\n");
                        printf("1. Yes\n");
                        printf("2. No\n");
                        printf("Choose an option (1-2): ");
                        if (scanf("%d", &functionalChoice) != 1 || (functionalChoice !=1 && functionalChoice !=2)) {
                            printf("Invalid input. Please enter 1 or 2.\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        getchar();
                        if (functionalChoice == 1) {
                            newEquipment.functional = 1;
                            newEquipment.broken = 0;
                            break;
                        } else {
                            newEquipment.functional = 0;
                            newEquipment.broken = 1;
                            break;
                        }
                    }
                } else {
                    // For multiple quantities
                    int allFunctionalChoice;
                    while (1) {
                        printf("Are all equipments functional?\n");
                        printf("1. Yes\n");
                        printf("2. No\n");
                        printf("Choose an option (1-2): ");
                        if (scanf("%d", &allFunctionalChoice) != 1 || (allFunctionalChoice !=1 && allFunctionalChoice !=2)) {
                            printf("Invalid input. Please enter 1 or 2.\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        getchar();
                        if (allFunctionalChoice == 1) {
                            newEquipment.functional = newEquipment.totalQuantity;
                            newEquipment.broken = 0;
                            break;
                        } else {
                            // Ask for the number of broken equipments
                            while (1) {
                                printf("Enter the number of broken equipments: ");
                                if (scanf("%d", &newEquipment.broken) !=1 || newEquipment.broken <0 || newEquipment.broken > newEquipment.totalQuantity) {
                                    printf("Invalid input. Please enter a number between 0 and %d.\n", newEquipment.totalQuantity);
                                    while (getchar() != '\n');
                                    continue;
                                }
                                getchar();
                                newEquipment.functional = newEquipment.totalQuantity - newEquipment.broken;
                                break;
                            }
                            break;
                        }
                    }
                }

                // Compute status based on functional and broken
                if (newEquipment.broken > 0) {
                    strcpy(newEquipment.status, "Under Maintenance");

                    // Prompt for repair ETA
                    while (1) {
                        printf("Enter repair ETA (dd mm yyyy): ");
                        if (scanf("%d %d %d", &newEquipment.repairETA.day, &newEquipment.repairETA.month, &newEquipment.repairETA.year) != 3) {
                            printf("Invalid date format. Please enter day month year as numbers.\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        getchar();

                        // Validate the date
                        Date currentDate = getCurrentDate();

                        if (!isValidDate(newEquipment.repairETA.day, newEquipment.repairETA.month, newEquipment.repairETA.year)) {
                            printf("Invalid date entered. Please enter a valid date.\n");
                            continue;
                        }

                        Date repairETA = newEquipment.repairETA;

                        int cmpResult = compareDates(repairETA, currentDate);
                        if (cmpResult < 0) {
                            printf("Error: Repair ETA cannot be in the past.\n");
                        } else {
                            break;
                        }
                    }
                } else {
                    strcpy(newEquipment.status, "Operational");
                    // Set repairETA to 0
                    newEquipment.repairETA.day = 0;
                    newEquipment.repairETA.month = 0;
                    newEquipment.repairETA.year = 0;
                }

                // Add equipment to list
                addEquipment(equipmentList, &newEquipment);

                printf("Equipment added successfully!\n");
                break;
            }
            case 2:
                // List All Equipment
                if (equipmentList->count == 0) {
                    printf("No equipment found.\n");
                } else {
                    for (int i = 0; i < equipmentList->count; i++) {
                        Equipment *e = &equipmentList->equipments[i];
                        printf("Equipment ID: %d\n", e->id);
                        printf("Name: %s\n", e->name);
                        printf("Total Quantity: %d\n", e->totalQuantity);
                        printf("Functional: %d\n", e->functional);
                        printf("Broken: %d\n", e->broken);
                        printf("Status: %s\n", e->status);
                        if (strcmp(e->status, "Under Maintenance") == 0) {
                            printf("Repair ETA: %02d/%02d/%04d\n", e->repairETA.day, e->repairETA.month, e->repairETA.year);
                        }
                        printf("--------------------------------\n");
                    }
                }
                break;
            case 3: {
                // Update Equipment Status
                printf("Enter equipment ID to update: ");
                int equipmentID;
                if (scanf("%d", &equipmentID) != 1) {
                    printf("Invalid input. Please enter a valid equipment ID.\n");
                    while (getchar() != '\n');
                    break;
                }
                getchar();

                // Find equipment
                Equipment *equipmentToUpdate = NULL;
                for (int i = 0; i < equipmentList->count; i++) {
                    if (equipmentList->equipments[i].id == equipmentID) {
                        equipmentToUpdate = &equipmentList->equipments[i];
                        break;
                    }
                }

                if (equipmentToUpdate != NULL) {
                    updateEquipmentStatus(equipmentToUpdate);
                } else {
                    printf("Equipment with ID %d not found.\n", equipmentID);
                }
                break;
            }
            case 4: {
                // Delete Equipment
                printf("Enter equipment ID to delete: ");
                int deleteID;
                if (scanf("%d", &deleteID) != 1) {
                    printf("Invalid input. Please enter a valid equipment ID.\n");
                    while (getchar() != '\n');
                    break;
                }
                getchar();

                deleteEquipment(equipmentList, deleteID);

                printf("Equipment deleted successfully!\n");
                break;
            }
            case 5:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);
}
