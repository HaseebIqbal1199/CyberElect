#include "../header/VotingSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <ctime>

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

    if (role == "voter") {
        string voterPath = "data/voters/";
        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile((voterPath + "*").c_str(), &findData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    Voter* voter = new Voter();
                    voter->loadFromFile(findData.cFileName);
                    if (voter->authenticate(username, password)) {
                        currentUser = voter;
                        return true;
                    }
                    delete voter;
                }
            } while (FindNextFile(hFind, &findData));
            FindClose(hFind);
        }
    }
    else if (role == "admin") {
        string adminPath = "data/admins/";
        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile((adminPath + "*").c_str(), &findData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    Administrator* admin = new Administrator();
                    admin->loadFromFile(findData.cFileName);
                    if (admin->authenticate(username, password)) {
                        currentUser = admin;
                        return true;
                    }
                    delete admin;
                }
            } while (FindNextFile(hFind, &findData));
            FindClose(hFind);
        }
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
    stringstream ss;
    ss << "E" << time(nullptr);
    string id = ss.str();
    
    ofstream file("data/elections/" + id + ".txt");
    if (file.is_open()) {
        file << type << endl;
        file << name << endl;
        file << start << endl;
        file << end << endl;
        file << region << endl;
        file << regions << endl;
        file << "Inactive" << endl;  // Initial status
        file << "0" << endl;  // Number of candidates
        file.close();
        return true;
    }
    return false;
}

void VotingSystem::displayElectionInfo(const string& electionId) {
    string filename = "data/elections/" + electionId + ".txt";
    ifstream file(filename);
    
    if (file.is_open()) {
        string type, name, start, end, region, status;
        getline(file, type);
        getline(file, name);
        getline(file, start);
        getline(file, end);
        getline(file, region);
        
        // Skip regions count
        string temp;
        getline(file, temp);
        
        getline(file, status);
        
        cout << "\nElection Details:" << endl;
        cout << "---------------" << endl;
        cout << "ID: " << electionId << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        if (!region.empty()) {
            cout << "Region: " << region << endl;
        }
        cout << "Start Date: " << start << endl;
        cout << "End Date: " << end << endl;
        cout << "Status: " << status << endl;
        
        // Read number of candidates
        string candidateCountStr;
        getline(file, candidateCountStr);
        int candidateCount = stoi(candidateCountStr);
        
        if (candidateCount > 0) {
            cout << "\nCandidates:" << endl;
            cout << "----------" << endl;
            
            for (int i = 0; i < candidateCount; i++) {
                string candidateName, partyAffiliation, votes;
                getline(file, candidateName);
                getline(file, partyAffiliation);
                getline(file, votes);
                
                cout << "ID: " << (i + 1) << endl;
                cout << "Name: " << candidateName << endl;
                cout << "Party: " << partyAffiliation << endl;
                cout << string(20, '-') << endl;
            }
        }
        file.close();
    }
}

bool VotingSystem::addCandidate(string electionId, string name, string partyAffiliation) {
    // First display election info
    cout << "Adding candidate to election:" << endl;
    displayElectionInfo(electionId);
    
    string filename = "data/elections/" + electionId + ".txt";
    string tempFile = "data/elections/temp.txt";
    
    ifstream inFile(filename);
    ofstream outFile(tempFile);
    
    if (inFile.is_open() && outFile.is_open()) {
        string line;
        int lineCount = 0;
        int candidateCount = 0;
        
        while (getline(inFile, line)) {
            if (lineCount == 7) {  // Number of candidates line
                candidateCount = stoi(line);
                outFile << (candidateCount + 1) << endl;
            } else {
                outFile << line << endl;
            }
            lineCount++;
        }
        
        // Add new candidate at the end
        outFile << name << endl;
        outFile << partyAffiliation << endl;
        outFile << "0" << endl;  // Initial votes
        
        inFile.close();
        outFile.close();
        
        remove(filename.c_str());
        rename(tempFile.c_str(), filename.c_str());
        
        cout << "\nCandidate added successfully!" << endl;
        cout << "New candidate details:" << endl;
        cout << "ID: " << (candidateCount + 1) << endl;
        cout << "Name: " << name << endl;
        cout << "Party: " << partyAffiliation << endl;
        
        return true;
    }
    return false;
}

