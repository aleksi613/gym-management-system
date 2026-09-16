#ifndef STORAGE_H
#define STORAGE_H

#include "gym_types.h"

void saveMembersToFile(MemberList *list, const char *filename);
void loadMembersFromFile(MemberList *list, const char *filename, int *nextMemberID);
void saveEquipmentToFile(EquipmentList *list, const char *filename);
void loadEquipmentFromFile(EquipmentList *list, const char *filename, int *nextEquipmentID);

#endif
