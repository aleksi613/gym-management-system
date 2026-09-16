#include "config.h"
#include "menus.h"
#include "storage.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    int intChoice;

    // Initialize MemberList
    MemberList memberList;
    memberList.count = 0;
    memberList.capacity = 10; // initial capacity
    memberList.members = malloc(memberList.capacity * sizeof(Member));
    if (memberList.members == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Initialize EquipmentList
    EquipmentList equipmentList;
    equipmentList.count = 0;
    equipmentList.capacity = 10; // initial capacity
    equipmentList.equipments = malloc(equipmentList.capacity * sizeof(Equipment));
    if (equipmentList.equipments == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Variables to keep track of next IDs
    int nextMemberID = 1;
    int nextEquipmentID = 1;

    // Load data from files
    loadMembersFromFile(&memberList, MEMBER_FILENAME, &nextMemberID);
    loadEquipmentFromFile(&equipmentList, EQUIPMENT_FILENAME, &nextEquipmentID);

    while(1){
        // main menu
        printf("=============================================\n");
        printf("      Welcome to the Gym Management System!\n");
        printf("=============================================\n");
        printf("1. Member Management\n");
        printf("2. Equipment Management\n");
        printf("3. Reports\n");
        printf("4. Exit\n");
        printf("=============================================\n");
        printf("Enter your choice (1-4):\n");

        // Check if the input is a valid integer
        if (scanf("%d", &intChoice) != 1) {
            printf("Error: Invalid input. Please enter a number between 1 and 4.\n");
            // Clear the input buffer to handle the invalid input
            while (getchar() != '\n');
            continue;
        }

        getchar();

        if(intChoice < 1 || intChoice > 4){
            printf("Invalid input. Please try again.\n");
            continue; // Re-prompt
        }

        switch(intChoice){
            case 1:
                memberManagementMenu(&memberList, &nextMemberID);
                break;
            case 2:
                equipmentManagementMenu(&equipmentList, &nextEquipmentID);
                break;
            case 3:
                reportsMenu(&equipmentList);
                break;
            case 4:
                printf("Exiting program...\n");
                // Save data to files
                saveMembersToFile(&memberList, MEMBER_FILENAME);
                saveEquipmentToFile(&equipmentList, EQUIPMENT_FILENAME);
                // Free allocated memory
                free(memberList.members);
                free(equipmentList.equipments);
                return 0;
        }
    }
}