bool VotingSystem::startElection(string electionId) {
    // First display election info
    cout << "Starting election:" << endl;
    displayElectionInfo(electionId);
    
    string filename = "data/elections/" + electionId + ".txt";
    string tempFile = "data/elections/temp.txt";
    
    ifstream inFile(filename);
    ofstream outFile(tempFile);
    
    if (inFile.is_open() && outFile.is_open()) {
        string line;
        int lineCount = 0;
        bool success = false;
        
        while (getline(inFile, line)) {
            if (lineCount == 6) {  // Status line
                if (line == "Inactive") {
                    outFile << "Active" << endl;
                    success = true;
                } else {
                    outFile << line << endl;
                }
            } else {
                outFile << line << endl;
            }
            lineCount++;
        }
        
        inFile.close();
        outFile.close();
        
        if (success) {
            remove(filename.c_str());
            rename(tempFile.c_str(), filename.c_str());
            cout << "\nElection started successfully!" << endl;
            return true;
        } else {
            remove(tempFile.c_str());
            cout << "\nError: Election is not in Inactive state!" << endl;
            return false;
        }
    }
    return false;
}

bool VotingSystem::endElection(string electionId) {
    // First display election info
    cout << "Ending election:" << endl;
    displayElectionInfo(electionId);
    
    string filename = "data/elections/" + electionId + ".txt";
    string tempFile = "data/elections/temp.txt";
    
    ifstream inFile(filename);
    ofstream outFile(tempFile);
    
    if (inFile.is_open() && outFile.is_open()) {
        string line;
        int lineCount = 0;
        bool success = false;
        
        while (getline(inFile, line)) {
            if (lineCount == 6) {  // Status line
                if (line == "Active") {
                    outFile << "Ended" << endl;
                    success = true;
                } else {
                    outFile << line << endl;
                }
            } else {
                outFile << line << endl;
            }
            lineCount++;
        }
        
        inFile.close();
        outFile.close();
        
        if (success) {
            remove(filename.c_str());
            rename(tempFile.c_str(), filename.c_str());
            cout << "\nElection ended successfully!" << endl;
            return true;
        } else {
            remove(tempFile.c_str());
            cout << "\nError: Election is not in Active state!" << endl;
            return false;
        }
    }
    return false;
}

void VotingSystem::viewResults(string electionId) {
    string filename = "data/elections/" + electionId + ".txt";
    ifstream file(filename);
    
    if (file.is_open()) {
        string type, name, start, end, region, status;
        getline(file, type);
        getline(file, name);
        getline(file, start);
        getline(file, end);
        getline(file, region);
        int regions;
        file >> regions;
        file.ignore();
        getline(file, status);
        int candidateCount;
        file >> candidateCount;
        file.ignore();
        
        cout << "\nElection Results:" << endl;
        cout << "----------------" << endl;
        cout << "Election: " << name << endl;
        cout << "Type: " << type << endl;
        if (!region.empty()) {
            cout << "Region: " << region << endl;
        }
        cout << "Status: " << status << endl;
        
        if (status != "Ended") {
            cout << "\nResults are only available after the election has ended." << endl;
            file.close();
            return;
        }
        
        cout << "\nCandidates and Votes:" << endl;
        cout << "-------------------" << endl;
        
        for (int i = 0; i < candidateCount; i++) {
            string candidateName, partyAffiliation, votes;
            getline(file, candidateName);
            getline(file, partyAffiliation);
            getline(file, votes);
            
            cout << "\nCandidate " << (i + 1) << ":" << endl;
            cout << "Name: " << candidateName << endl;
            cout << "Party: " << partyAffiliation << endl;
            cout << "Votes: " << votes << endl;
            cout << string(20, '-') << endl;
        }
        
        file.close();
    } else {
        cout << "Election not found!" << endl;
    }
}

