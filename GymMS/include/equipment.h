#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "gym_types.h"

void addEquipment(EquipmentList *list, Equipment *equipment);
void deleteEquipment(EquipmentList *list, int equipmentID);
void updateEquipmentStatus(Equipment *equipment);
void generateReport(EquipmentList *list, Report *report);

#endif
