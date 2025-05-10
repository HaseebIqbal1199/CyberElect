// New helper methods for menu-based selection
#include "../header/VotingSystem.h"
#include <iostream>
#include <vector>

using namespace std;

std::vector<std::string> VotingSystem::getRunningElectionIds() {
    std::vector<std::string> activeElections;
    
    // Look through up to MAX_ELECTIONS (matches ElectionManager::MAX_ELECTIONS)
    const int MAX_POSSIBLE_ELECTIONS = 20;
    for (int i = 0; i < MAX_POSSIBLE_ELECTIONS; i++) {
        Election* election = electionManager.getElection("E" + to_string(i+1));
        if (election && election->getIsActive()) {
            activeElections.push_back(election->getElectionId());
        }
    }
    
    return activeElections;
}

std::vector<std::string> VotingSystem::getAllElectionIds() {
    std::vector<std::string> allElections;
    
    // Look through up to MAX_ELECTIONS (matches ElectionManager::MAX_ELECTIONS)
    const int MAX_POSSIBLE_ELECTIONS = 20;
    for (int i = 0; i < MAX_POSSIBLE_ELECTIONS; i++) {
        Election* election = electionManager.getElection("E" + to_string(i+1));
        if (election) {
            allElections.push_back(election->getElectionId());
        }
    }
    
    return allElections;
}

std::vector<int> VotingSystem::getCandidateIndices(const std::string& electionId) {
    std::vector<int> indices;
    
    Election* election = electionManager.getElection(electionId);
    if (election) {
        for (int i = 0; i < election->getCandidateCount(); i++) {
            if (election->getCandidate(i) != nullptr) {
                indices.push_back(i);
            }
        }
    }
    
    return indices;
}

void VotingSystem::displayElectionSummary(const std::string& electionId) {
    Election* election = electionManager.getElection(electionId);
    if (election) {
        cout << "Election: " << election->getElectionName() 
             << " (ID: " << election->getElectionId() << ", Type: " << election->getType() << ")" << endl;
    }
}

void VotingSystem::displayCandidateSummary(const std::string& electionId, int candidateIndex) {
    Election* election = electionManager.getElection(electionId);
    if (election) {
        Candidate* candidate = election->getCandidate(candidateIndex);
        if (candidate) {
            cout << "Candidate: " << candidate->getName() 
                 << " (Party: " << candidate->getPartyAffiliation() << ")" << endl;
        }
    }
}
