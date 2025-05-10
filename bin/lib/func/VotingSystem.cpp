// # by Rafaqat, Haseeb, Rehan
#include "../header/VotingSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <ctime>
#include <vector>

using namespace std;

VotingSystem::VotingSystem() : currentUser(nullptr) {}

VotingSystem::~VotingSystem() {
    if (currentUser != nullptr) {
        delete currentUser;
    }
}

bool VotingSystem::registerVoter(string username, string password, string name) {
    stringstream ss;
    ss << "V" << time(nullptr);
    string id = ss.str();
    
    Voter* voter = new Voter(username, password, name, id);
    voter->saveToFile();
    delete voter;
    return true;
}

bool VotingSystem::registerAdmin(string username, string password, string name) {
    stringstream ss;
    ss << "A" << time(nullptr);
    string id = ss.str();
    
    Administrator* admin = new Administrator(username, password, name, id);
    admin->saveToFile();
    delete admin;
    return true;
}

bool VotingSystem::loginUser(string username, string password, string role) {
    if (currentUser != nullptr) {
        delete currentUser;
        currentUser = nullptr;
    }

    // Look for the user in the unified credentials file
    ifstream credsFile("data/Users/Creds.txt");
    if (credsFile.is_open()) {
        string line;
        while (getline(credsFile, line)) {
            stringstream ss(line);
            string id, fileUsername, filePassword, name, fileRole;
            
            getline(ss, id, ',');
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
            getline(ss, name, ',');
            getline(ss, fileRole, ',');
            
            if (fileUsername == username && filePassword == password && fileRole == role) {
                // User found with matching credentials
                if (role == "voter") {
                    Voter* voter = new Voter();
                    voter->loadFromFile(id);
                    currentUser = voter;
                    credsFile.close();
                    return true;
                } else if (role == "admin") {
                    Administrator* admin = new Administrator();
                    admin->loadFromFile(id);
                    currentUser = admin;
                    credsFile.close();
                    return true;
                }
            }
        }
        credsFile.close();
    }
    
    return false;
}

void VotingSystem::logout() {
    if (currentUser != nullptr) {
        delete currentUser;
        currentUser = nullptr;
        cout << "Logged out successfully." << endl;
    }
}

bool VotingSystem::createElection(string type, string name, string start, string end, string region, int regions) {
    // Use the electionManager to create the election
    return electionManager.createElection(type, name, start, end, region, regions);
}

