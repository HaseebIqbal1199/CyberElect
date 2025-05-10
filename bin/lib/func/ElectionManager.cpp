#include "../header/ElectionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

bool ElectionManager::addCandidateToElection(std::string electionId, std::string name, std::string party) {
    Election* election = getElection(electionId);
    if (election != nullptr) {
        bool result = election->addCandidate(name, party);
        if (result) {
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
