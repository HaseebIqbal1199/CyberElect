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
