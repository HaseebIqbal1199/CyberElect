#include "../header/ElectionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>

ElectionManager::ElectionManager() : electionCount(0) {
    for (int i = 0; i < MAX_ELECTIONS; i++) {
        elections[i] = nullptr;
    }
    loadAllElections();
}

ElectionManager::~ElectionManager() {
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr) {
            delete elections[i];
        }
    }
}

bool ElectionManager::createElection(std::string type, std::string name, std::string start, std::string end, 
                                  std::string region, int regions) {
    if (electionCount >= MAX_ELECTIONS) {
        return false;
    }

    std::string electionId = "E" + std::to_string(electionCount + 1);
    
    if (type == "local") {
        elections[electionCount] = new LocalElection(electionId, name, start, end, region);
    }
    else if (type == "national") {
        elections[electionCount] = new NationalElection(electionId, name, start, end, region, regions);
    }
    else if (type == "regional") {
        elections[electionCount] = new RegionalElection(electionId, name, start, end, region);
    }
    else {
        return false;
    }
    
    // Save the newly created election to the unified file
    elections[electionCount]->saveToFile();
    
    electionCount++;
    return true;
}

void ElectionManager::displayActiveElections() {
    // Get current system time
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    std::stringstream ss;
    ss << (localTime->tm_year + 1900) << "-" 
       << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-" 
       << std::setfill('0') << std::setw(2) << localTime->tm_mday;
    
    std::string currentDate = ss.str();
    std::cout << "\nCurrent Date: " << currentDate << std::endl;
    
    std::cout << "\nActive Elections:" << std::endl;
    std::cout << "----------------" << std::endl;
    
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr && elections[i]->getIsActive()) {
            std::cout << "ID: " << elections[i]->getElectionId() 
                     << " | Name: " << elections[i]->getElectionName() << std::endl;
        }
    }
}

void ElectionManager::displayAllElections() {
    // Get current system time
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    std::stringstream ss;
    ss << (localTime->tm_year + 1900) << "-" 
       << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-" 
       << std::setfill('0') << std::setw(2) << localTime->tm_mday;
    
    std::string currentDate = ss.str();
    std::cout << "\nCurrent Date: " << currentDate << std::endl;
    
    std::cout << "\nAll Elections:" << std::endl;
    std::cout << "-------------" << std::endl;
    
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr) {
            std::cout << "ID: " << elections[i]->getElectionId() 
                     << " | Name: " << elections[i]->getElectionName()
                     << " | Status: " << (elections[i]->getIsActive() ? "Active" : "Inactive") 
                     << std::endl;
        }
    }
}

Election* ElectionManager::getElection(std::string electionId) {
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr && elections[i]->getElectionId() == electionId) {
            return elections[i];
        }
    }
    return nullptr;
}

bool ElectionManager::addCandidateToElection(std::string electionId, std::string name, std::string party, std::string symbol) {
    Election* election = getElection(electionId);
    if (election != nullptr) {
        // First add the candidate normally
        bool result = election->addCandidate(name, party);
        
        if (result) {
            // If a symbol was provided, update the last added candidate with the symbol
            if (!symbol.empty()) {
                int lastCandidateIndex = election->getCandidateCount() - 1;
                if (lastCandidateIndex >= 0) {
                    Candidate* candidate = election->getCandidate(lastCandidateIndex);
                    if (candidate) {
                        candidate->setPartySymbol(symbol);
                    }
                }
            }
            
            // Re-save the election with updated candidate data
            updateElectionFile(election);
        }
        return result;
    }
    return false;
}

void ElectionManager::updateElectionFile(Election* election) {
    // This is a simplified approach. In a real-world scenario, 
    // we would need a more efficient way to update entries in the file.
    
    // First, load all elections
    std::vector<Election*> allElections;
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr) {
            allElections.push_back(elections[i]);
        }
    }
    
    // Clear the file and rewrite all elections
    std::ofstream file("data/elections/entries.txt", std::ios::trunc);
    if (file.is_open()) {
        for (Election* e : allElections) {
            file << e->getType() << std::endl;
            e->saveData(file);
        }
        file.close();
    }
}

void ElectionManager::viewElectionResults(std::string electionId) {
    Election* election = getElection(electionId);
    if (election != nullptr) {
        election->calculateResults();
        election->displayResults();
    }
}

void ElectionManager::loadAllElections() {
    // Load all elections from the unified file
    std::ifstream file("data/elections/entries.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line) && electionCount < MAX_ELECTIONS) {
            std::string type = line;
            std::string tempId;
            
            // Read the ID to check what type of election it is
            std::streampos pos = file.tellg();  // Save current position
            std::getline(file, tempId);
            file.seekg(pos);  // Go back to start of ID line
            
            // Create the appropriate election type
            if (type == "local") {
                elections[electionCount] = new LocalElection();
            }
            else if (type == "national") {
                elections[electionCount] = new NationalElection();
            }
            else if (type == "regional") {
                elections[electionCount] = new RegionalElection();
            }
            else {
                // Skip this entry if unknown type
                continue;
            }
            
            // Load the election data
            elections[electionCount]->loadData(file);
            electionCount++;
        }
        file.close();
    }
}

bool ElectionManager::electionExists(std::string electionId) {
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr && elections[i]->getElectionId() == electionId) {
            return true;
        }
    }
    return false;
}

// Alias for getElection to match the function signature in VotingSystem.cpp
Election* ElectionManager::getElectionById(const std::string& electionId) {
    return getElection(electionId);
}

// Get IDs of all active/running elections
std::vector<std::string> ElectionManager::getRunningElectionIds() {
    std::vector<std::string> runningElectionIds;
    
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr && elections[i]->getIsActive()) {
            runningElectionIds.push_back(elections[i]->getElectionId());
        }
    }
    
    return runningElectionIds;
}

// Get IDs of all elections, regardless of status
std::vector<std::string> ElectionManager::getAllElectionIds() {
    std::vector<std::string> allElectionIds;
    
    for (int i = 0; i < electionCount; i++) {
        if (elections[i] != nullptr) {
            allElectionIds.push_back(elections[i]->getElectionId());
        }
    }
    
    return allElectionIds;
}
