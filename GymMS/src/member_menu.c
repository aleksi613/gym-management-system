#include "menus.h"

#include "date_utils.h"
#include "member.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

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
            // Add member logic here
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
            // List members logic here
            listMembers(memberList);
            break;
        case 3: {
            // Find member logic here
            searchMembers(memberList);
            break;
        }
        case 4: {
            // Update member details logic here
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
                // Implement update logic here
                // For example, prompt which field to update
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
                            getchar(); // consume newline

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

                            // Validate date of birth
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
            // Delete member logic here
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
