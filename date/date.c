#include "date.h"
#include <stdlib.h>
#include <stdio.h>

static bool date_is_valid_ymd(int y, int m, int d) {
    // Year 0 is invalid
    if (y == 0 || m <= 0 || m > 12 || d <= 0) {
        return false;
    }

    // Array to hold the number of days in each month
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) {
        days_in_month[2] = 29; // February has 29 days in a leap year
    }

    return d <= days_in_month[m];
}

static int days_in_month(int year, int month) {
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
        return 29;
    } 
    else {
        return days_in_month[month];
    }
}

bool date_is_valid(const Date* date) {
    if (date == NULL) {
        perror("date is null");
    }
    return date_is_valid_ymd(date->year, date->month, date->month);
}

Date* date_create() {
    Date* date = (Date*)malloc(sizeof(Date));
    if (date) {
        // Setting invalid values to signify a null (invalid) date
        date->year = -1;
        date->month = -1;
        date->day = -1;
    }
    if (date == NULL) {
        perror("can not allocate memory for date");
        exit(-1);
    }
    return date;
}

Date* date_create_ymd(int y, int m, int d) {
    Date* date = (Date*)malloc(sizeof(Date));
    if (date) {
        if (date_is_valid_ymd(y, m, d)) {
            date->year = y;
            date->month = m;
            date->day = d;
        } 
        else {
            // Setting invalid values for an invalid date
            date->year = -1;
            date->month = -1;
            date->day = -1;
        }
    }
    return date;
}

bool date_is_null(const Date* date) {
    if (date == NULL) {
        perror("date is null");
        return true;
    }
    return false;
}

Date* date_add_days(const Date* orig_date, int ndays) {
    if (orig_date == NULL || !date_is_valid_ymd(orig_date->year, orig_date->month, orig_date->day)) {
        perror("date is null or not valid");
        exit(-1);
    }

    Date* new_date = (Date*)malloc(sizeof(Date));
    if (new_date == NULL) {
        perror("Cannot allocate memory for new date");
        exit(-1);
    }

    // Copy original date to the new date
    *new_date = *orig_date;

    while (ndays != 0) {
        if (ndays > 0) {
            // Adding days
            int days_this_month = days_in_month(new_date->year, new_date->month);
            if (new_date->day + ndays > days_this_month) {
                ndays -= (days_this_month - new_date->day + 1);
                new_date->day = 1;
                if (++new_date->month > 12) {
                    new_date->month = 1;
                    new_date->year++;
                }
            } 
            else {
                new_date->day += ndays;
                ndays = 0;
            }
        } 
        else {
            // Subtracting days
            if (new_date->day + ndays < 1) {
                ndays += new_date->day;
                if (--new_date->month < 1) {
                    new_date->month = 12;
                    new_date->year--;
                }
                new_date->day = days_in_month(new_date->year, new_date->month);
            } 
            else {
                new_date->day += ndays;
                ndays = 0;
            }
        }
    }

    return new_date;
}

Date* date_add_months(const Date* orig_date, int nmonths) {
    if (orig_date == NULL || !date_is_valid_ymd(orig_date->year, orig_date->month, orig_date->day)) {
        perror("date is null or not valid");
        exit(-1);
    }

    Date* new_date = (Date*)malloc(sizeof(Date));
    if (new_date == NULL) {
        perror("Cannot allocate memory for new date");
        exit(-1);
    }

    *new_date = *orig_date;
    int years_to_add = nmonths / 12;
    nmonths %= 12;

    new_date->year += years_to_add;
    new_date->month += nmonths;

    if (new_date->month > 12) {
        new_date->year++;
        new_date->month -= 12;
    }

    int days_this_month = days_in_month(new_date->year, new_date->month);
    if (new_date->day > days_this_month) {
        new_date->day = days_this_month;
    }

    return new_date;
}

Date* date_add_years(const Date* orig_date, int nyears) {
    if (orig_date == NULL || !date_is_valid_ymd(orig_date->year, orig_date->month, orig_date->day)) {
        perror("date is null or not valid");
        exit(-1);
    }

    Date* new_date = (Date*)malloc(sizeof(Date));
    if (new_date == NULL) {
        perror("Cannot allocate memory for new date");
        exit(-1);
    }

    *new_date = *orig_date;
    new_date->year += nyears;

    if (new_date->month == 2 && new_date->day == 29 && !date_is_valid_ymd(new_date->year, new_date->month, new_date->day)) {
        new_date->day = 28; // Adjust for non-leap year
    }

    return new_date;
}

void date_get_date(const Date* date, int *year, int *month, int *day) {
    if (date == NULL || year == NULL || month == NULL || day == NULL) {
        perror("Invalid argument passed to date_get_date");
        return;
    }

    *year = date->year;
    *month = date->month;
    *day = date->day;
}

int date_day(const Date* date) {
    if (date == NULL || !date_is_valid(date)) {
        // Handle the error, as the date is NULL
        perror("Date is null in date_day or date is not valid");
        return -1; // Return an invalid day
    }
    return date->day;
}

int date_month(const Date* date) {
    if (date == NULL || !date_is_valid(date)) {
        // Handle the error, as the date is NULL
        perror("Date is null in date_month or date is not valid");
        return -1; // Return an invalid month
    }
    return date->month;
}

int date_year(const Date* date) {
    if (date == NULL) {
        // Handle the error, as the date is NULL
        perror("Date is null in date_year or date is not valid");
        return -1; // Return an invalid year
    }
    return date->year;
}

int date_day_of_week(const Date* date) {
    if (date == NULL) {
        perror("Date is null in date_day_of_week");
        return -1; // Indicate an error
    }

    // Zeller's Congruence Algorithm to calculate day of week
    int h, q, m, k, j;
    q = date->day;
    m = (date->month < 3) ? date->month + 12 : date->month;
    k = date->year % 100;
    j = date->year / 100;

    // Adjust months for January and February
    if (m > 12) {
        k--;
    }

    h = (q + (13*(m + 1))/5 + k + k/4 + j/4 + 5*j) % 7;
    int dayOfWeek = ((h + 5) % 7) + 1; // Convert to 1 (Monday) to 7 (Sunday)

    return dayOfWeek;
}

int date_day_of_year(const Date* date) {
    if (date == NULL) {
        perror("Date is null in date_day_of_year");
        return -1; // Indicate an error
    }

    static const int daysBeforeMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    int dayOfYear = daysBeforeMonth[date->month - 1] + date->day;

    // Add one day if it's a leap year and after February
    if (date->month > 2 && ((date->year % 4 == 0 && date->year % 100 != 0) || (date->year % 400 == 0))) {
        dayOfYear++;
    }

    return dayOfYear;
}

int date_days_in_month(const Date* date) {
    if (date == NULL) {
        perror("Date is null in date_days_in_month");
        return -1; // Indicate an error
    }

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = daysInMonth[date->month - 1];

    // Check for leap year in February
    if (date->month == 2 && ((date->year % 4 == 0 && date->year % 100 != 0) || (date->year % 400 == 0))) {
        days++;
    }

    return days;
}