// BY REHAN
#include "../header/Voter.h"
#include <iostream>
#include <fstream>
#include <sstream>

Voter::Voter() : hasVoted(false), votedElectionsCount(0), cnic(""), dateOfBirth(""), city("") {
    role = "voter";
}

Voter::Voter(std::string username, std::string password, std::string name, std::string id,
             std::string cnic, std::string dob, std::string city)
    : User(username, password, name, id, "voter"), hasVoted(false), votedElectionsCount(0),
      cnic(cnic), dateOfBirth(dob), city(city) {}

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
                       << getName() << "," << getRole() << "," << cnic << "," << dateOfBirth << "," << city << "," << votedElectionsCount;
                
                for (int i = 0; i < votedElectionsCount; i++) {
                    outFile << "," << votedElections[i];
                }
                outFile << std::endl;
                userExists = true;
            } else {
                // Copy other users' data as is
                outFile << line << std::endl;
            }
        }
        
        // If user doesn't exist, add them to the file
        if (!userExists) {
            outFile << id << "," << getUsername() << "," << getPassword() << "," 
                   << getName() << "," << getRole() << "," << cnic << "," << dateOfBirth << "," << city << "," << votedElectionsCount;
            
            for (int i = 0; i < votedElectionsCount; i++) {
                outFile << "," << votedElections[i];
            }
            outFile << std::endl;
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
                   << getName() << "," << getRole() << "," << cnic << "," << dateOfBirth << "," << city << "," << votedElectionsCount;
            
            for (int i = 0; i < votedElectionsCount; i++) {
                newFile << "," << votedElections[i];
            }
            newFile << std::endl;
            newFile.close();
        }
    }
}

void Voter::loadFromFile(std::string userId) {
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
                
                // Get new fields
                std::getline(ss, cnic, ',');
                std::getline(ss, dateOfBirth, ',');
                std::getline(ss, city, ',');
                  
                std::string votedCountStr;
                std::getline(ss, votedCountStr, ',');
                
                // Parse votedElectionsCount safely without using stoi
                votedElectionsCount = 0;
                for (char c : votedCountStr) {
                    if (c >= '0' && c <= '9') {
                        votedElectionsCount = votedElectionsCount * 10 + (c - '0');
                    }
                }
                
                for (int i = 0; i < votedElectionsCount; i++) {
                    if (i < 9) {  // Make sure we don't go beyond array bounds
                        std::getline(ss, votedElections[i], ',');
                    }
                }
                break;
            }
        }
        file.close();
    }
}
