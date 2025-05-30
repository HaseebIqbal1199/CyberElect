// BY REHAN
#include "../header/Candidate.h"
#include <iostream>
#include <fstream>

using namespace std;

Candidate::Candidate() : voteCount(0) {}

Candidate::Candidate(string id, string n, string party, string symbol)
    : candidateId(id), name(n), partyAffiliation(party), partySymbol(symbol), voteCount(0) {}

string Candidate::getCandidateId() const { return candidateId; }
string Candidate::getName() const { return name; }
string Candidate::getPartyAffiliation() const { return partyAffiliation; }
string Candidate::getPartySymbol() const { return partySymbol; }
int Candidate::getVoteCount() const { return voteCount; }

void Candidate::setPartySymbol(const std::string& symbol) { partySymbol = symbol; }

void Candidate::incrementVotes() { voteCount++; }

void Candidate::displayInfo() const {
    cout << "ID: " << candidateId 
         << " | Name: " << name 
         << " | Party: " << partyAffiliation 
         << " | Votes: " << voteCount << endl;
}

// These methods are now deprecated as candidate data is stored in the unified elections file
void Candidate::saveToFile(string electionId) {
    // Method removed - candidate data is now stored in the unified elections file
}

void Candidate::loadFromFile(string electionId, string candId) {
    // Method removed - candidate data is now loaded from the unified elections file
}
