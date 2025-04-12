#include "../header/Administrator.h"
#include <iostream>
#include <fstream>

Administrator::Administrator() {}

Administrator::Administrator(std::string username, std::string password, std::string name, std::string id)
    : User(username, password, name, id) {}

bool Administrator::login() {
    std::cout << "Administrator " << getName() << " logged in successfully." << std::endl;
    return true;
}

void Administrator::createElection(std::string type) {
    // This functionality is handled by VotingSystem class
}

void Administrator::addCandidate(std::string electionId, std::string candidateName, std::string partyAffiliation) {
    // This functionality is handled by VotingSystem class
}

void Administrator::viewResults(std::string electionId) {
    // This functionality is handled by VotingSystem class
}

void Administrator::viewAllElections() {
    // This functionality is handled by VotingSystem class
}

void Administrator::saveToFile() {
    std::ofstream file("data/admins/" + getID() + ".txt");
    if (file.is_open()) {
        file << getUsername() << std::endl;
        file << getPassword() << std::endl;
        file << getName() << std::endl;
        file << getID() << std::endl;
        file.close();
    }
}

void Administrator::loadFromFile(std::string userId) {
    std::ifstream file("data/admins/" + userId);
    if (file.is_open()) {
        getline(file, username);
        getline(file, password);
        getline(file, name);
        getline(file, id);
        file.close();
    }
}