void VotingSystem::viewAllElections() {
    cout << "\nAll Elections:" << endl;
    cout << "-------------" << endl;
    
    string electionPath = "data/elections/";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((electionPath + "*").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
                string(findData.cFileName) != "temp.txt") {
                    
                ifstream file(electionPath + findData.cFileName);
                if (file.is_open()) {
                    string type, name, status;
                    getline(file, type);
                    getline(file, name);
                    
                    // Skip to status line
                    for (int i = 0; i < 4; i++) {
                        string temp;
                        getline(file, temp);
                    }
                    getline(file, status);
                    
                    string electionId = findData.cFileName;
                    electionId = electionId.substr(0, electionId.find(".txt"));
                    
                    cout << "ID: " << electionId << " | Name: " << name << " | Status: " << status << endl;
                    file.close();
                }
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
}

void VotingSystem::viewElections() {
    cout << "\nActive Elections:" << endl;
    cout << "----------------" << endl;
    
    string electionPath = "data/elections/";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((electionPath + "*").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
                string(findData.cFileName) != "temp.txt") {
                    
                ifstream file(electionPath + findData.cFileName);
                if (file.is_open()) {
                    string type, name, status;
                    getline(file, type);
                    getline(file, name);
                    
                    // Skip to status line
                    for (int i = 0; i < 4; i++) {
                        string temp;
                        getline(file, temp);
                    }
                    getline(file, status);
                    
                    if (status == "Active") {
                        string electionId = findData.cFileName;
                        electionId = electionId.substr(0, electionId.find(".txt"));
                        cout << "ID: " << electionId << " | Name: " << name << endl;
                    }
                    file.close();
                }
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
}

void VotingSystem::displayRunningElections() {
    cout << "\nRunning Elections Status:" << endl;
    cout << "------------------------" << endl;
    
    string electionPath = "data/elections/";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((electionPath + "*").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        bool found = false;
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
                string(findData.cFileName) != "temp.txt") {
                    
                ifstream file(electionPath + findData.cFileName);
                if (file.is_open()) {
                    string type, name, start, end, region, status;
                    string electionId = findData.cFileName;
                    electionId = electionId.substr(0, electionId.find("."));  // Remove .txt
                    
                    getline(file, type);
                    getline(file, name);
                    getline(file, start);
                    getline(file, end);
                    getline(file, region);
                    
                    // Skip regions count
                    string temp;
                    getline(file, temp);
                    
                    getline(file, status);
                    
                    if (status == "Active") {
                        found = true;
                        cout << "\nElection ID: " << electionId << endl;
                        cout << "Election Name: " << name << endl;
                        cout << "Type: " << type << endl;
                        if (!region.empty()) {
                            cout << "Region: " << region << endl;
                        }
                        cout << "Start Date: " << start << endl;
                        cout << "End Date: " << end << endl;
                        cout << "Status: " << status << endl;
                        
                        // Read number of candidates
                        string candidateCountStr;
                        getline(file, candidateCountStr);
                        int candidateCount = stoi(candidateCountStr);
                        
                        cout << "\nCandidates:" << endl;
                        cout << "----------" << endl;
                        
                        for (int i = 0; i < candidateCount; i++) {
                            string candidateName, partyAffiliation, votes;
                            getline(file, candidateName);
                            getline(file, partyAffiliation);
                            getline(file, votes);
                            
                            cout << "ID: " << (i + 1) << endl;
                            cout << "Name: " << candidateName << endl;
                            cout << "Party: " << partyAffiliation << endl;
                            cout << string(20, '-') << endl;
                        }
                        cout << string(30, '-') << endl;
                    }
                    file.close();
                }
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
        
        if (!found) {
            cout << "No elections are currently running." << endl;
        }
    }
}

bool VotingSystem::castVote(string electionId, string candidateId) {
    string filename = "data/elections/" + electionId + ".txt";
    string tempFile = "data/elections/temp.txt";
    
    ifstream inFile(filename);
    ofstream outFile(tempFile);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        return false;
    }
    
    // First check if voter has already voted
    if (hasVoted(electionId)) {
        cout << "You have already voted in this election!" << endl;
        return false;
    }
    
    string line;
    int lineCount = 0;
    int candidateCount = 0;
    int targetCandidate = stoi(candidateId);
    bool voteRecorded = false;
    
    // Copy election data and update candidate votes
    while (getline(inFile, line)) {
        if (lineCount < 8) {
            outFile << line << endl;
            if (lineCount == 7) {
                candidateCount = stoi(line);
            }
        } else {
            // We're in the candidates section
            int currentCandidate = (lineCount - 8) / 3 + 1;
            
            if (currentCandidate == targetCandidate) {
                // This is the candidate being voted for
                outFile << line << endl;  // Name
                getline(inFile, line);
                outFile << line << endl;  // Party
                getline(inFile, line);
                int votes = stoi(line);
                outFile << (votes + 1) << endl;  // Increment votes
                voteRecorded = true;
                lineCount += 2;
            } else {
                // Copy other candidates' data as is
                outFile << line << endl;
            }
        }
        lineCount++;
    }
    
    inFile.close();
    outFile.close();
    
    if (voteRecorded) {
        // Record that this voter has voted
        string voterFile = "data/voters/" + dynamic_cast<Voter*>(currentUser)->getID() + ".txt";
        ofstream voteRecord(voterFile, ios::app);
        if (voteRecord.is_open()) {
            voteRecord << electionId << endl;
            voteRecord.close();
        }
        
        remove(filename.c_str());
        rename(tempFile.c_str(), filename.c_str());
        cout << "Vote cast successfully!" << endl;
        return true;
    }
    
    remove(tempFile.c_str());
    cout << "Invalid candidate ID!" << endl;
    return false;
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
