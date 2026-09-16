#ifndef GYM_TYPES_H
#define GYM_TYPES_H

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int memberID;
    char firstName[50];
    char lastName[50];
    char phoneNum[15];
    char gender;
    char emergencyName[50];
    char emergencyPhone[15];
    char emergencyRelation[10];
    Date dob;
} Member;

typedef struct {
    int count;
    int capacity;
    Member *members;
} MemberList;

typedef struct {
    int memberID;
    char membershipType[15];
    char membershipFormat[15];
    double cost;
    char membershipStatus[10];
    Date startDate;
} Membership;

typedef struct {
    char name[50];
    int totalQuantity;
    int functional;
    int broken;
    char status[20];
    Date repairETA;
    int id;
} Equipment;

typedef struct {
    int count;
    int capacity;
    Equipment *equipments;
} EquipmentList;

typedef struct {
    int report_ID;
    Date report_date;
    int total_equipment_count;
    int total_functional_equipment;
    int total_broken_equipment;
    char summary[200];
} Report;

typedef struct {
    int memberID;
    Date terminationDate;
    char reason[30];
    char notes[200];
} TerminateMembership;

#endif
