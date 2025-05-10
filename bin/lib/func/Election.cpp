#include "../header/Election.h"
#include <iostream>
#include <sstream>
#include <fstream>

Election::Election() : isActive(false), candidateCount(0) {
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        candidates[i] = nullptr;
    }
}

Election::Election(std::string id, std::string name, std::string start, std::string end)
    : electionId(id), electionName(name), startDate(start), endDate(end), 
      isActive(false), candidateCount(0) {
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        candidates[i] = nullptr;
    }
}

Election::~Election() {
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i] != nullptr) {
            delete candidates[i];
        }
    }
}

std::string Election::getElectionId() const { return electionId; }
std::string Election::getElectionName() const { return electionName; }
std::string Election::getStartDate() const { return startDate; }
std::string Election::getEndDate() const { return endDate; }
bool Election::getIsActive() const { return isActive; }

bool Election::startElection() {
    isActive = true;
    return true;
}

bool Election::endElection() {
    isActive = false;
    return true;
}

bool Election::addCandidate(std::string name, std::string partyAffiliation) {
    if (candidateCount >= MAX_CANDIDATES) {
        return false;
    }
    
    std::string candidateId = electionId + "_C" + std::to_string(candidateCount + 1);
    candidates[candidateCount] = new Candidate(candidateId, name, partyAffiliation);
    candidateCount++;
    return true;
}

void Election::displayCandidates() {
    std::cout << "Candidates for election " << electionName << ":" << std::endl;
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i] != nullptr) {
            candidates[i]->displayInfo();
        }
    }
}

Candidate* Election::getCandidate(int index) {
    if (index >= 0 && index < candidateCount) {
        return candidates[index];
    }
    return nullptr;
}

Candidate* Election::getCandidateById(std::string candidateId) {
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i] != nullptr && candidates[i]->getCandidateId() == candidateId) {
            return candidates[i];
        }
    }
    return nullptr;
}

int Election::getCandidateCount() const { return candidateCount; }

bool Election::candidateExists(std::string candidateId) {
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i] != nullptr && candidates[i]->getCandidateId() == candidateId) {
            return true;
        }
    }
    return false;
}

void Election::saveToFile() {
    // Open the unified election file in append mode
    std::ofstream file("data/elections/entries.txt", std::ios::app);
    if (file.is_open()) {
        // Save election type first
        file << getType() << std::endl;
        
        // Save common data
        saveData(file);
        
        file.close();
    }
}

void Election::loadFromFile(std::string electionId) {
    std::ifstream file("data/elections/entries.txt");
    if (file.is_open()) {
        std::string line;
        std::string currentType;
        std::string currentId;
        
        while (std::getline(file, line)) {
            currentType = line; // Type is on the first line
            
            // Read next line to check ID
            std::getline(file, currentId);
            
            if (currentId == electionId) {
                // Found the election, restore the ID and load the data
                this->electionId = currentId;
                
                // Reset the file cursor to start reading from the next line
                file.seekg(-currentId.length() - 1, std::ios::cur);
                loadData(file);
                file.close();
                return;
            } else {
                // Skip to the next election by reading all candidate data
                std::getline(file, line); // electionName
                std::getline(file, line); // startDate
                std::getline(file, line); // endDate
                std::getline(file, line); // isActive
                
                std::string candidateCountStr;
                std::getline(file, candidateCountStr);
                int candidateCount = std::stoi(candidateCountStr);
                
                // Skip candidate data (4 lines per candidate)
                for (int i = 0; i < candidateCount * 4; i++) {
                    std::getline(file, line);
                }
            }
        }
        
        file.close();
    }
}

// Helper methods for unified file operations
void Election::saveData(std::ofstream& file) {
    // Common election data
    file << electionId << std::endl;
    file << electionName << std::endl;
    file << startDate << std::endl;
    file << endDate << std::endl;
    file << (isActive ? "Active" : "Inactive") << std::endl;
    file << candidateCount << std::endl;
    
    // Save candidate data
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i] != nullptr) {
            file << candidates[i]->getCandidateId() << std::endl;
            file << candidates[i]->getName() << std::endl;
            file << candidates[i]->getPartyAffiliation() << std::endl;
            file << candidates[i]->getVoteCount() << std::endl;
        }
    }
}

void Election::loadData(std::ifstream& file) {
    // Read common election data
    std::getline(file, electionId);
    std::getline(file, electionName);
    std::getline(file, startDate);
    std::getline(file, endDate);
    
    std::string activeStr;
    std::getline(file, activeStr);
    isActive = (activeStr == "Active");
      std::string countStr;
    std::getline(file, countStr);
    
    // Parse candidate count safely without using stoi
    candidateCount = 0;
    for (char c : countStr) {
        if (c >= '0' && c <= '9') {
            candidateCount = candidateCount * 10 + (c - '0');
        }
    }
    
    // Load candidate data
    for (int i = 0; i < candidateCount; i++) {
        std::string candId, name, party, votes;
        
        std::getline(file, candId);
        std::getline(file, name);
        std::getline(file, party);
        std::getline(file, votes);
          candidates[i] = new Candidate(candId, name, party);
        
        // Set vote count safely without using stoi
        int voteCount = 0;
        for (char c : votes) {
            if (c >= '0' && c <= '9') {
                voteCount = voteCount * 10 + (c - '0');
            }
        }
        
        // Set vote count (using helper method to access private field)
        for (int j = 0; j < voteCount; j++) {
            candidates[i]->incrementVotes();
        }
    }
}
