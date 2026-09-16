#include "date_utils.h"
#include "equipment.h"
#include "member.h"
#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_MEMBERS_FILE "tests/.test_members.dat"
#define TEST_EQUIPMENT_FILE "tests/.test_equipment.dat"

static int assertions_run = 0;
static int assertions_failed = 0;

#define ASSERT_TRUE(condition)                                                   \
    do {                                                                         \
        assertions_run++;                                                        \
        if (!(condition)) {                                                      \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            assertions_failed++;                                                 \
        }                                                                        \
    } while (0)

#define ASSERT_INT_EQUAL(expected, actual)                                      \
    do {                                                                         \
        int expected_value = (expected);                                         \
        int actual_value = (actual);                                             \
        assertions_run++;                                                        \
        if (expected_value != actual_value) {                                    \
            fprintf(stderr,                                                      \
                    "FAIL %s:%d: expected %d, got %d\n",                       \
                    __FILE__,                                                     \
                    __LINE__,                                                     \
                    expected_value,                                               \
                    actual_value);                                                \
            assertions_failed++;                                                 \
        }                                                                        \
    } while (0)

#define ASSERT_STRING_EQUAL(expected, actual)                                   \
    do {                                                                         \
        const char *expected_value = (expected);                                 \
        const char *actual_value = (actual);                                     \
        assertions_run++;                                                        \
        if (strcmp(expected_value, actual_value) != 0) {                         \
            fprintf(stderr,                                                      \
                    "FAIL %s:%d: expected \"%s\", got \"%s\"\n",                 \
                    __FILE__,                                                     \
                    __LINE__,                                                     \
                    expected_value,                                               \
                    actual_value);                                                \
            assertions_failed++;                                                 \
        }                                                                        \
    } while (0)

