#include "../header/LocalElection.h"
#include <iostream>
#include <fstream>

LocalElection::LocalElection() {}

LocalElection::LocalElection(std::string id, std::string name, std::string start, std::string end, std::string loc)
    : Election(id, name, start, end), locality(loc) {}

void LocalElection::calculateResults() {
    // Simple calculation: just count votes
    for (int i = 0; i < getCandidateCount(); i++) {
        if (getCandidate(i) != nullptr) {
            // Results are stored in the candidate objects
        }
    }
}

void LocalElection::displayResults() {
    std::cout << "\nLocal Election Results for " << getElectionName() << " (" << locality << ")" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
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
