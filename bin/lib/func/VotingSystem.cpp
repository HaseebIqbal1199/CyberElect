// # by Rafaqat, Haseeb, Rehan
#include "../header/VotingSystem.h"
#include "../header/timeUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <ctime>
#include <iomanip>
#include <conio.h> // For _kbhit and _getch

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
static void setColor(Color textColor, int bgColor = BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Reset console text color
static void resetColor() {
    setColor(WHITE);
}

// Helper function to update remaining time display
static void updateRemainingTimeDisplay(const std::string& electionId, const std::string& endDate) {
    RemainingTime time = calculateRemainingTime(endDate);
    
    std::cout << "\rCurrent Time: " << getCurrentTime() 
              << " | Remaining: " << std::setfill('0') << std::setw(2) << time.hours << ":" 
              << std::setfill('0') << std::setw(2) << time.minutes << ":" 
              << std::setfill('0') << std::setw(2) << time.seconds << " " << std::flush;
              
    // Sleep for 1 second
    Sleep(1000);
}

VotingSystem::VotingSystem() : currentUser(nullptr) {}

VotingSystem::~VotingSystem() {
    if (currentUser != nullptr) {
        delete currentUser;
    }
}

bool VotingSystem::registerVoter(string username, string password, string name, 
                                string cnic, string dob, string city) {
    stringstream ss;
    ss << "V" << time(nullptr);
    string id = ss.str();
    
    Voter* voter = new Voter(username, password, name, id, cnic, dob, city);
    voter->saveToFile();
    delete voter;
    return true;
}

bool VotingSystem::registerAdmin(string username, string password, string name) {
    stringstream ss;
    ss << "A" << time(nullptr);
    string id = ss.str();
    
    Administrator* admin = new Administrator(username, password, name, id);
    admin->saveToFile();
    delete admin;
    return true;
}

bool VotingSystem::loginUser(string username, string password, string role) {
    if (currentUser != nullptr) {
        delete currentUser;
        currentUser = nullptr;
    }

    // Look for the user in the unified credentials file
    ifstream credsFile("data/Users/Creds.txt");
    if (credsFile.is_open()) {
        string line;
        while (getline(credsFile, line)) {
            stringstream ss(line);
            string id, fileUsername, filePassword, name, fileRole;
            
            getline(ss, id, ',');
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
            getline(ss, name, ',');
            getline(ss, fileRole, ',');
            
            if (fileUsername == username && filePassword == password && fileRole == role) {
                // User found with matching credentials
                if (role == "voter") {
                    Voter* voter = new Voter();
                    voter->loadFromFile(id);
                    currentUser = voter;
                    credsFile.close();
                    return true;
                } else if (role == "admin") {
                    Administrator* admin = new Administrator();
                    admin->loadFromFile(id);
                    currentUser = admin;
                    credsFile.close();
                    return true;
                }
            }
        }
        credsFile.close();
    }
    
    return false;
}

void VotingSystem::logout() {
    if (currentUser != nullptr) {
        delete currentUser;
        currentUser = nullptr;
        cout << "Logged out successfully." << endl;
    }
}

bool VotingSystem::createElection(string type, string name, string start, string end, string region, int regions) {
    // Use the electionManager to create the election
    return electionManager.createElection(type, name, start, end, region, regions);
}

void VotingSystem::displayElectionInfo(const string& electionId) {
    // Check if the election exists in memory
    Election* election = electionManager.getElection(electionId);
    if (election != nullptr) {
        // Show current system time
        string currentDate = getCurrentDate();
        cout << "\nCurrent Date: " << currentDate << endl << endl;
        
        cout << "Election Details:" << endl;
        cout << "---------------" << endl;
        cout << "ID: " << election->getElectionId() << endl;
        cout << "Name: " << election->getElectionName() << endl;
        cout << "Type: " << election->getType() << endl;
        
        // Display type-specific details
        if (election->getType() == "local") {
            LocalElection* localElection = dynamic_cast<LocalElection*>(election);
            if (localElection) {
                cout << "Locality: " << localElection->getLocality() << endl;
            }
        } else if (election->getType() == "regional") {
            RegionalElection* regionalElection = dynamic_cast<RegionalElection*>(election);
            if (regionalElection) {
                cout << "Region: " << regionalElection->getRegion() << endl;
            }
        } else if (election->getType() == "national") {
            NationalElection* nationalElection = dynamic_cast<NationalElection*>(election);
            if (nationalElection) {
                cout << "Country: " << nationalElection->getCountry() << endl;
                cout << "Regions: " << nationalElection->getRegions() << endl;            }
        }
        
        cout << "Start Date: " << election->getStartDate() << endl;
        cout << "End Date: " << election->getEndDate() << endl;
        cout << "Status: " << (election->getIsActive() ? "Active" : "Inactive") << endl;
        
        if (election->getCandidateCount() > 0) {
            cout << "\nCandidates:" << endl;
            cout << "----------" << endl;
            
            for (int i = 0; i < election->getCandidateCount(); i++) {
                Candidate* candidate = election->getCandidate(i);
                if (candidate != nullptr) {
                    cout << "ID: " << (i + 1) << endl;
                    cout << "Name: " << candidate->getName() << endl;
                    cout << "Party: " << candidate->getPartyAffiliation() << endl;
                    cout << string(20, '-') << endl;
                }
            }
        }
        return;
    }
    
    // Fall back to reading from the file directly if not in memory
    string filename = "data/elections/entries.txt";
    ifstream file(filename);
    
    if (file.is_open()) {
        string line;
        bool foundElection = false;
        
        while (getline(file, line)) {
            string type = line;
            string id, name, start, end, region, status;
            int regions = 0;
            
            getline(file, id);
            
            if (id == electionId) {                foundElection = true;
                getline(file, name);
                getline(file, start);
                getline(file, end);
                getline(file, status);
                
                // Show current system time
                string currentDate = getCurrentDate();
                cout << "\nCurrent Date: " << currentDate << endl << endl;
                
                cout << "Election Details:" << endl;
                cout << "---------------" << endl;
                cout << "ID: " << id << endl;
                cout << "Name: " << name << endl;
                cout << "Type: " << type << endl;
                
                if (type == "local" || type == "regional") {
                    cout << "Region: " << region << endl;
                } else if (type == "national") {
                    cout << "Country: " << region << endl;
                    cout << "Regions: " << regions << endl;
                }
                
                cout << "Start Date: " << start << endl;
                cout << "End Date: " << end << endl;
                cout << "Status: " << status << endl;
                  // Read number of candidates
                string candidateCountStr;
                getline(file, candidateCountStr);
                
                // Parse candidate count safely without using stoi
                int candidateCount = 0;
                for (char c : candidateCountStr) {
                    if (c >= '0' && c <= '9') {
                        candidateCount = candidateCount * 10 + (c - '0');
                    } else {
                        candidateCount = 0;
                        break;
                    }
                }
                
                if (candidateCount > 0) {
                    cout << "\nCandidates:" << endl;
                    cout << "----------" << endl;
                    
                    for (int i = 0; i < candidateCount; i++) {
                        string candidateId, candidateName, partyAffiliation, votes;
                        getline(file, candidateId);
                        getline(file, candidateName);
                        getline(file, partyAffiliation);
                        getline(file, votes);
                        
                        cout << "ID: " << (i + 1) << endl;
                        cout << "Name: " << candidateName << endl;
                        cout << "Party: " << partyAffiliation << endl;
                        cout << string(20, '-') << endl;
                    }
                }
                break;
            } else {
                // Skip to the next election
                getline(file, name);
                getline(file, start);
                getline(file, end);
                getline(file, status);
                  // Skip candidate data
                string candidateCountStr;
                getline(file, candidateCountStr);
                
                // Parse candidate count safely without using stoi
                int candidateCount = 0;
                for (char c : candidateCountStr) {
                    if (c >= '0' && c <= '9') {
                        candidateCount = candidateCount * 10 + (c - '0');
                    } else {
                        candidateCount = 0;
                        break;
                    }
                }
                
                for (int i = 0; i < candidateCount * 4; i++) {
                    getline(file, line);
                }
            }
        }
        
        file.close();
        
        if (!foundElection) {
            cout << "Election not found!" << endl;
        }
    }
}

bool VotingSystem::addCandidate(string electionId, string name, string partyAffiliation, string partySymbol) {
    return electionManager.addCandidateToElection(electionId, name, partyAffiliation, partySymbol);
}

bool VotingSystem::startElection(string electionId) {
    return electionManager.setElectionStatus(electionId, true);
}

bool VotingSystem::endElection(string electionId) {
    return electionManager.setElectionStatus(electionId, false);
}

void VotingSystem::viewResults(string electionId) {
    Election* election = electionManager.getElection(electionId);
    if (election != nullptr) {
        election->calculateResults();
        election->displayResults();
    } else {
        cout << "Election not found or not ended yet." << endl;
    }
}

void VotingSystem::viewAllElections() {
    electionManager.displayAllElections();
}

void VotingSystem::viewElections() {
    cout << "\nActive Elections:" << endl;
    cout << "----------------" << endl;
    
    // Use the electionManager to get active elections
    electionManager.displayActiveElections();
}

void VotingSystem::displayRunningElections() {    
    // Show current system time
    std::string currentDate = getCurrentDate();
    std::cout << "\nCurrent Date: " << currentDate << std::endl;
    
    std::cout << "\nRunning Elections Status:" << std::endl;
    std::cout << "------------------------" << std::endl;
    
    // Get all active elections from the ElectionManager
    // Only check up to 20 possible elections (matching ElectionManager::MAX_ELECTIONS)
    const int MAX_POSSIBLE_ELECTIONS = 20;
    bool foundRunningElections = false;
    
    for (int i = 0; i < MAX_POSSIBLE_ELECTIONS; i++) {
        Election* election = electionManager.getElection("E" + std::to_string(i+1));
        if (election && election->getIsActive()) {
            foundRunningElections = true;
            std::cout << "\nElection ID: " << election->getElectionId() << std::endl;
            std::cout << "Election Name: " << election->getElectionName() << std::endl;
            std::cout << "Type: " << election->getType() << std::endl;
            
            // Display type-specific details
            if (election->getType() == "local") {
                LocalElection* localElection = dynamic_cast<LocalElection*>(election);
                if (localElection) {
                    std::cout << "Locality: " << localElection->getLocality() << std::endl;
                }
            } else if (election->getType() == "regional") {
                RegionalElection* regionalElection = dynamic_cast<RegionalElection*>(election);
                if (regionalElection) {
                    std::cout << "Region: " << regionalElection->getRegion() << std::endl;
                }
            } else if (election->getType() == "national") {
                NationalElection* nationalElection = dynamic_cast<NationalElection*>(election);
                if (nationalElection) {
                    std::cout << "Country: " << nationalElection->getCountry() << std::endl;
                    std::cout << "Regions: " << nationalElection->getRegions() << std::endl;
                }            
            }
              std::cout << "Start Date: " << election->getStartDate() << std::endl;
            std::cout << "End Date: " << election->getEndDate() << std::endl;
            std::cout << "Status: Active" << std::endl;
            
            // Calculate and display remaining time with hours, minutes, and seconds
            RemainingTime time = calculateRemainingTime(election->getEndDate());
            std::cout << "Remaining Time: " << time.hours << " hours, " << time.minutes << " minutes, " << time.seconds << " seconds" << std::endl;
            
            // Display candidates
            std::cout << "\nCandidates:" << std::endl;
            std::cout << "----------" << std::endl;
            
            for (int j = 0; j < election->getCandidateCount(); j++) {
                Candidate* candidate = election->getCandidate(j);
                if (candidate != nullptr) {
                    std::cout << "ID: " << (j + 1) << std::endl;
                    std::cout << "Name: " << candidate->getName() << std::endl;
                    std::cout << "Party: " << candidate->getPartyAffiliation() << std::endl;
                    
                    // Show party symbol in a random color if it exists
                    if (!candidate->getPartySymbol().empty()) {
                        // Choose a random color based on index (3-15 to avoid black and dark colors)
                        int colorCode = 3 + (j % 13);
                        setColor(static_cast<Color>(colorCode));
                        std::cout << "Symbol: " << candidate->getPartySymbol() << std::endl;
                        resetColor();
                    }
                    
                    std::cout << std::string(20, '-') << std::endl;
                }
            }
            std::cout << std::string(30, '-') << std::endl;
        }
    }
      if (!foundRunningElections) {
        std::cout << "\nNo running elections found." << std::endl;
    }
}

bool VotingSystem::castVote(string electionId, string candidateId) {
    // First check if voter has already voted
    if (hasVoted(electionId)) {
        cout << "You have already voted in this election!" << endl;
        return false;
    }
    
    // Get the election and candidate
    Election* election = electionManager.getElection(electionId);
    if (election == nullptr || !election->getIsActive()) {
        cout << "Election not found or not active!" << endl;
        return false;
    }
      // Just use the provided candidateId directly as our index
    // The UI uses 1-based indexing, so subtract 1 to get 0-based index for internal use
    int candIndex;
    try {
        candIndex = stoi(candidateId) - 1; // Convert to 0-based index
    } catch (const std::exception& e) {
        cout << "Invalid candidate ID format!" << endl;
        return false;
    }
    
    if (candIndex < 0 || candIndex >= election->getCandidateCount()) {
        cout << "Invalid candidate ID!" << endl;
        return false;
    }
    
    Candidate* candidate = election->getCandidate(candIndex);
    if (candidate == nullptr) {
        cout << "Candidate not found!" << endl;
        return false;
    }
      // Cast the vote
    candidate->incrementVotes();
    
    // Record that this voter has voted
    Voter* voter = dynamic_cast<Voter*>(currentUser);
    if (voter != nullptr) {
        voter->addVotedElection(electionId);
        voter->saveToFile();
    }
    
    // Update the election file with the new vote count using the proper method
    electionManager.updateElection(election);
    
    cout << "Vote cast successfully!" << endl;
    return true;
}

bool VotingSystem::hasVoted(string electionId) {
    if (currentUser != nullptr) {
        Voter* voter = dynamic_cast<Voter*>(currentUser);
        if (voter != nullptr) {
            return voter->hasVotedInElection(electionId);
        }
    }    return false;
}

// Get the list of running elections using arrays
void VotingSystem::getRunningElectionIds(std::string* electionIds, int maxCount, int& count) {
    count = 0;
    
    // Use the electionManager to get running elections
    electionManager.getRunningElectionIds(electionIds, maxCount, count);
}

// Get the list of all elections using arrays
void VotingSystem::getAllElectionIds(std::string* electionIds, int maxCount, int& count) {
    count = 0;
    
    // Use the electionManager to get all elections
    electionManager.getAllElectionIds(electionIds, maxCount, count);
}

// Display detailed information about an election
void VotingSystem::displayElectionSummary(const std::string& electionId) {
    Election* election = electionManager.getElectionById(electionId);
    if (election != nullptr) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Election ID: " << electionId << std::endl;
        std::cout << "Name: " << election->getElectionName() << std::endl;
        std::cout << "Type: " << election->getType() << std::endl;
        std::cout << "Start Date: " << election->getStartDate() << std::endl;
        std::cout << "End Date: " << election->getEndDate() << std::endl;
        std::cout << "Status: " << (election->getIsActive() ? "Running" : "Not Running") << std::endl;
        
        if (election->getType() == "local") {
            LocalElection* local = dynamic_cast<LocalElection*>(election);
            if (local) {
                std::cout << "Region: " << local->getLocality() << std::endl;
            }
        } else if (election->getType() == "regional") {
            RegionalElection* regional = dynamic_cast<RegionalElection*>(election);
            if (regional) {
                std::cout << "Region: " << regional->getRegion() << std::endl;
            }
        }
        
        std::cout << "Candidates:" << std::endl;
        for (int i = 0; i < election->getCandidateCount(); i++) {
            Candidate* candidate = election->getCandidate(i);
            if (candidate) {
                std::cout << "  " << i+1 << ". " << candidate->getName() << " (" << candidate->getPartyAffiliation() << ")";
                
                // Show party symbol in a random color if it exists
                if (!candidate->getPartySymbol().empty()) {
                    // Choose a random color (3-15 to avoid black and dark colors)
                    int colorCode = 3 + (i % 13);
                    setColor(static_cast<Color>(colorCode));
                    std::cout << " Symbol: " << candidate->getPartySymbol();
                    resetColor();
                }
                std::cout << std::endl;
            }
        }
        std::cout << "----------------------------------------" << std::endl;
    } else {
        std::cout << "Election not found." << std::endl;
    }
}