static void *checkedMalloc(size_t size) {
    void *memory = malloc(size);

    if (memory == NULL) {
        fprintf(stderr, "Test setup failed: unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    return memory;
}

static void testDateUtilities(void) {
    Date earlier = {31, 12, 2025};
    Date later = {1, 1, 2026};
    Date birthday = {17, 9, 2000};
    Date beforeBirthday = {16, 9, 2026};
    Date onBirthday = {17, 9, 2026};
    Date current = getCurrentDate();

    ASSERT_TRUE(isValidDate(29, 2, 2024));
    ASSERT_TRUE(!isValidDate(29, 2, 2023));
    ASSERT_TRUE(!isValidDate(31, 4, 2026));
    ASSERT_TRUE(!isValidDate(1, 13, 2026));
    ASSERT_TRUE(!isValidDate(1, 1, 1899));
    ASSERT_INT_EQUAL(-1, compareDates(earlier, later));
    ASSERT_INT_EQUAL(0, compareDates(later, later));
    ASSERT_INT_EQUAL(1, compareDates(later, earlier));
    ASSERT_INT_EQUAL(25, calculateAge(birthday, beforeBirthday));
    ASSERT_INT_EQUAL(26, calculateAge(birthday, onBirthday));
    ASSERT_TRUE(isValidDate(current.day, current.month, current.year));
}

static void testMemberCollection(void) {
    MemberList list = {0, 1, checkedMalloc(sizeof(Member))};
    Member first = {0};
    Member second = {0};

    first.memberID = 10;
    strcpy(first.firstName, "Alex");
    strcpy(first.lastName, "Morgan");

    second.memberID = 20;
    strcpy(second.firstName, "Jamie");
    strcpy(second.lastName, "Taylor");

    addMember(&list, &first);
    addMember(&list, &second);

    ASSERT_INT_EQUAL(2, list.count);
    ASSERT_INT_EQUAL(2, list.capacity);
    ASSERT_STRING_EQUAL("Alex", findMemberByID(&list, 10)->firstName);
    ASSERT_TRUE(findMemberByID(&list, 999) == NULL);

    deleteMember(&list, 10);

    ASSERT_INT_EQUAL(1, list.count);
    ASSERT_INT_EQUAL(1, list.capacity);
    ASSERT_INT_EQUAL(20, list.members[0].memberID);

    free(list.members);
}

static void testEquipmentCollection(void) {
    EquipmentList list = {0, 1, checkedMalloc(sizeof(Equipment))};
    Equipment treadmill = {0};
    Equipment bike = {0};

    treadmill.id = 3;
    strcpy(treadmill.name, "Treadmill");
    treadmill.totalQuantity = 2;
    treadmill.functional = 2;
    strcpy(treadmill.status, "Operational");

    bike.id = 4;
    strcpy(bike.name, "Exercise Bike");
    bike.totalQuantity = 3;
    bike.functional = 2;
    bike.broken = 1;
    strcpy(bike.status, "Under Maintenance");

    addEquipment(&list, &treadmill);
    addEquipment(&list, &bike);

    ASSERT_INT_EQUAL(2, list.count);
    ASSERT_INT_EQUAL(2, list.capacity);
    ASSERT_STRING_EQUAL("Exercise Bike", list.equipments[1].name);

    deleteEquipment(&list, 3);

    ASSERT_INT_EQUAL(1, list.count);
    ASSERT_INT_EQUAL(1, list.capacity);
    ASSERT_INT_EQUAL(4, list.equipments[0].id);

    free(list.equipments);
}

static void testEquipmentReport(void) {
    Equipment equipment[] = {
        {.totalQuantity = 5, .functional = 4, .broken = 1},
        {.totalQuantity = 3, .functional = 1, .broken = 2}
    };
    EquipmentList list = {2, 2, equipment};
    Report report = {0};
    Date today = getCurrentDate();

    generateReport(&list, &report);

    ASSERT_INT_EQUAL(8, report.total_equipment_count);
    ASSERT_INT_EQUAL(5, report.total_functional_equipment);
    ASSERT_INT_EQUAL(3, report.total_broken_equipment);
    ASSERT_INT_EQUAL(0, compareDates(today, report.report_date));
    ASSERT_STRING_EQUAL(
        "Total Equipment: 8\nFunctional Equipment: 5\nBroken Equipment: 3\n",
        report.summary);
}

static void testMissingStorageFiles(void) {
    MemberList members = {0};
    EquipmentList equipment = {0};
    int nextMemberID = 0;
    int nextEquipmentID = 0;

    remove(TEST_MEMBERS_FILE);
    remove(TEST_EQUIPMENT_FILE);

    loadMembersFromFile(&members, TEST_MEMBERS_FILE, &nextMemberID);
    loadEquipmentFromFile(&equipment, TEST_EQUIPMENT_FILE, &nextEquipmentID);

    ASSERT_INT_EQUAL(0, members.count);
    ASSERT_INT_EQUAL(10, members.capacity);
    ASSERT_INT_EQUAL(1, nextMemberID);
    ASSERT_INT_EQUAL(0, equipment.count);
    ASSERT_INT_EQUAL(10, equipment.capacity);
    ASSERT_INT_EQUAL(1, nextEquipmentID);

    free(members.members);
    free(equipment.equipments);
}

static void testStorageRoundTrip(void) {
    Member savedMember = {0};
    Equipment savedEquipment = {0};
    MemberList membersToSave = {1, 1, &savedMember};
    EquipmentList equipmentToSave = {1, 1, &savedEquipment};
    MemberList loadedMembers = {0};
    EquipmentList loadedEquipment = {0};
    int nextMemberID = 0;
    int nextEquipmentID = 0;

    savedMember.memberID = 42;
    strcpy(savedMember.firstName, "Jordan");
    strcpy(savedMember.lastName, "Lee");
    strcpy(savedMember.phoneNum, "6135550101");
    savedMember.gender = 'F';
    savedMember.dob = (Date){12, 3, 1998};

    savedEquipment.id = 7;
    strcpy(savedEquipment.name, "Rowing Machine");
    savedEquipment.totalQuantity = 4;
    savedEquipment.functional = 3;
    savedEquipment.broken = 1;
    strcpy(savedEquipment.status, "Under Maintenance");
    savedEquipment.repairETA = (Date){1, 12, 2030};

    saveMembersToFile(&membersToSave, TEST_MEMBERS_FILE);
    saveEquipmentToFile(&equipmentToSave, TEST_EQUIPMENT_FILE);
    loadMembersFromFile(&loadedMembers, TEST_MEMBERS_FILE, &nextMemberID);
    loadEquipmentFromFile(&loadedEquipment, TEST_EQUIPMENT_FILE, &nextEquipmentID);

    ASSERT_INT_EQUAL(1, loadedMembers.count);
    ASSERT_INT_EQUAL(43, nextMemberID);
    ASSERT_INT_EQUAL(42, loadedMembers.members[0].memberID);
    ASSERT_STRING_EQUAL("Jordan", loadedMembers.members[0].firstName);
    ASSERT_STRING_EQUAL("Lee", loadedMembers.members[0].lastName);
    ASSERT_INT_EQUAL(1998, loadedMembers.members[0].dob.year);

    ASSERT_INT_EQUAL(1, loadedEquipment.count);
    ASSERT_INT_EQUAL(8, nextEquipmentID);
    ASSERT_INT_EQUAL(7, loadedEquipment.equipments[0].id);
    ASSERT_STRING_EQUAL("Rowing Machine", loadedEquipment.equipments[0].name);
    ASSERT_INT_EQUAL(1, loadedEquipment.equipments[0].broken);
    ASSERT_INT_EQUAL(2030, loadedEquipment.equipments[0].repairETA.year);

    free(loadedMembers.members);
    free(loadedEquipment.equipments);
    remove(TEST_MEMBERS_FILE);
    remove(TEST_EQUIPMENT_FILE);
}

int main(void) {
    testDateUtilities();
    testMemberCollection();
    testEquipmentCollection();
    testEquipmentReport();
    testMissingStorageFiles();
    testStorageRoundTrip();

    if (assertions_failed == 0) {
        printf("PASS: %d assertions\n", assertions_run);
        return EXIT_SUCCESS;
    }

    fprintf(stderr,
            "FAIL: %d of %d assertions failed\n",
            assertions_failed,
            assertions_run);
    return EXIT_FAILURE;
}
