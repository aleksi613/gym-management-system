#include "storage.h"

#include <stdio.h>
#include <stdlib.h>

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
