// BY REHAN
#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

class Candidate {
private:
    std::string candidateId;
    std::string name;
    std::string partyAffiliation;
    std::string partySymbol;
    int voteCount;

public:
    Candidate();
    Candidate(std::string id, std::string name, std::string party, std::string symbol = "");

    std::string getCandidateId() const;
    std::string getName() const;
    std::string getPartyAffiliation() const;
    std::string getPartySymbol() const;
    int getVoteCount() const;
    
    void setPartySymbol(const std::string& symbol);
    void incrementVotes();
    void displayInfo() const;
    void saveToFile(std::string electionId);
    void loadFromFile(std::string electionId, std::string candidateId);
};

#endif // CANDIDATE_H
