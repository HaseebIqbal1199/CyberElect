// # by Rafaqat, Haseeb, Rehan
#include "bin/lib/header/VotingSystem.h"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include <Windows.h>

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

void displayMainMenu() {
    system("cls"); // Clear screen for better visuals
      // Display cool cybersecurity-themed banner
    setColor(LIGHTCYAN);
    cout << endl;
    printCentered(" .d8888b. Y88b   d88P 8888888b.  8888888888 8888888b.  8888888888 888      8888888888  .d8888b. 88888888888 ", 100);
    printCentered("d88P  Y88b Y88b d88P  888   Y88b 888        888   Y88b 888        888      888        d88P  Y88b    888     ", 100);
    printCentered("888    888  Y88o88P   888    888 888        888    888 888        888      888        888    888    888     ", 100);
    printCentered("888          Y888P    888   d88P 8888888    888   d88P 8888888    888      8888888    888           888     ", 100);
    printCentered("888           888     8888888P\"  888        8888888P\"  888        888      888        888           888     ", 100);
    printCentered("888    888    888     888 T88b   888        888 T88b   888        888      888        888    888    888     ", 100);
    printCentered("Y88b  d88P    888     888  T88b  888        888  T88b  888        888      888        Y88b  d88P    888     ", 100);
    printCentered(" \"Y8888P\"     888     888   T88b 8888888888 888   T88b 8888888888 88888888 8888888888  \"Y8888P\"     888     ", 100);
    resetColor();
    
    cout << endl;
    setColor(YELLOW);
    printCentered(":: SECURE VOTING SYSTEM ::", 100);
    printCentered("[ Stops the f*ckin' dictatorship ]", 100);
    resetColor();
    cout << endl;

    displaySeparator('=', 80, LIGHTBLUE);
    cout << endl;
    
    setColor(LIGHTGREEN);
    printCentered("MAIN MENU", 80);
    resetColor();
    cout << endl;
    
    // Menu options with color highlights and aligned numbers
    setColor(CYAN);
    printCentered("  [1] Login as Voter", 80);
    printCentered("  [2] Login as Administrator", 80);
    printCentered("  [3] Register New Account", 80);
    printCentered("  [4] View Running Elections", 80);
    setColor(LIGHTRED);
    printCentered("  [5] Exit System", 80);
    resetColor();
    
    cout << endl;
    displaySeparator('=', 80, LIGHTBLUE);
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

        switch (choice) {
            case 1: {
                system("cls");
                setColor(LIGHTGREEN);
                printCentered("+--------------------------------------+", 80);
                printCentered("|          ACTIVE ELECTIONS            |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                votingSystem.viewElections();
                
                cout << endl;
                setColor(YELLOW);
                cout << " Press Enter to continue...";
                resetColor();
                cin.get();
                break;
            }
            case 2: {
                system("cls");
                // First show active elections
                setColor(LIGHTGREEN);                printCentered("+--------------------------------------+", 80);
                printCentered("|               CAST VOTE                |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                votingSystem.displayRunningElections();
                
                cout << endl;
                setColor(CYAN);
                cout << "\n Enter Election ID (or 'back' to return to menu): ";
                resetColor();
                string electionId;
                getline(cin, electionId);
                
                if (electionId == "back") {
                    break;
                }
                
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
                
                votingSystem.displayElectionInfo(electionId);
                
                cout << endl;
                setColor(CYAN);
                cout << " Enter Candidate ID (or 'back' to return to menu): ";
                resetColor();
                string candidateId;
                getline(cin, candidateId);
                
                if (candidateId == "back") {
                    break;
                }
                
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
                do {
                    type = getStringInput(" Enter election type (local/national/regional): ");
                    if (type == "local" || type == "national" || type == "regional") {
                        validType = true;
                    } else {
                        setColor(LIGHTRED);
                        cout << " [!] Invalid election type. Please enter 'local', 'national', or 'regional'." << endl;
                        setColor(CYAN);
                    }
                } while (!validType);
                
                string name = getStringInput(" Enter election name: ");
                string start = getStringInput(" Enter start date (YYYY-MM-DD): ");
                string end = getStringInput(" Enter end date (YYYY-MM-DD): ");
                string region;
                int regions = 0;
                
                if (type == "national") {
                    region = getStringInput(" Enter country: ");
                    
                    bool validRegions = false;
                    string regionsStr;                    do {
                        regionsStr = getStringInput(" Enter number of regions: ");
                        
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
                    region = getStringInput(" Enter region: ");
                }
                else if (type == "local") {
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
                printCentered("|             START ELECTION           |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                printCentered("AVAILABLE ELECTIONS", 80);
                displaySeparator('-', 40, BLUE);
                resetColor();
                
                votingSystem.viewAllElections();
                
                cout << endl;
                setColor(CYAN);
                string electionId = getStringInput(" Enter Election ID to start: ");
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
                
                // Show only active elections
                votingSystem.displayRunningElections();
                
                cout << endl;
                setColor(CYAN);
                string electionId = getStringInput(" Enter Election ID to end: ");
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
                
                votingSystem.viewAllElections();
                
                cout << endl;
                setColor(CYAN);
                string electionId = getStringInput(" Enter Election ID: ");
                string name = getStringInput(" Enter Candidate Name: ");
                string party = getStringInput(" Enter Party Affiliation: ");
                resetColor();
                
                if (votingSystem.addCandidate(electionId, name, party)) {
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
                
                votingSystem.viewAllElections();
                
                cout << endl;
                setColor(CYAN);
                string electionId = getStringInput(" Enter Election ID: ");
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
            case 6: {                system("cls");
                setColor(LIGHTGREEN);
                printCentered("+--------------------------------------+", 80);
                printCentered("|            ALL ELECTIONS             |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                votingSystem.viewAllElections();
                
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
            case 1: {
                system("cls");
                setColor(LIGHTGREEN);                printCentered("+--------------------------------------+", 80);
                printCentered("|          VOTER REGISTRATION           |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                setColor(CYAN);
                username = getStringInput(" Enter username: ");
                password = getStringInput(" Enter password: ");
                name = getStringInput(" Enter full name: ");
                resetColor();

                success = votingSystem.registerVoter(username, password, name);
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
                    cout << "\n [✓] Login successful!" << endl;
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
                break;
            case 4: {
                system("cls");
                setColor(LIGHTCYAN);                printCentered("+--------------------------------------+", 80);
                printCentered("|         RUNNING ELECTIONS            |", 80);
                printCentered("+--------------------------------------+", 80);
                resetColor();
                cout << endl;
                
                votingSystem.displayRunningElections();
                
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