// Get the indices of candidates in an election using arrays
void VotingSystem::getCandidateIndices(const std::string& electionId, int* indices, int maxCount, int& count) {
    Election* election = electionManager.getElectionById(electionId);
    count = 0;
    
    if (election != nullptr) {
        for (int i = 0; i < election->getCandidateCount() && count < maxCount; i++) {
            indices[count++] = i;
        }
    }
}

// Display detailed information about a candidate
void VotingSystem::displayCandidateSummary(const std::string& electionId, int candidateIndex) {
    Election* election = electionManager.getElectionById(electionId);
    
    if (election != nullptr) {
        if (candidateIndex >= 0 && candidateIndex < election->getCandidateCount()) {
            Candidate* candidate = election->getCandidate(candidateIndex);
            if (candidate) {
                std::cout << "----------------------------------------" << std::endl;
                std::cout << "Candidate Name: " << candidate->getName() << std::endl;
                std::cout << "Party Affiliation: " << candidate->getPartyAffiliation() << std::endl;
                
                // Show party symbol in a random color if it exists
                if (!candidate->getPartySymbol().empty()) {
                    // Choose a random color (3-15 to avoid black and dark colors)
                    int colorCode = 3 + (candidateIndex % 13);
                    setColor(static_cast<Color>(colorCode));
                    std::cout << "Party Symbol: " << candidate->getPartySymbol() << std::endl;
                    resetColor();
                }
                
                std::cout << "Votes Received: " << candidate->getVoteCount() << std::endl;
                std::cout << "----------------------------------------" << std::endl;
            }
        } else {
            std::cout << "Invalid candidate index." << std::endl;
        }
    } else {
        std::cout << "Election not found." << std::endl;
    }
}

