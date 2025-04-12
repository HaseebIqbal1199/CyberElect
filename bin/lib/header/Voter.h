#ifndef VOTER_H
#define VOTER_H

#include "User.h"
#include <string>

class Voter : public User {
private:
    bool hasVoted;
    std::string votedElections[10];
    int votedElectionsCount;

public:
    Voter();
    Voter(std::string username, std::string password, std::string name, std::string id);

    bool login() override;
    void viewElections();
    bool castVote(std::string electionId, std::string candidateId);
    bool checkVoteStatus(std::string electionId);
    void saveToFile() override;
    void loadFromFile(std::string userId) override;
    
    bool hasVotedInElection(std::string electionId);
    void addVotedElection(std::string electionId);
};

#endif // VOTER_H
