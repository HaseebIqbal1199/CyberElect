#include "../header/Candidate.h"
#include <iostream>
#include <fstream>

using namespace std;

Candidate::Candidate() : voteCount(0) {}

Candidate::Candidate(string id, string n, string party)
    : candidateId(id), name(n), partyAffiliation(party), voteCount(0) {}

string Candidate::getCandidateId() const { return candidateId; }
string Candidate::getName() const { return name; }
string Candidate::getPartyAffiliation() const { return partyAffiliation; }
int Candidate::getVoteCount() const { return voteCount; }

void Candidate::incrementVotes() { voteCount++; }

void Candidate::displayInfo() const {
    cout << "ID: " << candidateId 
         << " | Name: " << name 
         << " | Party: " << partyAffiliation 
         << " | Votes: " << voteCount << endl;
}

void Candidate::saveToFile(string electionId) {
    ofstream file("data/candidates/" + electionId + "_" + candidateId + ".txt");
    if (file.is_open()) {
        file << candidateId << endl;
        file << name << endl;
        file << partyAffiliation << endl;
        file << voteCount << endl;
        file.close();
    }
}

void Candidate::loadFromFile(string electionId, string candId) {
    ifstream file("data/candidates/" + electionId + "_" + candId + ".txt");
    if (file.is_open()) {
        getline(file, candidateId);
        getline(file, name);
        getline(file, partyAffiliation);
        file >> voteCount;
        file.close();
    }
}
