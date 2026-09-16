#include "equipment.h"

#include "date_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addEquipment(EquipmentList *list, Equipment *equipment){
    // Check if list is full
    if(list->count == list->capacity){
        list->capacity *= 2;

        // Reallocate memory for new capacity
        list->equipments = realloc(list->equipments, list->capacity * sizeof(Equipment));

        if(list->equipments == NULL){
            printf("Memory Allocation Failed!\n");
            exit(1);
        }
    }

    list->equipments[list->count] = *equipment;
    list->count++;
}

void deleteEquipment(EquipmentList *list, int equipmentID){
    int foundIndex = -1;

    for (int i = 0; i < list->count; i++){
        if(list->equipments[i].id == equipmentID){
            foundIndex = i;
            break;
        }
    }

    // Equipment not found if foundIndex = -1
    if (foundIndex == -1){
        printf("Equipment with ID %d not found.\n", equipmentID);
        return;
    }

    // Shift all subsequent equipments to the left by 1
    for(int i = foundIndex; i < list->count - 1; i++){
        list->equipments[i] = list->equipments[i+1];
    }

    // Decrement count
    list->count--;

    // Check if capacity should be reduced for efficiency
    if (list->count > 0 && list->count <= list->capacity / 2){
        // Halve capacity
        list->capacity /= 2;

        list->equipments = realloc(list->equipments, list->capacity * sizeof(Equipment));

        if(list->equipments == NULL){
            printf("Memory reallocation failed!\n");
            exit(1); // Handle reallocation failure
        }
    }
}

void updateEquipmentStatus(Equipment *equipment) {
    printf("Current status: %s\n", equipment->status);

    // Get the current date
    Date currentDate = getCurrentDate();

    // Present a menu for status selection
    int statusChoice;
    while (1) {
        printf("Select new status:\n");
        printf("1. Operational\n");
        printf("2. Under Maintenance\n");
        printf("Choose an option (1-2): ");
        if (scanf("%d", &statusChoice) != 1) {
            printf("Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar(); // Consume newline

        if (statusChoice == 1 || statusChoice == 2) {
            break;
        } else {
            printf("Invalid choice. Please select 1 or 2.\n");
        }
    }

    if (statusChoice == 1) {
        strcpy(equipment->status, "Operational");
        // Set repairETA to 0
        equipment->repairETA.day = 0;
        equipment->repairETA.month = 0;
        equipment->repairETA.year = 0;
    } else {
        strcpy(equipment->status, "Under Maintenance");

        while (1) { // Loop until a valid date is entered
            printf("Enter the repair ETA (dd mm yyyy): ");
            int day, month, year;
            if (scanf("%d %d %d", &day, &month, &year) != 3) {
                printf("Error: Invalid date format. Please enter the day, month, and year as integers.\n");
                // Clear the input buffer in the event of an invalid entry
                while (getchar() != '\n'); // Discard invalid input
                continue; // Re-prompt user
            }
            getchar();

            // Validate the date
            if (!isValidDate(day, month, year)) {
                printf("Error: Invalid date entered. Please enter a valid date.\n");
                continue;
            }

            Date repairETA = { day, month, year };

            int cmpResult = compareDates(repairETA, currentDate);
            if (cmpResult < 0) {
                printf("Error: Repair ETA cannot be in the past.\n");
            } else {
                // Update the repair ETA date
                equipment->repairETA.day = day;
                equipment->repairETA.month = month;
                equipment->repairETA.year = year;
                break; // Exit the loop if the date is valid
            }
        }
    }

    printf("The equipment status has been successfully updated.\n");
}

void generateReport(EquipmentList *list, Report *report){

    // Initialize report fields
    report->total_equipment_count = 0;
    report->total_functional_equipment = 0;
    report->total_broken_equipment = 0;

    for(int i = 0; i < list->count; i++){
        report->total_equipment_count += list->equipments[i].totalQuantity;
        report->total_functional_equipment += list->equipments[i].functional;
        report->total_broken_equipment += list->equipments[i].broken;
    }

    // Set report date to current live date
    report->report_date = getCurrentDate();

    // Generate report summary
    snprintf(report->summary, sizeof(report->summary),
        "Total Equipment: %d\nFunctional Equipment: %d\nBroken Equipment: %d\n",
        report->total_equipment_count,
        report->total_functional_equipment,
        report->total_broken_equipment);

    // Displays report
    printf("Report Date: %02d/%02d/%04d\n", report->report_date.day, report->report_date.month, report->report_date.year);
    printf("%s", report->summary);

}
