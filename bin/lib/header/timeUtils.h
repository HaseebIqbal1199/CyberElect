#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <string>

// Structure to hold time components
struct RemainingTime {
    int hours;
    int minutes;
    int seconds;
};

// Function to get current time as a formatted string (HH:MM:SS)
std::string getCurrentTime();

// Function to get current date as YYYY-MM-DD
std::string getCurrentDate();

// Calculate remaining time until an election ends
RemainingTime calculateRemainingTime(const std::string& endDate);

#endif // TIME_UTILS_H
