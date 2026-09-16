#include "member.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

void printMember(const Member *member){
    printf("Member ID: %d\n", member->memberID);
    printf("First Name: %s\n", member->firstName);
    printf("Last Name: %s\n", member->lastName);
    printf("Phone Number: %s\n", member->phoneNum);
    printf("Gender: %c\n", member->gender);
    printf("Emergency Contact Name: %s\n", member->emergencyName);
    printf("Emergency Contact Phone: %s\n", member->emergencyPhone);
    printf("Emergency Contact Relation: %s\n", member->emergencyRelation);
    printf("Date of Birth: %02d/%02d/%04d\n", member->dob.day, member->dob.month, member->dob.year);
    printf("-------------------------------\n");
}

void addMember(MemberList *list, Member *member){
    // Check if list is full
    if(list->count == list->capacity){
        list->capacity *= 2;

        // Reallocate memory for new capacity
        list->members = realloc(list->members, list->capacity * sizeof(Member));
        if(list->members == NULL){
            // Handle memory allocation failure
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }

    // Add new member
    list->members[list->count] = *member;
    list->count++;
}

void deleteMember(MemberList *list, int memberID){

    int foundIndex = -1;

    for (int i = 0; i < list->count; i++){
        if(list->members[i].memberID == memberID){
            foundIndex = i;
            break;
        }
    }

    // Member not found if foundIndex = -1
    if (foundIndex == -1){
        printf("Member with ID %d not found.\n", memberID);
        return;
    }

    // Shift all subsequent members to the left by 1
    for(int i = foundIndex; i < list->count - 1; i++){
        list->members[i] = list->members[i+1];
    }

    // Decrement the member count since a member was deleted
    list->count--;

    // Check if capacity should be reduced for efficiency when member count falls below half the capacity
    if (list->count > 0 && list->count <= list->capacity / 2){
        // Halve capacity
        list->capacity /= 2;

        list->members = realloc(list->members, list->capacity * sizeof(Member));

        if(list->members == NULL){
            printf("Memory reallocation failed!\n");
            exit(1); // Handle reallocation failure
        }
    }
}

void listMembers(MemberList *list){
    // Check for an empty list
    if (list->count == 0){
        printf("There are no current members in the database\n");
        return;
    }

    for(int i = 0; i < list->count; i++){
        printMember(&list->members[i]);
    }
}

Member* findMemberByID(MemberList *list, int memberID){
    for(int i = 0; i <list->count; i++){
        if(list->members[i].memberID == memberID){
            return &list->members[i];
        }
    }

    // If member not found
    return NULL;
}

void searchMembers(MemberList *list) {
    if (list->count == 0) {
        printf("No members found in the database.\n");
        return;
    }

    int searchChoice;
    printf("Search by:\n");
    printf("1. Member ID\n");
    printf("2. First Name\n");
    printf("3. Last Name\n");
    printf("4. Both First and Last Name\n");
    printf("Enter your choice (1-4): ");
    if (scanf("%d", &searchChoice) != 1 || searchChoice < 1 || searchChoice > 4) {
        printf("Invalid choice.\n");
        while (getchar() != '\n');
        return;
    }
    getchar(); // consume newline

    switch (searchChoice) {
        case 1: {
            // Search by Member ID
            printf("Enter member ID to search: ");
            int searchID;
            if (scanf("%d", &searchID) != 1) {
                printf("Invalid input. Please enter a valid member ID.\n");
                while (getchar() != '\n');
                return;
            }
            getchar();

            Member *foundMember = findMemberByID(list, searchID);
            if (foundMember != NULL) {
                printMember(foundMember);
            } else {
                printf("Member with ID %d not found.\n", searchID);
            }
            break;
        }
        case 2: {
            // Search by First Name
            char firstName[50];
            printf("Enter first name to search: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = '\0';

            int found = 0;
            for (int i = 0; i < list->count; i++) {
                if (strcasecmp(list->members[i].firstName, firstName) == 0) {
                    printMember(&list->members[i]);
                    found = 1;
                }
            }
            if (!found) {
                printf("No members found with the first name '%s'.\n", firstName);
            }
            break;
        }
        case 3: {
            // Search by Last Name
            char lastName[50];
            printf("Enter last name to search: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = '\0';

            int found = 0;
            for (int i = 0; i < list->count; i++) {
                if (strcasecmp(list->members[i].lastName, lastName) == 0) {
                    printMember(&list->members[i]);
                    found = 1;
                }
            }
            if (!found) {
                printf("No members found with the last name '%s'.\n", lastName);
            }
            break;
        }
        case 4: {
            // Search by Both First and Last Name
            char firstName[50], lastName[50];
            printf("Enter first name to search: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = '\0';

            printf("Enter last name to search: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = '\0';

            int found = 0;
            for (int i = 0; i < list->count; i++) {
                if (strcasecmp(list->members[i].firstName, firstName) == 0 &&
                    strcasecmp(list->members[i].lastName, lastName) == 0) {
                    printMember(&list->members[i]);
                    found = 1;
                }
            }
            if (!found) {
                printf("No members found with the name '%s %s'.\n", firstName, lastName);
            }
            break;
        }
        default:
            printf("Invalid choice.\n");
    }
}
