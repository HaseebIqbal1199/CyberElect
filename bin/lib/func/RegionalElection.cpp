#include "../header/RegionalElection.h"
#include <iostream>
#include <fstream>

RegionalElection::RegionalElection() {}

RegionalElection::RegionalElection(std::string id, std::string name, std::string start, std::string end, std::string reg)
    : Election(id, name, start, end), region(reg) {}

void RegionalElection::calculateResults() {
    // Simple calculation: just count votes
    for (int i = 0; i < getCandidateCount(); i++) {
        if (getCandidate(i) != nullptr) {
            // Results are stored in the candidate objects
        }
    }
}

void RegionalElection::displayResults() {
    std::cout << "\nRegional Election Results for " << getElectionName() << " (" << region << ")" << std::endl;
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
