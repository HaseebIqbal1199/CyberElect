#include "../header/Election.h"
#include <iostream>

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
