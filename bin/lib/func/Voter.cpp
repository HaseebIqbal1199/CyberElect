// BY REHAN
#include "../header/Voter.h"
#include <iostream>
#include <fstream>

Voter::Voter() : hasVoted(false), votedElectionsCount(0) {}

Voter::Voter(std::string username, std::string password, std::string name, std::string id)
    : User(username, password, name, id), hasVoted(false), votedElectionsCount(0) {}

bool Voter::login() {
    std::cout << "Voter " << getName() << " logged in successfully." << std::endl;
    return true;
}

bool Voter::hasVotedInElection(std::string electionId) {
    for (int i = 0; i < votedElectionsCount; i++) {
        if (votedElections[i] == electionId) {
            return true;
        }
    }
    return false;
}

void Voter::addVotedElection(std::string electionId) {
    if (votedElectionsCount < 10) {
        votedElections[votedElectionsCount++] = electionId;
    }
}

bool Voter::checkVoteStatus(std::string electionId) {
    return hasVotedInElection(electionId);
}

bool Voter::castVote(std::string electionId, std::string candidateId) {
    if (!hasVotedInElection(electionId)) {
        addVotedElection(electionId);
        return true;
    }
    return false;
}

void Voter::saveToFile() {
    std::ofstream file("data/voters/" + getID() + ".txt");
    if (file.is_open()) {
        file << getUsername() << std::endl;
        file << getPassword() << std::endl;
        file << getName() << std::endl;
        file << getID() << std::endl;
        file << votedElectionsCount << std::endl;
        for (int i = 0; i < votedElectionsCount; i++) {
            file << votedElections[i] << std::endl;
        }
        file.close();
    }
}

void Voter::loadFromFile(std::string userId) {
    std::ifstream file("data/voters/" + userId);
    if (file.is_open()) {
        getline(file, username);
        getline(file, password);
        getline(file, name);
        getline(file, id);
        file >> votedElectionsCount;
        file.ignore();
        for (int i = 0; i < votedElectionsCount; i++) {
            getline(file, votedElections[i]);
        }
        file.close();
    }
}
