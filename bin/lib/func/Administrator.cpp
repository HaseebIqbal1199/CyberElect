// # by Rafaqat
#include "../header/Administrator.h"
#include <iostream>
#include <fstream>
#include <sstream>

Administrator::Administrator() {
    role = "admin";
}

Administrator::Administrator(std::string username, std::string password, std::string name, std::string id)
    : User(username, password, name, id, "admin") {}

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
    // First, check if the user already exists in the common file
    bool userExists = false;
    std::string id = getID();
    std::string tempFile = "data/Users/temp.txt";
    
    std::ifstream inFile("data/Users/Creds.txt");
    std::ofstream outFile(tempFile);
    
    if (inFile.is_open() && outFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string fileId;
            std::getline(ss, fileId, ',');
            
            if (fileId == id) {
                // Found the user, update their record
                outFile << id << "," << getUsername() << "," << getPassword() << "," 
                       << getName() << "," << getRole() << ",0" << std::endl;
                userExists = true;
            } else {
                // Copy other users' data as is
                outFile << line << std::endl;
            }
        }
        
        // If user doesn't exist, add them to the file
        if (!userExists) {
            outFile << id << "," << getUsername() << "," << getPassword() << "," 
                   << getName() << "," << getRole() << ",0" << std::endl;
        }
        
        inFile.close();
        outFile.close();
        
        // Replace the original file with the temp file
        std::remove("data/Users/Creds.txt");
        std::rename(tempFile.c_str(), "data/Users/Creds.txt");
    } else {
        // File doesn't exist yet, create it
        std::ofstream newFile("data/Users/Creds.txt");
        if (newFile.is_open()) {
            newFile << id << "," << getUsername() << "," << getPassword() << "," 
                   << getName() << "," << getRole() << ",0" << std::endl;
            newFile.close();
        }
    }
}

void Administrator::loadFromFile(std::string userId) {
    // First try to load from the new common file
    bool found = false;
    std::ifstream file("data/Users/Creds.txt");
      if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string fileId;
            std::getline(ss, fileId, ',');
              if (fileId == userId) {
                // Found the user
                found = true;
                id = fileId;
                std::getline(ss, username, ',');
                std::getline(ss, password, ',');
                std::getline(ss, name, ',');
                std::getline(ss, role, ',');
                break;
            }
        }
        file.close();
    }
}
