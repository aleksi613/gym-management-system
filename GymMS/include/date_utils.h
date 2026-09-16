#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include "gym_types.h"

Date getCurrentDate(void);
int isValidDate(int day, int month, int year);
int compareDates(Date d1, Date d2);
int calculateAge(Date dob, Date currentDate);

#endif
