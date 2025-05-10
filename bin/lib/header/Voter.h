// BY REHAN
#ifndef VOTER_H
#define VOTER_H

#include "User.h"
#include <string>

class Voter : public User {
private:
    bool hasVoted;
    std::string votedElections[10];
    int votedElectionsCount;
    std::string cnic;         // Added CNIC field
    std::string dateOfBirth;  // Added date of birth field
    std::string city;         // Added city information

public:
    Voter();
    Voter(std::string username, std::string password, std::string name, std::string id, 
          std::string cnic = "", std::string dob = "", std::string city = "");

    bool login() override;
    void viewElections();
    bool castVote(std::string electionId, std::string candidateId);
    bool checkVoteStatus(std::string electionId);
    void saveToFile() override;
    void loadFromFile(std::string userId) override;
    
    // Getters for new fields
    std::string getCNIC() const { return cnic; }
    std::string getDateOfBirth() const { return dateOfBirth; }
    std::string getCity() const { return city; }
      // Setters for new fields
    void setCNIC(const std::string& newCNIC) { cnic = newCNIC; }
    void setDateOfBirth(const std::string& newDOB) { dateOfBirth = newDOB; }
    void setCity(const std::string& newCity) { city = newCity; }
    
    bool hasVotedInElection(std::string electionId);
    void addVotedElection(std::string electionId);
};

#endif // VOTER_H
