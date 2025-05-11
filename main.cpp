// # by Rafaqat, Haseeb, Rehan
#include "bin/lib/header/VotingSystem.h"
#include "bin/lib/header/timeUtils.h"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <ctime>
#include <regex>
#include <sstream>

using namespace std;

// Color codes for Windows console
enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// Input validation utilities
bool isValidDate(const string& date) {
    regex datePattern(R"(\d{4}-\d{2}-\d{2})");
    if (!regex_match(date, datePattern)) {
        return false;
    }
    
    int year, month, day;
    char dash1, dash2;
    stringstream ss(date);
    ss >> year >> dash1 >> month >> dash2 >> day;
    
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[2] = 29; // Leap year
    }
    
    return day >= 1 && day <= daysInMonth[month];
}

bool isValidCNIC(const string& cnic) {
    // Pakistani CNIC pattern: 12345-1234567-1
    regex cnicPattern(R"(\d{5}-\d{7}-\d{1})");
    return regex_match(cnic, cnicPattern);
}

int calculateAge(const string& dateOfBirth) {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;
    
    int birthYear, birthMonth, birthDay;
    char dash1, dash2;
    stringstream ss(dateOfBirth);
    ss >> birthYear >> dash1 >> birthMonth >> dash2 >> birthDay;
    
    int age = currentYear - birthYear;
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--; // Not yet had birthday this year
    }
      return age;
}

// Use the function from timeUtils.h instead
// string getCurrentDate() {
//    time_t now = time(nullptr);
//    tm* localTime = localtime(&now);
//    
//    stringstream ss;
//    ss << (localTime->tm_year + 1900) << "-" 
//       << setfill('0') << setw(2) << (localTime->tm_mon + 1) << "-" 
//       << setfill('0') << setw(2) << localTime->tm_mday;
//    
//    return ss.str();
// }

// Set console text color
void setColor(int textColor, int bgColor = BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Reset console color to default
void resetColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, WHITE);
}

// Center align text
void printCentered(const string& text, int width = 80) {
    int padding = (width - text.length()) / 2;
    if (padding > 0) {
        cout << string(padding, ' ');
    }
    cout << text << endl;
}

// Display a cool separator
void displaySeparator(char symbol = '=', int width = 80, Color color = CYAN) {
    setColor(color);
    cout << string(width, symbol) << endl;
    resetColor();
}

// Display banner text
void displayBanner(const string& text, Color color = LIGHTGREEN) {
    int width = 80;
    displaySeparator('-', width, BLUE);
    cout << endl;
    setColor(color);
    printCentered(text, width);
    resetColor();
    cout << endl;
    displaySeparator('-', width, BLUE);
}

VotingSystem votingSystem;

// Helper function for getting validated integer input
int getValidatedInt(const string& prompt, int min, int max) {
    int choice;
    bool validInput = false;
    
    do {
        cout << prompt;
        
        // Check if the input is not an integer
        if (!(cin >> choice)) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        
        // Consume newline character
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // Check if the input is in range
        if (choice < min || choice > max) {
            cout << "Please enter a number between " << min << " and " << max << "." << endl;
        } else {
            validInput = true;
        }
    } while (!validInput);
    
    return choice;
}

// Helper function for getting string input
string getStringInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

void displayMainMenu() {    system("cls"); // Clear screen for better visuals
      // Display cool cybersecurity-themed banner - more compact version for better centering
    setColor(LIGHTCYAN);
    cout << endl;
    printCentered(" _______  __   __  _______  _______  ______    _______  ___      _______  _______  _______ ", 100);
    printCentered("|       ||  | |  ||  _    ||       ||    _ |  |       ||   |    |       ||       ||       |", 100);
    printCentered("|       ||  |_|  || |_|   ||    ___||   | ||  |    ___||   |    |    ___||       ||_     _|", 100);
    printCentered("|       ||       ||       ||   |___ |   |_||_ |   |___ |   |    |   |___ |       |  |   |  ", 100);
    printCentered("|      _||       ||  _   | |    ___||    __  ||    ___||   |___ |    ___||      _|  |   |  ", 100);
    printCentered("|     |_ |   _   || |_|   ||   |___ |   |  | ||   |___ |       ||   |___ |     |_   |   |  ", 100);
    printCentered("|_______||__| |__||_______||_______||___|  |_||_______||_______||_______||_______|  |___|  ", 100);
    resetColor();
      cout << endl;    setColor(YELLOW);
    printCentered(":: SECURE VOTING SYSTEM ::", 100);
    printCentered("[ Stops the f*ckin' dictatorship ]", 100);
    resetColor();
    cout << endl;    displaySeparator('=', 90, LIGHTBLUE);
    cout << endl;
    
    setColor(LIGHTGREEN);
    printCentered("MAIN MENU", 100);
    resetColor();
    cout << endl;
      // Menu options with color highlights and aligned numbers
    setColor(CYAN);
    printCentered("  [1] Login as Voter", 100);
    printCentered("  [2] Login as Administrator", 100);
    printCentered("  [3] Register New Account", 100);
    printCentered("  [4] View Running Elections", 100);
    setColor(LIGHTRED);
    printCentered("  [5] Exit System", 100);
    resetColor();
      cout << endl;
    displaySeparator('=', 90, LIGHTBLUE);
    cout << endl;
    
    setColor(YELLOW);
    cout << " > Enter your choice: ";
    resetColor();
}

