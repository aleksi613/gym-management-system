#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MEMBER_FILENAME "members.dat"
#define EQUIPMENT_FILENAME "equipment.dat"

typedef struct{
    int day;
    int month;
    int year;
} Date; 

typedef struct{
    int memberID;
    char firstName[50];
    char lastName[50];
    char phoneNum[15];
    char gender; // M for male and F for female
    char emergencyName[50]; // The emergency contact's full name
    char emergencyPhone[15]; // The emergency contact's phone number
    char emergencyRelation[10]; // Relationship to member
    Date dob;
} Member;

typedef struct{
    int count; // Amount of current members
    int capacity;
    Member *members;
} MemberList;

typedef struct{
    int memberID;
    char membershipType[15]; // Essential, Premium, or Student
    char membershipFormat[15]; // Bi-Weekly payment by default, choice of annual payment for a cheaper rate
    double cost;
    char membershipStatus[10]; // Will always be "Active" unless member terminates membership ("Expired") or gets banned ("Banned")
    Date startDate;
} Membership;

typedef struct{
    char name[50];
    int totalQuantity;
    int functional; // number of functional equipment from totalQuantity
    int broken; // number of equipment that needs repair/broken
    char status[20]; // "Operational" or "Under Maintenance"
    Date repairETA; // Date when the equipment should be operational again
    int id;
} Equipment;

typedef struct {
    int count; // Number of equipment currently in the list
    int capacity; // Maximum capacity before the need to reallocate
    Equipment *equipments; // Ptr to an array of Equipment structs
} EquipmentList;

// Members can notify employees and/or employees can use the system to fill out the report function when made aware of broken equipment
typedef struct{
    int report_ID; // A Case number ID will be attributed to each report made for functionality to search specific report by ID
    Date report_date; // Date the report was generated
    int total_equipment_count; // Total number of equipment in the gym
    int total_functional_equipment; // Total number of functional equipment across all types
    int total_broken_equipment; // Total number of broken equipment across all types
    char summary[200]; // Summary of the report
} Report;

// Used if a member would like to terminate their membership or done by employee due to violation
typedef struct{
    int memberID;
    Date terminationDate;
    char reason[30]; // Reason their membership is being terminated, voluntary, violation, etc.
    char notes [200]; // Notes to explain termination to have on file if necessary
} TerminateMembership;

Date getCurrentDate() {
    Date currentDate;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    currentDate.day = t->tm_mday;
    currentDate.month = t->tm_mon + 1;
    currentDate.year = t->tm_year + 1900;

    return currentDate;
}

// Function to check if a date is valid
int isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100)
        return 0;
    if (month < 1 || month > 12)
        return 0;
    int maxDay;

    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            maxDay = 31;
            break;
        case 4: case 6: case 9: case 11:
            maxDay = 30;
            break;
        case 2:
            // Check for leap year
            if ((year % 4 == 0 && year % 100 !=0) || (year % 400 == 0))
                maxDay = 29;
            else
                maxDay = 28;
            break;
        default:
            return 0;
    }
    if (day < 1 || day > maxDay)
        return 0;
    return 1;
}

// Function to compare two dates
int compareDates(Date d1, Date d2) {
    if (d1.year < d2.year)
        return -1;
    else if (d1.year > d2.year)
        return 1;
    else {
        if (d1.month < d2.month)
            return -1;
        else if (d1.month > d2.month)
            return 1;
        else {
            if (d1.day < d2.day)
                return -1;
            else if (d1.day > d2.day)
                return 1;
            else
                return 0;
        }
    }
}

// Function to calculate age
int calculateAge(Date dob, Date currentDate) {
    int age = currentDate.year - dob.year;
    if (currentDate.month < dob.month || (currentDate.month == dob.month && currentDate.day < dob.day)) {
        age--;
    }
    return age;
}

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