void VotingSystem::displayElectionInfo(const string& electionId) {
    // Check if the election exists in memory
    Election* election = electionManager.getElection(electionId);
    if (election != nullptr) {
        cout << "\nElection Details:" << endl;
        cout << "---------------" << endl;
        cout << "ID: " << election->getElectionId() << endl;
        cout << "Name: " << election->getElectionName() << endl;
        cout << "Type: " << election->getType() << endl;
        
        // Display type-specific details
        if (election->getType() == "local") {
            LocalElection* localElection = dynamic_cast<LocalElection*>(election);
            if (localElection) {
                cout << "Locality: " << localElection->getLocality() << endl;
            }
        } else if (election->getType() == "regional") {
            RegionalElection* regionalElection = dynamic_cast<RegionalElection*>(election);
            if (regionalElection) {
                cout << "Region: " << regionalElection->getRegion() << endl;
            }
        } else if (election->getType() == "national") {
            NationalElection* nationalElection = dynamic_cast<NationalElection*>(election);
            if (nationalElection) {
                cout << "Country: " << nationalElection->getCountry() << endl;
                cout << "Regions: " << nationalElection->getRegions() << endl;            }
        }
        
        cout << "Start Date: " << election->getStartDate() << endl;
        cout << "End Date: " << election->getEndDate() << endl;
        cout << "Status: " << (election->getIsActive() ? "Active" : "Inactive") << endl;
        
        if (election->getCandidateCount() > 0) {
            cout << "\nCandidates:" << endl;
            cout << "----------" << endl;
            
            for (int i = 0; i < election->getCandidateCount(); i++) {
                Candidate* candidate = election->getCandidate(i);
                if (candidate != nullptr) {
                    cout << "ID: " << (i + 1) << endl;
                    cout << "Name: " << candidate->getName() << endl;
                    cout << "Party: " << candidate->getPartyAffiliation() << endl;
                    cout << string(20, '-') << endl;
                }
            }
        }
        return;
    }
    
    // Fall back to reading from the file directly if not in memory
    string filename = "data/elections/entries.txt";
    ifstream file(filename);
    
    if (file.is_open()) {
        string line;
        bool foundElection = false;
        
        while (getline(file, line)) {
            string type = line;
            string id, name, start, end, region, status;
            int regions = 0;
            
            getline(file, id);
            
            if (id == electionId) {
                foundElection = true;
                getline(file, name);
                getline(file, start);
                getline(file, end);
                getline(file, status);
                
                cout << "\nElection Details:" << endl;
                cout << "---------------" << endl;
                cout << "ID: " << id << endl;
                cout << "Name: " << name << endl;
                cout << "Type: " << type << endl;
                
                if (type == "local" || type == "regional") {
                    cout << "Region: " << region << endl;
                } else if (type == "national") {
                    cout << "Country: " << region << endl;
                    cout << "Regions: " << regions << endl;
                }
                
                cout << "Start Date: " << start << endl;
                cout << "End Date: " << end << endl;
                cout << "Status: " << status << endl;
                  // Read number of candidates
                string candidateCountStr;
                getline(file, candidateCountStr);
                
                // Parse candidate count safely without using stoi
                int candidateCount = 0;
                for (char c : candidateCountStr) {
                    if (c >= '0' && c <= '9') {
                        candidateCount = candidateCount * 10 + (c - '0');
                    } else {
                        candidateCount = 0;
                        break;
                    }
                }
                
                if (candidateCount > 0) {
                    cout << "\nCandidates:" << endl;
                    cout << "----------" << endl;
                    
                    for (int i = 0; i < candidateCount; i++) {
                        string candidateId, candidateName, partyAffiliation, votes;
                        getline(file, candidateId);
                        getline(file, candidateName);
                        getline(file, partyAffiliation);
                        getline(file, votes);
                        
                        cout << "ID: " << (i + 1) << endl;
                        cout << "Name: " << candidateName << endl;
                        cout << "Party: " << partyAffiliation << endl;
                        cout << string(20, '-') << endl;
                    }
                }
                break;
            } else {
                // Skip to the next election
                getline(file, name);
                getline(file, start);
                getline(file, end);
                getline(file, status);
                  // Skip candidate data
                string candidateCountStr;
                getline(file, candidateCountStr);
                
                // Parse candidate count safely without using stoi
                int candidateCount = 0;
                for (char c : candidateCountStr) {
                    if (c >= '0' && c <= '9') {
                        candidateCount = candidateCount * 10 + (c - '0');
                    } else {
                        candidateCount = 0;
                        break;
                    }
                }
                
                for (int i = 0; i < candidateCount * 4; i++) {
                    getline(file, line);
                }
            }
        }
        
        file.close();
        
        if (!foundElection) {
            cout << "Election not found!" << endl;
        }
    }
}

bool VotingSystem::addCandidate(string electionId, string name, string partyAffiliation) {
    return electionManager.addCandidateToElection(electionId, name, partyAffiliation);
}

bool VotingSystem::startElection(string electionId) {
    Election* election = electionManager.getElection(electionId);
    if (election != nullptr && !election->getIsActive()) {
        election->startElection();
        // Update the election data in the file
        election->saveToFile();
        return true;
    }
    return false;
}

bool VotingSystem::endElection(string electionId) {
    Election* election = electionManager.getElection(electionId);
    if (election != nullptr && election->getIsActive()) {
        election->endElection();
        // Update the election data in the file
        election->saveToFile();
        return true;
    }
    return false;
}

void VotingSystem::viewResults(string electionId) {
    Election* election = electionManager.getElection(electionId);
    if (election != nullptr) {
        election->calculateResults();
        election->displayResults();
    } else {
        cout << "Election not found or not ended yet." << endl;
    }
}

void VotingSystem::viewAllElections() {
    electionManager.displayAllElections();
}

void VotingSystem::viewElections() {
    cout << "\nActive Elections:" << endl;
    cout << "----------------" << endl;
    
    // Use the electionManager to get active elections
    electionManager.displayActiveElections();
}