// Display a live countdown timer for an election
void VotingSystem::displayLiveCountdown(const std::string& electionId, int seconds) {
    Election* election = electionManager.getElectionById(electionId);
    
    if (election != nullptr && election->getIsActive()) {
        std::cout << "\nLive Countdown for Election: " << election->getElectionName() << std::endl;
        std::cout << "Press any key to return to the menu..." << std::endl;
        
        // Save cursor position
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        
        // Display countdown for specified seconds or until a key is pressed
        for (int i = 0; i < seconds; i++) {
            if (_kbhit()) {
                _getch(); // Consume the key press
                break;
            }
            
            // Update the timer display
            RemainingTime time = calculateRemainingTime(election->getEndDate());
            
            // Set color based on remaining time
            if (time.hours < 1) {
                setColor(LIGHTRED); // Less than 1 hour
            } else if (time.hours < 5) {
                setColor(YELLOW); // Less than 5 hours
            } else {
                setColor(LIGHTGREEN); // More than 5 hours
            }
              // Display the countdown
            std::cout << "\rCurrent Time: " << getCurrentTime() 
                    << " | Remaining: " << std::setfill('0') << std::setw(2) << time.hours << ":" 
                    << std::setfill('0') << std::setw(2) << time.minutes << ":" 
                    << std::setfill('0') << std::setw(2) << time.seconds << " " << std::flush;
            
            resetColor();
            
            // Wait for 1 second
            Sleep(1000);
        }
        
        // Reset cursor position
        std::cout << std::endl;
    } else {
        std::cout << "Election not found or not active." << std::endl;
    }
}