void memberManagementMenu(MemberList *memberList, int *nextMemberID) {
    int choice;
    do {
        printf("==========================================\n");
        printf("        Member Management\n");
        printf("==========================================\n");
        printf("1. Add a New Member\n");
        printf("2. List All Members\n");
        printf("3. Find a Member\n");
        printf("4. Update Member Details\n");
        printf("5. Delete a Member\n");
        printf("6. Back to Main Menu\n");
        printf("==========================================\n");
        printf("Enter your choice (1-6): \n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1-6.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();  // Consume the newline character left in the buffer

    switch(choice) {
        case 1: {
            Member newMember;
            newMember.memberID = (*nextMemberID)++;

            // Input First Name
            while (1) {
                printf("Enter first name: ");
                fgets(newMember.firstName, sizeof(newMember.firstName), stdin);
                newMember.firstName[strcspn(newMember.firstName, "\n")] = '\0'; // remove newline

                // Check if name contains only letters and spaces and is at least 2 characters
                int validName = 1;
                if (strlen(newMember.firstName) < 2) {
                    validName = 0;
                } else {
                    for (size_t i = 0; i < strlen(newMember.firstName); i++) {
                        if (!isalpha(newMember.firstName[i]) && !isspace(newMember.firstName[i])) {
                            validName = 0;
                            break;
                        }
                    }
                }

                if (!validName) {
                    printf("Invalid first name. Please enter a valid name with at least 2 letters.\n");
                } else {
                    break;
                }
            }

            // Input Last Name
            while (1) {
                printf("Enter last name: ");
                fgets(newMember.lastName, sizeof(newMember.lastName), stdin);
                newMember.lastName[strcspn(newMember.lastName, "\n")] = '\0'; // remove newline

                // Check if name contains only letters and spaces and is at least 2 characters
                int validName = 1;
                if (strlen(newMember.lastName) < 2) {
                    validName = 0;
                } else {
                    for (size_t i = 0; i < strlen(newMember.lastName); i++) {
                        if (!isalpha(newMember.lastName[i]) && !isspace(newMember.lastName[i])) {
                            validName = 0;
                            break;
                        }
                    }
                }

                if (!validName) {
                    printf("Invalid last name. Please enter a valid name with at least 2 letters.\n");
                } else {
                    break;
                }
            }

            // Input Phone Number
            while (1) {
                printf("Enter phone number (digits only): ");
                fgets(newMember.phoneNum, sizeof(newMember.phoneNum), stdin);
                newMember.phoneNum[strcspn(newMember.phoneNum, "\n")] = '\0';

                // Validate phone number (digits only)
                int validPhone = 1;
                for (size_t i = 0; i < strlen(newMember.phoneNum); i++) {
                    if (!isdigit(newMember.phoneNum[i])) {
                        validPhone = 0;
                        break;
                    }
                }

                if (!validPhone || strlen(newMember.phoneNum) < 7) {
                    printf("Invalid phone number. Please enter digits only, at least 7 digits.\n");
                } else {
                    break;
                }
            }

            // Input Gender
            while (1) {
                printf("Enter gender (M/F): ");
                scanf("%c", &newMember.gender);
                getchar(); // consume newline
                newMember.gender = toupper(newMember.gender);
                if (newMember.gender != 'M' && newMember.gender != 'F') {
                    printf("Invalid gender. Please enter 'M' or 'F'.\n");
                } else {
                    break;
                }
            }

            // Input Emergency Contact Name
            while (1) {
                printf("Enter emergency contact name: ");
                fgets(newMember.emergencyName, sizeof(newMember.emergencyName), stdin);
                newMember.emergencyName[strcspn(newMember.emergencyName, "\n")] = '\0';

                // Validate name
                int validName = 1;
                if (strlen(newMember.emergencyName) < 2) {
                    validName = 0;
                } else {
                    for (size_t i = 0; i < strlen(newMember.emergencyName); i++) {
                        if (!isalpha(newMember.emergencyName[i]) && !isspace(newMember.emergencyName[i])) {
                            validName = 0;
                            break;
                        }
                    }
                }

                if (!validName) {
                    printf("Invalid name. Please enter a valid name with at least 2 letters.\n");
                } else {
                    break;
                }
            }

            // Input Emergency Contact Phone
            while (1) {
                printf("Enter emergency contact phone (digits only): ");
                fgets(newMember.emergencyPhone, sizeof(newMember.emergencyPhone), stdin);
                newMember.emergencyPhone[strcspn(newMember.emergencyPhone, "\n")] = '\0';

                // Validate phone number
                int validPhone = 1;
                for (size_t i = 0; i < strlen(newMember.emergencyPhone); i++) {
                    if (!isdigit(newMember.emergencyPhone[i])) {
                        validPhone = 0;
                        break;
                    }
                }

                if (!validPhone || strlen(newMember.emergencyPhone) < 7) {
                    printf("Invalid phone number. Please enter digits only, at least 7 digits.\n");
                } else {
                    break;
                }
            }

            // Input Emergency Contact Relation
            int relationChoice;
            while (1) {
                printf("Select emergency contact relation:\n");
                printf("1. Spouse\n");
                printf("2. Partner\n");
                printf("3. Friend\n");
                printf("4. Relative\n");
                printf("5. Parent\n");
                printf("6. Other\n");
                printf("Choose an option (1-6): ");
                if (scanf("%d", &relationChoice) != 1) {
                    printf("Invalid input. Please enter a number between 1-6.\n");
                    while (getchar() != '\n');
                    continue;
                }
                getchar(); // consume newline

                if (relationChoice >=1 && relationChoice <=6) {
                    break;
                } else {
                    printf("Invalid choice. Please select a number between 1 and 6.\n");
                }
            }

            const char *relations[] = {"Spouse", "Partner", "Friend", "Relative", "Parent", "Other"};
            strcpy(newMember.emergencyRelation, relations[relationChoice -1]);

            // Prompt for date of birth
            while (1) {
                printf("Enter date of birth (dd mm yyyy): ");
                if (scanf("%d %d %d", &newMember.dob.day, &newMember.dob.month, &newMember.dob.year) != 3) {
                    printf("Invalid date format. Please enter day month year as numbers.\n");
                    while (getchar() != '\n');
                    continue;
                }
                getchar();

                // Validate date of birth
                Date currentDate = getCurrentDate();

                if (!isValidDate(newMember.dob.day, newMember.dob.month, newMember.dob.year)) {
                    printf("Invalid date of birth. Please enter a valid date.\n");
                    continue;
                }

                int age = calculateAge(newMember.dob, currentDate);
                if (age < 13) {
                    printf("Member must be at least 13 years old to register.\n");
                    continue;
                }
                break;
            }

            // Now, add the member to the list
            addMember(memberList, &newMember);

            printf("Member added successfully!\n");
            break;
        }
        case 2:
            listMembers(memberList);
            break;
        case 3: {
            searchMembers(memberList);
            break;
        }
        case 4: {
            printf("Enter member ID to update: ");
            int updateID;
            if (scanf("%d", &updateID) != 1) {
                printf("Invalid input. Please enter a valid member ID.\n");
                while (getchar() != '\n');
                break;
            }
            getchar();

            Member *memberToUpdate = findMemberByID(memberList, updateID);

            if (memberToUpdate != NULL) {
                int updateChoice;
                printf("Which field do you want to update?\n");
                printf("1. First Name\n");
                printf("2. Last Name\n");
                printf("3. Phone Number\n");
                printf("4. Gender\n");
                printf("5. Emergency Contact Name\n");
                printf("6. Emergency Contact Phone\n");
                printf("7. Emergency Contact Relation\n");
                printf("8. Date of Birth\n");
                printf("Enter choice (1-8): ");
                if (scanf("%d", &updateChoice) != 1) {
                    printf("Invalid input. Please enter a number between 1-8.\n");
                    while (getchar() != '\n');
                    break;
                }
                getchar();

                switch (updateChoice) {
                    case 1:
                        while (1) {
                            printf("Enter new first name: ");
                            fgets(memberToUpdate->firstName, sizeof(memberToUpdate->firstName), stdin);
                            memberToUpdate->firstName[strcspn(memberToUpdate->firstName, "\n")] = '\0';

                            int validName = 1;
                            if (strlen(memberToUpdate->firstName) < 2) {
                                validName = 0;
                            } else {
                                for (size_t i = 0; i < strlen(memberToUpdate->firstName); i++) {
                                    if (!isalpha(memberToUpdate->firstName[i]) && !isspace(memberToUpdate->firstName[i])) {
                                        validName = 0;
                                        break;
                                    }
                                }
                            }

                            if (!validName) {
                                printf("Invalid first name. Please enter a valid name with at least 2 letters.\n");
                            } else {
                                break;
                            }
                        }
                        break;
                    case 2:
                        while (1) {
                            printf("Enter new last name: ");
                            fgets(memberToUpdate->lastName, sizeof(memberToUpdate->lastName), stdin);
                            memberToUpdate->lastName[strcspn(memberToUpdate->lastName, "\n")] = '\0';

                            int validName = 1;
                            if (strlen(memberToUpdate->lastName) < 2) {
                                validName = 0;
                            } else {
                                for (size_t i = 0; i < strlen(memberToUpdate->lastName); i++) {
                                    if (!isalpha(memberToUpdate->lastName[i]) && !isspace(memberToUpdate->lastName[i])) {
                                        validName = 0;
                                        break;
                                    }
                                }
                            }

                            if (!validName) {
                                printf("Invalid last name. Please enter a valid name with at least 2 letters.\n");
                            } else {
                                break;
                            }
                        }
                        break;
                    case 3:
                        while (1) {
                            printf("Enter new phone number: ");
                            fgets(memberToUpdate->phoneNum, sizeof(memberToUpdate->phoneNum), stdin);
                            memberToUpdate->phoneNum[strcspn(memberToUpdate->phoneNum, "\n")] = '\0';

                            int validPhone = 1;
                            for (size_t i = 0; i < strlen(memberToUpdate->phoneNum); i++) {
                                if (!isdigit(memberToUpdate->phoneNum[i])) {
                                    validPhone = 0;
                                    break;
                                }
                            }

                            if (!validPhone || strlen(memberToUpdate->phoneNum) < 7) {
                                printf("Invalid phone number. Please enter digits only, at least 7 digits.\n");
                            } else {
                                break;
                            }
                        }
                        break;
                    case 4:
                        while (1) {
                            printf("Enter new gender (M/F): ");
                            scanf("%c", &memberToUpdate->gender);
                            getchar();
                            memberToUpdate->gender = toupper(memberToUpdate->gender);
                            if (memberToUpdate->gender != 'M' && memberToUpdate->gender != 'F') {
                                printf("Invalid gender. Please enter 'M' or 'F'.\n");
                            } else {
                                break;
                            }
                        }
                        break;
                    case 5:
                        while (1) {
                            printf("Enter new emergency contact name: ");
                            fgets(memberToUpdate->emergencyName, sizeof(memberToUpdate->emergencyName), stdin);
                            memberToUpdate->emergencyName[strcspn(memberToUpdate->emergencyName, "\n")] = '\0';

                            int validName = 1;
                            if (strlen(memberToUpdate->emergencyName) < 2) {
                                validName = 0;
                            } else {
                                for (size_t i = 0; i < strlen(memberToUpdate->emergencyName); i++) {
                                    if (!isalpha(memberToUpdate->emergencyName[i]) && !isspace(memberToUpdate->emergencyName[i])) {
                                        validName = 0;
                                        break;
                                    }
                                }
                            }

                            if (!validName) {
                                printf("Invalid name. Please enter a valid name with at least 2 letters.\n");
                            } else {
                                break;
                            }
                        }
                        break;
                    case 6:
                        while (1) {
                            printf("Enter new emergency contact phone: ");
                            fgets(memberToUpdate->emergencyPhone, sizeof(memberToUpdate->emergencyPhone), stdin);
                            memberToUpdate->emergencyPhone[strcspn(memberToUpdate->emergencyPhone, "\n")] = '\0';

                            int validPhone = 1;
                            for (size_t i = 0; i < strlen(memberToUpdate->emergencyPhone); i++) {
                                if (!isdigit(memberToUpdate->emergencyPhone[i])) {
                                    validPhone = 0;
                                    break;
                                }
                            }

                            if (!validPhone || strlen(memberToUpdate->emergencyPhone) < 7) {
                                printf("Invalid phone number. Please enter digits only, at least 7 digits.\n");
                            } else {
                                break;
                            }
                        }
                        break;
                    case 7: {
                        int relationChoice;
                        while (1) {
                            printf("Select new emergency contact relation:\n");
                            printf("1. Spouse\n");
                            printf("2. Partner\n");
                            printf("3. Friend\n");
                            printf("4. Relative\n");
                            printf("5. Parent\n");
                            printf("6. Other\n");
                            printf("Choose an option (1-6): ");
                            if (scanf("%d", &relationChoice) != 1) {
                                printf("Invalid input. Please enter a number between 1-6.\n");
                                while (getchar() != '\n');
                                continue;
                            }
                            getchar();

                            if (relationChoice >=1 && relationChoice <=6) {
                                break;
                            } else {
                                printf("Invalid choice. Please select a number between 1 and 6.\n");
                            }
                        }

                        const char *relations[] = {"Spouse", "Partner", "Friend", "Relative", "Parent", "Other"};
                        strcpy(memberToUpdate->emergencyRelation, relations[relationChoice -1]);
                        break;
                    }
                    case 8:
                        while (1) {
                            printf("Enter new date of birth (dd mm yyyy): ");
                            if (scanf("%d %d %d", &memberToUpdate->dob.day, &memberToUpdate->dob.month, &memberToUpdate->dob.year) != 3) {
                                printf("Invalid date format. Please enter day month year as numbers.\n");
                                while (getchar() != '\n');
                                continue;
                            }
                            getchar();

                            // Validate dob
                            Date currentDate = getCurrentDate();

                            if (!isValidDate(memberToUpdate->dob.day, memberToUpdate->dob.month, memberToUpdate->dob.year)) {
                                printf("Invalid date of birth. Please enter a valid date.\n");
                                continue;
                            }

                            int age = calculateAge(memberToUpdate->dob, currentDate);
                            if (age < 13) {
                                printf("Member must be at least 13 years old to register.\n");
                                continue;
                            }
                            break;
                        }
                        break;
                    default:
                        printf("Invalid choice.\n");
                }

                printf("Member details updated successfully!\n");
            } else {
                printf("Member with ID %d not found.\n", updateID);
            }

            break;
        }
        case 5: {
            printf("Enter member ID to delete: ");
            int deleteID;
            if (scanf("%d", &deleteID) != 1) {
                printf("Invalid input. Please enter a valid member ID.\n");
                while (getchar() != '\n');
                break;
            }
            getchar();

            deleteMember(memberList, deleteID);

            printf("Member deleted successfully!\n");
            break;
        }
        case 6:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

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
        getchar(); 

        switch(choice) {
            case 1: {
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

void saveMembersToFile(MemberList *list, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Write the count
    fwrite(&list->count, sizeof(int), 1, file);

    // Write the members
    fwrite(list->members, sizeof(Member), list->count, file);

    fclose(file);
}

void loadMembersFromFile(MemberList *list, const char *filename, int *nextMemberID) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        // File doesn't exist, initialize empty list
        list->count = 0;
        list->capacity = 10;
        list->members = malloc(list->capacity * sizeof(Member));
        if (list->members == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        *nextMemberID = 1;
        return;
    }

    // Read the count
    fread(&list->count, sizeof(int), 1, file);

    // Ensure the capacity is sufficient
    list->capacity = list->count > 10 ? list->count : 10;
    list->members = malloc(list->capacity * sizeof(Member));
    if (list->members == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    // Read the members
    fread(list->members, sizeof(Member), list->count, file);

    // Update nextMemberID
    *nextMemberID = 1;
    for (int i = 0; i < list->count; i++) {
        if (list->members[i].memberID >= *nextMemberID) {
            *nextMemberID = list->members[i].memberID + 1;
        }
    }

    fclose(file);
}

void saveEquipmentToFile(EquipmentList *list, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening equipment file for writing!\n");
        return;
    }

    // Write the count
    fwrite(&list->count, sizeof(int), 1, file);

    // Write the equipments
    fwrite(list->equipments, sizeof(Equipment), list->count, file);

    fclose(file);
}

void loadEquipmentFromFile(EquipmentList *list, const char *filename, int *nextEquipmentID) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        // File doesn't exist, initialize empty list
        list->count = 0;
        list->capacity = 10;
        list->equipments = malloc(list->capacity * sizeof(Equipment));
        if (list->equipments == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        *nextEquipmentID = 1;
        return;
    }

    // Read the count
    fread(&list->count, sizeof(int), 1, file);

    // Ensure the capacity is sufficient
    list->capacity = list->count > 10 ? list->count : 10;
    list->equipments = malloc(list->capacity * sizeof(Equipment));
    if (list->equipments == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    // Read the equipments
    fread(list->equipments, sizeof(Equipment), list->count, file);

    // Update nextEquipmentID
    *nextEquipmentID = 1;
    for (int i = 0; i < list->count; i++) {
        if (list->equipments[i].id >= *nextEquipmentID) {
            *nextEquipmentID = list->equipments[i].id + 1;
        }
    }

    fclose(file);
}

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