void VotingSystem::displayRunningElections() {    cout << "\nRunning Elections Status:" << endl;
    cout << "------------------------" << endl;
    
    // Get all active elections from the ElectionManager
    // Only check up to 20 possible elections (matching ElectionManager::MAX_ELECTIONS)
    const int MAX_POSSIBLE_ELECTIONS = 20;
    for (int i = 0; i < MAX_POSSIBLE_ELECTIONS; i++) {
        Election* election = electionManager.getElection("E" + to_string(i+1));
        if (election && election->getIsActive()) {
            cout << "\nElection ID: " << election->getElectionId() << endl;
            cout << "Election Name: " << election->getElectionName() << endl;
            cout << "Type: " << election->getType() << endl;
            
            // Display type-specific details
            if (election->getType() == "local") {
                LocalElection* localElection = dynamic_cast<LocalElection*>(election);
                if (localElection) {
                    cout << "Locality: " << localElection->getLocality() << endl;
                }
            } else if (election->getType() == "regional") {
                RegionalElection* regionalElection = dynamic_cast<RegionalElection*>(election);
                if (regionalElection) {
                    cout << "Region: " << regionalElection->getRegion() << endl;
                }
            } else if (election->getType() == "national") {
                NationalElection* nationalElection = dynamic_cast<NationalElection*>(election);
                if (nationalElection) {
                    cout << "Country: " << nationalElection->getCountry() << endl;
                    cout << "Regions: " << nationalElection->getRegions() << endl;
                }            }
            
            cout << "Start Date: " << election->getStartDate() << endl;
            cout << "End Date: " << election->getEndDate() << endl;
            cout << "Status: Active" << endl;
            
            // Display candidates
            cout << "\nCandidates:" << endl;
            cout << "----------" << endl;
            
            for (int j = 0; j < election->getCandidateCount(); j++) {
                Candidate* candidate = election->getCandidate(j);
                if (candidate != nullptr) {
                    cout << "ID: " << (j + 1) << endl;
                    cout << "Name: " << candidate->getName() << endl;
                    cout << "Party: " << candidate->getPartyAffiliation() << endl;
                    cout << string(20, '-') << endl;
                }
            }
            cout << string(30, '-') << endl;
        }
    }
}

bool VotingSystem::castVote(string electionId, string candidateId) {
    // First check if voter has already voted
    if (hasVoted(electionId)) {
        cout << "You have already voted in this election!" << endl;
        return false;
    }    // Get the election and candidate
    Election* election = electionManager.getElection(electionId);
    if (election == nullptr || !election->getIsActive()) {
        cout << "Election not found or not active!" << endl;
        return false;
    }
    
    // Parse candidateId to integer safely without using stoi
    int candIndex = -1;
    bool validInput = true;
    
    for (char c : candidateId) {
        if (c >= '0' && c <= '9') {
            if (candIndex == -1) {
                candIndex = 0;
            }
            candIndex = candIndex * 10 + (c - '0');
        } else {
            validInput = false;
            break;
        }
    }
    
    if (!validInput || candIndex <= 0) {
        cout << "Invalid candidate ID! Please enter a numeric value." << endl;
        return false;
    }
    
    candIndex--; // Adjust for 1-based indexing
    
    if (candIndex < 0 || candIndex >= election->getCandidateCount()) {
        cout << "Invalid candidate ID!" << endl;
        return false;
    }
    
    Candidate* candidate = election->getCandidate(candIndex);
    if (candidate == nullptr) {
        cout << "Candidate not found!" << endl;
        return false;
    }
    
    // Cast the vote
    candidate->incrementVotes();
    
    // Record that this voter has voted
    Voter* voter = dynamic_cast<Voter*>(currentUser);
    if (voter != nullptr) {
        voter->addVotedElection(electionId);
        voter->saveToFile();
    }
    
    // Update the election file with the new vote count
    election->saveToFile();
    
    cout << "Vote cast successfully!" << endl;
    return true;
}

bool VotingSystem::hasVoted(string electionId) {
    if (currentUser != nullptr) {
        Voter* voter = dynamic_cast<Voter*>(currentUser);
        if (voter != nullptr) {
            return voter->hasVotedInElection(electionId);
        }
    }
    return false;
}
