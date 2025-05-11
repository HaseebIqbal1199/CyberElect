// New helper methods for menu-based selection
#include "../header/VotingSystem.h"
#include <iostream>

using namespace std;

void VotingSystem::getRunningElectionIds(std::string* electionIds, int maxCount, int& count) {
    count = 0;
    
    // Look through up to MAX_ELECTIONS (matches ElectionManager::MAX_ELECTIONS)
    const int MAX_POSSIBLE_ELECTIONS = 20;
    for (int i = 0; i < MAX_POSSIBLE_ELECTIONS && count < maxCount; i++) {
        Election* election = electionManager.getElection("E" + to_string(i+1));
        if (election && election->getIsActive()) {
            electionIds[count++] = election->getElectionId();
        }
    }
}

void VotingSystem::getAllElectionIds(std::string* electionIds, int maxCount, int& count) {
    count = 0;
    
    // Look through up to MAX_ELECTIONS (matches ElectionManager::MAX_ELECTIONS)
    const int MAX_POSSIBLE_ELECTIONS = 20;
    for (int i = 0; i < MAX_POSSIBLE_ELECTIONS && count < maxCount; i++) {
        Election* election = electionManager.getElection("E" + to_string(i+1));
        if (election) {
            electionIds[count++] = election->getElectionId();
        }
    }
}

void VotingSystem::getCandidateIndices(const std::string& electionId, int* indices, int maxCount, int& count) {
    count = 0;
    
    Election* election = electionManager.getElection(electionId);
    if (election) {
        for (int i = 0; i < election->getCandidateCount() && count < maxCount; i++) {
            if (election->getCandidate(i) != nullptr) {
                indices[count++] = i;
            }
        }
    }
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
