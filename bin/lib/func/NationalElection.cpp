#include "../header/NationalElection.h"
#include <iostream>
#include <fstream>

NationalElection::NationalElection() : regions(0) {}

NationalElection::NationalElection(std::string id, std::string name, std::string start, std::string end, 
                                 std::string country, int reg)
    : Election(id, name, start, end), country(country), regions(reg) {}

void NationalElection::calculateResults() {
    // Simple calculation: just count votes
    for (int i = 0; i < getCandidateCount(); i++) {
        if (getCandidate(i) != nullptr) {
            // Results are stored in the candidate objects
        }
    }
}

void NationalElection::displayResults() {
    std::cout << "\nNational Election Results for " << getElectionName() << " (" << country << ")" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Number of Regions: " << regions << std::endl;
    
    int maxVotes = 0;
    Candidate* winner = nullptr;
    
    for (int i = 0; i < getCandidateCount(); i++) {
        Candidate* candidate = getCandidate(i);
        if (candidate != nullptr) {
            candidate->displayInfo();
            if (candidate->getVoteCount() > maxVotes) {
                maxVotes = candidate->getVoteCount();
                winner = candidate;
            }
        }
    }
    
    if (winner != nullptr) {
        std::cout << "\nWinner: " << winner->getName() << " (" << winner->getPartyAffiliation() 
                 << ") with " << winner->getVoteCount() << " votes" << std::endl;
    }
}

void NationalElection::saveToFile() {
    std::ofstream file("data/elections/national_" + getElectionId() + ".txt");
    if (file.is_open()) {
        file << getElectionId() << std::endl;
        file << getElectionName() << std::endl;
        file << startDate << std::endl;
        file << endDate << std::endl;
        file << country << std::endl;
        file << regions << std::endl;
        file << isActive << std::endl;
        file << getCandidateCount() << std::endl;
        
        for (int i = 0; i < getCandidateCount(); i++) {
            if (getCandidate(i) != nullptr) {
                getCandidate(i)->saveToFile(getElectionId());
            }
        }
        file.close();
    }
}

void NationalElection::loadFromFile(std::string eId) {
    std::ifstream file("data/elections/national_" + eId + ".txt");
    if (file.is_open()) {
        getline(file, electionId);
        getline(file, electionName);
        getline(file, startDate);
        getline(file, endDate);
        getline(file, country);
        file >> regions;
        file >> isActive;
        file >> candidateCount;
        
        for (int i = 0; i < candidateCount; i++) {
            std::string candidateId = electionId + "_C" + std::to_string(i + 1);
            candidates[i] = new Candidate();
            candidates[i]->loadFromFile(electionId, candidateId);
        }
        file.close();
    }
}
