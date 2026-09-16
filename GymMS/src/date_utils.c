#include "date_utils.h"

#include <time.h>

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