void voterMenu() {
    while (true) {
        system("cls"); // Clear screen for better visuals
        
        // Voter menu banner using ASCII box drawing to avoid encoding issues
        setColor(LIGHTGREEN);
        printCentered("+--------------------------------------+", 80);
        printCentered("|            VOTER TERMINAL            |", 80);
        printCentered("+--------------------------------------+", 80);
        resetColor();
        cout << endl;
        
        displaySeparator('-', 60, BLUE);
        cout << endl;
        
        // Menu options with color highlights and aligned numbers
        setColor(LIGHTCYAN);
        printCentered("  [1] View Active Elections", 80);
        printCentered("  [2] Cast Your Vote", 80);
        setColor(LIGHTRED);
        printCentered("  [3] Logout", 80);
        resetColor();
        
        cout << endl;
        displaySeparator('-', 60, BLUE);
        cout << endl;
        
        setColor(YELLOW);
        cout << " > Enter your choice: ";
        resetColor();

        // Get validated input
        int choice = getValidatedInt("", 1, 3);

        switch (choice) {            case 1: {
                system("cls");
                setColor(LIGHTGREEN);
                printCentered("+--------------------------------------+", 80);
                printCentered("|          ACTIVE ELECTIONS            |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("RUNNING ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Get list of running elections for menu selection
                const int MAX_ELECTIONS = 20;
                string activeElectionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getRunningElectionIds(activeElectionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No active elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(activeElectionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice (or " + to_string(electionCount + 1) + " to go back): ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID and display full details
                string electionId = activeElectionIds[electionChoice - 1];
                
                cout << endl;
                setColor(YELLOW);
                printCentered("ELECTION DETAILS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                  votingSystem.displayElectionInfo(electionId);
                
                // Add a live countdown timer option
                cout << endl;
                setColor(CYAN);
                cout << " [1] Show live countdown" << endl;
                cout << " [2] Back to elections list" << endl << endl;
                resetColor();
                
                int countdownChoice = getValidatedInt(" Enter choice: ", 1, 2);
                
                if (countdownChoice == 1) {
                    // Display live countdown for 30 seconds or until key press
                    votingSystem.displayLiveCountdown(electionId, 30);
                }
                
                cout << endl;
                setColor(YELLOW);
                cout << " Press Enter to continue...";
                resetColor();
                cin.get();
                break;
            }
            case 2: {                system("cls");
                // First show active elections
                setColor(LIGHTGREEN);                
                printCentered("+--------------------------------------+", 80);
                printCentered("|               CAST VOTE                |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                // Get list of running elections for menu selection
                const int MAX_ELECTIONS = 20;
                string electionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getRunningElectionIds(electionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No active elections are currently running." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                setColor(CYAN);
                printCentered("SELECT AN ELECTION", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(electionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice: ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID
                string electionId = electionIds[electionChoice - 1];
                
                if (votingSystem.hasVoted(electionId)) {
                    setColor(LIGHTRED);
                    cout << " [!] You have already voted in this election." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                  // Show election details again with candidate list
                setColor(LIGHTCYAN);
                printCentered("ELECTION DETAILS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Display election info to show the user
                votingSystem.displayElectionInfo(electionId);
                
                // Get list of candidates for menu selection
                const int MAX_CANDIDATES = 20;
                int candidateIndices[MAX_CANDIDATES];
                int candidateCount = 0;
                votingSystem.getCandidateIndices(electionId, candidateIndices, MAX_CANDIDATES, candidateCount);
                
                if (candidateCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No candidates are available in this election." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                cout << endl;
                setColor(CYAN);
                printCentered("SELECT A CANDIDATE", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Display candidates as a numbered menu
                for (int i = 0; i < candidateCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayCandidateSummary(electionId, candidateIndices[i]);
                }
                
                cout << " [" << (candidateCount + 1) << "] Back to elections" << endl << endl;
                
                // Get user selection
                int candidateChoice = getValidatedInt(" Enter your choice: ", 1, candidateCount + 1);
                
                // Check if user wants to go back
                if (candidateChoice == candidateCount + 1) {
                    // Go back to election selection
                    continue;
                }
                  // Convert the menu choice to candidate index (1-based for the UI)
                string candidateId = to_string(candidateChoice);
                
                if (votingSystem.castVote(electionId, candidateId)) {
                    setColor(LIGHTGREEN);
                    cout << " [✓] Vote cast successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << " [!] Failed to cast vote. Please check the election and candidate IDs." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 3:
                votingSystem.logout();
                setColor(YELLOW);
                cout << " [i] Logged out successfully." << endl;
                resetColor();
                cout << "\n Press Enter to continue...";
                cin.get();
                return;
        }
    }
}

void adminMenu() {
    while (true) {
        system("cls"); // Clear screen for better visuals
        
        // Admin menu banner using ASCII box drawing to avoid encoding issues
        setColor(LIGHTMAGENTA);
        printCentered("+--------------------------------------+", 80);
        printCentered("|        ADMINISTRATOR CONSOLE         |", 80);
        printCentered("+--------------------------------------+", 80);
        resetColor();
        cout << endl;
        
        displaySeparator('-', 60, BLUE);
        cout << endl;
        
        // Menu options with color highlights and aligned numbers
        setColor(LIGHTCYAN);
        printCentered("  [1] Create New Election", 80);
        printCentered("  [2] Start Election", 80);
        printCentered("  [3] End Election", 80);
        printCentered("  [4] Add Candidate", 80);
        printCentered("  [5] View Election Results", 80);
        printCentered("  [6] View All Elections", 80);
        setColor(LIGHTRED);
        printCentered("  [7] Logout", 80);
        resetColor();
        
        cout << endl;
        displaySeparator('-', 60, BLUE);
        cout << endl;
        
        setColor(YELLOW);
        cout << " > Enter your choice: ";
        resetColor();

        // Get validated input
        int choice = getValidatedInt("", 1, 7);

        switch (choice) {
            case 1: {
                system("cls");
                setColor(LIGHTGREEN);                printCentered("+--------------------------------------+", 80);
                printCentered("|           CREATE NEW ELECTION          |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                  string type;
                bool validType = false;
                
                setColor(CYAN);
                cout << " Election Types:" << endl;
                cout << "  - local (e.g., City Mayor, Town Council)" << endl;
                cout << "  - national (e.g., Presidential, Congressional)" << endl;
                cout << "  - regional (e.g., State Governor, Provincial)" << endl << endl;
                
                do {
                    type = getStringInput(" Enter election type (local/national/regional): ");
                    if (type == "local" || type == "national" || type == "regional") {
                        validType = true;
                    } else {
                        setColor(LIGHTRED);
                        cout << " [!] Invalid election type. Please enter 'local', 'national', or 'regional'." << endl;
                        setColor(CYAN);
                    }} while (!validType);
                
                string name = getStringInput(" Enter election name: ");
                
                // Get start date with example and option to use current date
                string start;
                bool validStartDate = false;
                do {
                    string currentDate = getCurrentDate();
                    cout << " Enter start date (YYYY-MM-DD) or press Enter for today (" << currentDate << "): ";
                    getline(cin, start);
                    
                    // If empty, use current date
                    if (start.empty()) {
                        start = currentDate;
                        validStartDate = true;
                    } 
                    else if (!isValidDate(start)) {
                        setColor(LIGHTRED);
                        cout << " [!] Invalid date format. Please use YYYY-MM-DD format." << endl;
                        setColor(CYAN);
                    } else {
                        validStartDate = true;
                    }
                } while (!validStartDate);
                  // Get end date with example
                string end;
                bool validEndDate = false;
                do {
                    end = getStringInput(" Enter end date (YYYY-MM-DD): ");
                    if (!isValidDate(end)) {
                        setColor(LIGHTRED);
                        cout << " [!] Invalid date format. Please use YYYY-MM-DD format." << endl;
                        setColor(CYAN);
                    } else {
                        // Compare dates to ensure end date is after start date
                        int startYear, startMonth, startDay;
                        int endYear, endMonth, endDay;
                        char dash1, dash2;
                        
                        stringstream startSS(start);
                        startSS >> startYear >> dash1 >> startMonth >> dash2 >> startDay;
                        
                        stringstream endSS(end);
                        endSS >> endYear >> dash1 >> endMonth >> dash2 >> endDay;
                        
                        if (endYear < startYear || 
                           (endYear == startYear && endMonth < startMonth) ||
                           (endYear == startYear && endMonth == startMonth && endDay < startDay)) {
                            setColor(LIGHTRED);
                            cout << " [!] End date must be after start date." << endl;
                            setColor(CYAN);
                        } else {
                            validEndDate = true;
                        }
                    }
                } while (!validEndDate);
                  string region;
                int regions = 0;
                
                if (type == "national") {
                    cout << " Examples: Pakistan, United States, Canada, etc." << endl;
                    region = getStringInput(" Enter country: ");
                    
                    bool validRegions = false;
                    string regionsStr;                    do {
                        regionsStr = getStringInput(" Enter number of regions (provinces/states): ");
                        
                        // Parse the string to int safely without using stoi
                        bool isValid = true;
                        regions = 0;
                        
                        for (char c : regionsStr) {
                            if (c >= '0' && c <= '9') {
                                regions = regions * 10 + (c - '0');
                            } else {
                                isValid = false;
                                break;
                            }
                        }
                        
                        if (!isValid) {
                            setColor(LIGHTRED);
                            cout << " [!] Please enter a valid number." << endl;
                            setColor(CYAN);
                        } else if (regions <= 0) {
                            setColor(LIGHTRED);
                            cout << " [!] Number of regions must be greater than 0." << endl;
                            setColor(CYAN);
                        } else {
                            validRegions = true;
                        }
                    } while (!validRegions);
                }
                else if (type == "regional") {
                    cout << " Examples: Punjab, California, Ontario, etc." << endl;
                    region = getStringInput(" Enter region: ");
                }
                else if (type == "local") {
                    cout << " Examples: Lahore, San Francisco, Toronto, etc." << endl;
                    region = getStringInput(" Enter locality: ");
                }
                resetColor();
                
                if (votingSystem.createElection(type, name, start, end, region, regions)) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Election created successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Failed to create election." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 2: {                system("cls");
                setColor(LIGHTGREEN);
                printCentered("+--------------------------------------+", 80);
                printCentered("|             START ELECTION           |", 80);                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("AVAILABLE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Get list of all elections for menu selection
                const int MAX_ELECTIONS = 20;
                string electionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getAllElectionIds(electionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(electionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice: ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID
                string electionId = electionIds[electionChoice - 1];
                resetColor();
                
                if (votingSystem.startElection(electionId)) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Election started successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Failed to start election. Please check if the election exists." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            
            case 3: {
                system("cls");
                setColor(LIGHTGREEN);                printCentered("+--------------------------------------+", 80);
                printCentered("|              END ELECTION              |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("ACTIVE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Get list of active elections for menu selection
                const int MAX_ELECTIONS = 20;
                string activeElectionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getRunningElectionIds(activeElectionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No active elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Display active elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(activeElectionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice: ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID
                string electionId = activeElectionIds[electionChoice - 1];
                resetColor();
                
                if (votingSystem.endElection(electionId)) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Election ended successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Failed to end election. Please check if the election exists and is active." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            
            case 4: {
                system("cls");
                setColor(LIGHTGREEN);                printCentered("+--------------------------------------+", 80);
                printCentered("|             ADD CANDIDATE              |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("AVAILABLE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Get list of all elections for menu selection
                const int MAX_ELECTIONS = 20;
                string electionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getAllElectionIds(electionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(electionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice: ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID
                string electionId = electionIds[electionChoice - 1];
                  // Get candidate information
                setColor(CYAN);
                string name = getStringInput(" Enter Candidate Name: ");
                string party = getStringInput(" Enter Party Affiliation: ");
                string symbol = getStringInput(" Enter Party Symbol (e.g., Tiger, Arrow, Lion): ");
                resetColor();
                
                if (votingSystem.addCandidate(electionId, name, party, symbol)) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Candidate added successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Failed to add candidate. Please check if the election exists." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 5: {                system("cls");
                setColor(LIGHTGREEN);
                printCentered("+--------------------------------------+", 80);
                printCentered("|          ELECTION RESULTS            |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("AVAILABLE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Get list of all elections for menu selection
                const int MAX_ELECTIONS = 20;
                string electionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getAllElectionIds(electionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(electionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice: ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID
                string electionId = electionIds[electionChoice - 1];
                resetColor();
                
                cout << endl;
                setColor(YELLOW);
                printCentered("RESULTS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                votingSystem.viewResults(electionId);
                
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            
            case 6: {                
                system("cls");
                setColor(LIGHTGREEN);
                printCentered("+--------------------------------------+", 80);
                printCentered("|            ALL ELECTIONS             |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("AVAILABLE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                // Get list of all elections for menu selection
                const int MAX_ELECTIONS = 20;
                string electionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getAllElectionIds(electionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(electionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice (or " + to_string(electionCount + 1) + " to go back): ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID and display full details
                string electionId = electionIds[electionChoice - 1];
                
                cout << endl;
                setColor(YELLOW);
                printCentered("ELECTION DETAILS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                votingSystem.displayElectionInfo(electionId);
                
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 7:
                votingSystem.logout();
                setColor(YELLOW);
                cout << "\n [i] Logged out successfully." << endl;
                resetColor();
                cout << "\n Press Enter to continue...";
                cin.get();
                return;
        }
    }
}

void registrationMenu() {
    while (true) {
        system("cls"); // Clear screen for better visuals
        
        // Registration menu banner
        setColor(LIGHTGREEN);        printCentered("+--------------------------------------+", 80);
        printCentered("|              REGISTRATION              |", 80);
        printCentered("+--------------------------------------+", 80);
        resetColor();
        cout << endl;
        
        displaySeparator('-', 60, BLUE);
        cout << endl;
          // Menu options with color highlights and aligned numbers
        setColor(LIGHTCYAN);
        printCentered("  [1] Register as Voter", 80);
        printCentered("  [2] Register as Administrator", 80);
        setColor(LIGHTRED);
        printCentered("  [3] Back to Main Menu", 80);
        resetColor();
        
        cout << endl;
        displaySeparator('-', 60, BLUE);
        cout << endl;
        
        setColor(YELLOW);
        cout << " > Enter your choice: ";
        resetColor();

        int choice = getValidatedInt("", 1, 3);
        
        string username, password, name;
        bool success = false;

        switch (choice) {
            case 1: {                system("cls");
                setColor(LIGHTGREEN);                
                printCentered("+--------------------------------------+", 80);
                printCentered("|          VOTER REGISTRATION           |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                username = getStringInput(" Enter username: ");
                password = getStringInput(" Enter password: ");
                name = getStringInput(" Enter full name: ");
                
                // Get and validate CNIC
                string cnic;
                do {
                    cnic = getStringInput(" Enter CNIC (format: 12345-1234567-1): ");
                    if (!isValidCNIC(cnic)) {
                        setColor(LIGHTRED);
                        cout << " [!] Invalid CNIC format. Please use format: 12345-1234567-1" << endl;
                        setColor(CYAN);
                    }
                } while (!isValidCNIC(cnic));
                
                // Get and validate Date of Birth with 18+ age check
                string dob;
                bool validDob = false;
                do {
                    dob = getStringInput(" Enter date of birth (YYYY-MM-DD): ");                    if (!isValidDate(dob)) {
                        setColor(LIGHTRED);
                        cout << " [!] Invalid date format. Please use YYYY-MM-DD format." << endl;
                        setColor(CYAN);
                    } else if (calculateAge(dob) < 18) {
                        setColor(LIGHTRED);
                        cout << " [!] You must be at least 18 years old to register as a voter." << endl;
                        cout << " Press any key to return to the home screen...";
                        resetColor();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        char c;
                        cin.get(c);
                        return; // Return to main menu
                    } else {
                        validDob = true;
                    }
                } while (!validDob);
                
                // Get city information
                string city = getStringInput(" Enter your city: ");
                resetColor();

                success = votingSystem.registerVoter(username, password, name, cnic, dob, city);
                if (success) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Voter registered successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Failed to register voter." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 2: {
                system("cls");
                setColor(LIGHTMAGENTA);                printCentered("+--------------------------------------+", 80);
                printCentered("|      ADMINISTRATOR REGISTRATION      |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                username = getStringInput(" Enter username: ");
                password = getStringInput(" Enter password: ");
                name = getStringInput(" Enter full name: ");
                resetColor();

                success = votingSystem.registerAdmin(username, password, name);
                if (success) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Administrator registered successfully!" << endl;
                    resetColor();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Failed to register administrator." << endl;
                    resetColor();
                }
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 3:
                return;
        }
    }
}

// Use the global function from timeUtils.h without redefining
// RemainingTime calculateRemainingTime(const string& endDate) {
//    // Call the global function from timeUtils.h
//    return ::calculateRemainingTime(endDate);
// }

int main() {
    // Set console title and size
    SetConsoleTitle(TEXT("CyberElect - Secure Voting System"));
    
    // Create necessary directories for unified storage
    system("mkdir data 2>nul");
    system("mkdir data\\elections 2>nul"); // For all election data
    system("mkdir data\\Users 2>nul");     // For all user credentials

    while (true) {
        displayMainMenu();
        
        int choice = getValidatedInt("", 1, 5);
        
        switch (choice) {
            case 1: {
                system("cls");
                setColor(LIGHTGREEN);                printCentered("+--------------------------------------+", 80);
                printCentered("|             VOTER LOGIN              |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                string username = getStringInput(" Enter username: ");
                string password = getStringInput(" Enter password: ");
                resetColor();
                
                if (votingSystem.loginUser(username, password, "voter")) {
                    setColor(LIGHTGREEN);
                    cout << "\n [ok] Login successful!" << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    voterMenu();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Invalid credentials! Access denied." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                }
                break;
            }
            case 2: {
                system("cls");
                setColor(LIGHTMAGENTA);                printCentered("+--------------------------------------+", 80);
                printCentered("|        ADMINISTRATOR LOGIN           |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                string username = getStringInput(" Enter username: ");
                string password = getStringInput(" Enter password: ");
                resetColor();
                
                if (votingSystem.loginUser(username, password, "admin")) {
                    setColor(LIGHTGREEN);
                    cout << "\n [✓] Login successful!" << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    adminMenu();
                } else {
                    setColor(LIGHTRED);
                    cout << "\n [!] Invalid credentials! Access denied." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                }
                break;
            }
            case 3:
                registrationMenu();
                break;            case 4: {
                system("cls");
                setColor(LIGHTCYAN);                
                printCentered("+--------------------------------------+", 80);
                printCentered("|         RUNNING ELECTIONS            |", 80);
                printCentered("+--------------------------------------+", 80);                resetColor();
                cout << endl;
                
                // Get list of running elections for menu selection
                const int MAX_ELECTIONS = 20;
                std::string activeElectionIds[MAX_ELECTIONS];
                int electionCount = 0;
                votingSystem.getRunningElectionIds(activeElectionIds, MAX_ELECTIONS, electionCount);
                
                if (electionCount == 0) {
                    setColor(LIGHTRED);
                    cout << " [!] No active elections available." << endl;
                    resetColor();
                    cout << "\n Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                // Show current date
                string currentDate = getCurrentDate();
                setColor(YELLOW);
                cout << "\nCurrent Date: " << currentDate << endl;
                
                setColor(CYAN);
                printCentered("ACTIVE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                  // Display elections as a numbered menu
                for (int i = 0; i < electionCount; i++) {
                    cout << " [" << (i + 1) << "] ";
                    votingSystem.displayElectionSummary(activeElectionIds[i]);
                }
                
                cout << " [" << (electionCount + 1) << "] Back to menu" << endl << endl;
                
                // Get user selection
                int electionChoice = getValidatedInt(" Enter your choice (or " + to_string(electionCount + 1) + " to go back): ", 1, electionCount + 1);
                
                // Check if user wants to go back
                if (electionChoice == electionCount + 1) {
                    break;
                }
                
                // Get the selected election ID and display full details
                string electionId = activeElectionIds[electionChoice - 1];
                
                cout << endl;
                setColor(YELLOW);
                printCentered("ELECTION DETAILS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                votingSystem.displayElectionInfo(electionId);
                
                cout << "\n Press Enter to continue...";
                cin.get();
                break;
            }
            case 5: {
                system("cls");
                setColor(YELLOW);
                cout << endl << endl;
                printCentered("Thank you for using CyberElect Voting System!", 80);
                printCentered("Exiting securely...", 80);
                cout << endl << endl;
                
                // Animation for exit
                for (int i = 0; i < 20; i++) {
                    setColor(i % 15 + 1);
                    cout << string(i * 4, ' ') << "■";
                    Sleep(50);
                }
                
                resetColor();
                cout << endl << endl;
                return 0;
            }
        }
    }
    
    return 0;
}
