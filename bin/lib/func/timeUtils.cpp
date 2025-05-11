#include "../header/timeUtils.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

// Function to get current time as a formatted string (HH:MM:SS)
std::string getCurrentTime() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":" 
       << std::setfill('0') << std::setw(2) << localTime->tm_min << ":" 
       << std::setfill('0') << std::setw(2) << localTime->tm_sec;
    
    return ss.str();
}

// Function to get current date as YYYY-MM-DD
std::string getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    std::stringstream ss;
    ss << (localTime->tm_year + 1900) << "-" 
       << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-" 
       << std::setfill('0') << std::setw(2) << localTime->tm_mday;
    
    return ss.str();
}

// Calculate remaining time until an election ends
RemainingTime calculateRemainingTime(const std::string& endDate) {
    // Parse the end date (YYYY-MM-DD format)
    int endYear, endMonth, endDay;
    char dash1, dash2;
    std::stringstream ss(endDate);
    ss >> endYear >> dash1 >> endMonth >> dash2 >> endDay;
    
    // Get current time
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;
    int currentHour = localTime->tm_hour;
    int currentMinute = localTime->tm_min;
    int currentSecond = localTime->tm_sec;
    
    // Calculate time difference in hours, minutes, and seconds
    // Assuming end time is at end of day (23:59:59)
    int totalDays = 0;
    
    if (endYear > currentYear || 
        (endYear == currentYear && endMonth > currentMonth) ||
        (endYear == currentYear && endMonth == currentMonth && endDay > currentDay)) {
        
        // Remaining days in current month
        int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((currentYear % 4 == 0 && currentYear % 100 != 0) || (currentYear % 400 == 0)) {
            daysInMonth[2] = 29; // Leap year
        }
        
        if (endYear == currentYear && endMonth == currentMonth) {
            totalDays = endDay - currentDay;
        } else {
            // Days left in current month
            totalDays += (daysInMonth[currentMonth] - currentDay);
            
            // Days in full months between
            if (endYear == currentYear) {
                for (int m = currentMonth + 1; m < endMonth; m++) {
                    totalDays += daysInMonth[m];
                }
            } else {
                // Remaining months in current year
                for (int m = currentMonth + 1; m <= 12; m++) {
                    totalDays += daysInMonth[m];
                }
                
                // Full years in between
                for (int y = currentYear + 1; y < endYear; y++) {
                    totalDays += ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) ? 366 : 365;
                }
                
                // Months in the final year
                if ((endYear % 4 == 0 && endYear % 100 != 0) || (endYear % 400 == 0)) {
                    daysInMonth[2] = 29; // Leap year for end year
                } else {
                    daysInMonth[2] = 28;
                }
                
                for (int m = 1; m < endMonth; m++) {
                    totalDays += daysInMonth[m];
                }
            }
            
            // Add days in final month
            totalDays += endDay;
        }
        
        // Convert to hours, minutes, and seconds
        // End time is 23:59:59, so calculate seconds left
        int totalHours = (totalDays * 24) - currentHour - 1; // -1 because end at 23:59:59
        int totalMinutes = 59 - currentMinute;
        int totalSeconds = 59 - currentSecond;

        // Handle carry for seconds/minutes
        if (totalSeconds < 0) {
            totalSeconds += 60;
            totalMinutes--;
        }
        if (totalMinutes < 0) {
            totalMinutes += 60;
            totalHours--;
        }
        
        // Return hours, minutes, and seconds remaining
        RemainingTime result;
        result.hours = totalHours;
        result.minutes = totalMinutes;
        result.seconds = totalSeconds;
        return result;
    }
    
    // If end date is today or in the past, return 0
    RemainingTime result = {0, 0, 0};
    return result;
}
