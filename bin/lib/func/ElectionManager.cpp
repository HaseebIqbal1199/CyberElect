#include "../header/ElectionManager.h"
#include <iostream>
#include <fstream>

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
        return election->addCandidate(name, party);
    }
    return false;
}

void ElectionManager::viewElectionResults(std::string electionId) {
    Election* election = getElection(electionId);
    if (election != nullptr) {
        election->calculateResults();
        election->displayResults();
    }
}

void ElectionManager::loadAllElections() {
    // This would load all elections from files in the data/elections directory
    // For each file found, create appropriate election object and load its data
    
    // Example implementation (simplified):
    std::ifstream localIndex("data/elections/local_index.txt");
    std::ifstream nationalIndex("data/elections/national_index.txt");
    std::ifstream regionalIndex("data/elections/regional_index.txt");
    
    std::string electionId;
    
    // Load local elections
    while (localIndex >> electionId && electionCount < MAX_ELECTIONS) {
        elections[electionCount] = new LocalElection();
        elections[electionCount]->loadFromFile(electionId);
        electionCount++;
    }
    
    // Load national elections
    while (nationalIndex >> electionId && electionCount < MAX_ELECTIONS) {
        elections[electionCount] = new NationalElection();
        elections[electionCount]->loadFromFile(electionId);
        electionCount++;
    }
    
    // Load regional elections
    while (regionalIndex >> electionId && electionCount < MAX_ELECTIONS) {
        elections[electionCount] = new RegionalElection();
        elections[electionCount]->loadFromFile(electionId);
        electionCount++;
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
