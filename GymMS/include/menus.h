#ifndef MENUS_H
#define MENUS_H

#include "gym_types.h"

void memberManagementMenu(MemberList *memberList, int *nextMemberID);
void equipmentManagementMenu(EquipmentList *equipmentList, int *nextEquipmentID);
void reportsMenu(EquipmentList *equipmentList);

#endif
