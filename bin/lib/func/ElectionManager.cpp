#include "../header/ElectionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
    // This method rewrites the entire elections file with updated data
    
    // Create a temporary file for writing
    std::string tempFile = "data/elections/temp.txt";
    std::ofstream outFile(tempFile);
    
    if (outFile.is_open()) {
        // Write all elections to the temp file
        for (int i = 0; i < electionCount; i++) {
            if (elections[i] != nullptr) {
                // Write type first (not part of saveData)
                outFile << elections[i]->getType() << std::endl;
                
                // Write election data
                outFile << elections[i]->getElectionId() << std::endl;
                outFile << elections[i]->getElectionName() << std::endl;
                outFile << elections[i]->getStartDate() << std::endl;
                outFile << elections[i]->getEndDate() << std::endl;
                outFile << (elections[i]->getIsActive() ? "Active" : "Inactive") << std::endl;
                
                // Write candidate info
                int candidateCount = elections[i]->getCandidateCount();
                outFile << candidateCount << std::endl;
                
                for (int j = 0; j < candidateCount; j++) {
                    Candidate* candidate = elections[i]->getCandidate(j);
                    if (candidate != nullptr) {
                        outFile << candidate->getCandidateId() << std::endl;
                        outFile << candidate->getName() << std::endl;
                        outFile << candidate->getPartyAffiliation() << std::endl;
                        outFile << candidate->getPartySymbol() << std::endl;
                        outFile << candidate->getVoteCount() << std::endl;
                    }
                }
            }
        }
          outFile.close();
        
        // Replace the original file with the temp file
        std::remove("data/elections/entries.txt");
        std::rename(tempFile.c_str(), "data/elections/entries.txt");
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
                std::getline(file, line); // Skip ID
                std::getline(file, line); // Skip name
                std::getline(file, line); // Skip start date
                std::getline(file, line); // Skip end date
                std::getline(file, line); // Skip active status
                std::getline(file, line); // Skip candidate count
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

// Get IDs of all active/running elections using arrays
void ElectionManager::getRunningElectionIds(std::string* electionIds, int maxCount, int& count) {
    count = 0;
    
    for (int i = 0; i < electionCount && count < maxCount; i++) {
        if (elections[i] != nullptr && elections[i]->getIsActive()) {
            electionIds[count++] = elections[i]->getElectionId();
        }
    }
}

// Get IDs of all elections, regardless of status using arrays
void ElectionManager::getAllElectionIds(std::string* electionIds, int maxCount, int& count) {
    count = 0;
    
    for (int i = 0; i < electionCount && count < maxCount; i++) {
        if (elections[i] != nullptr) {
            electionIds[count++] = elections[i]->getElectionId();
        }
    }
}

// Public method to update an election
bool ElectionManager::updateElection(Election* election) {
    if (election == nullptr || !electionExists(election->getElectionId())) {
        return false;
    }
    
    // Use the private method to update the election data in the file
    updateElectionFile(election);
    return true;
}

// Method to directly update an election's active status
bool ElectionManager::setElectionStatus(std::string electionId, bool isActive) {
    Election* election = getElection(electionId);
    if (election != nullptr) {
        if (isActive) {
            election->startElection();
        } else {
            election->endElection();
        }
        updateElectionFile(election);
        return true;
    }
    return false;
}
