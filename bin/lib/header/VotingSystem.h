// # by Rafaqat, Haseeb, Rehan
#pragma once
#include "User.h"
#include "Voter.h"
#include "Administrator.h"
#include "ElectionManager.h"
#include <string>

class VotingSystem {
private:
    User* currentUser;
    ElectionManager electionManager;
    
public:
    VotingSystem();
    ~VotingSystem();
    
    bool registerVoter(std::string username, std::string password, std::string name, 
                       std::string cnic = "", std::string dob = "", std::string city = "");
    bool registerAdmin(std::string username, std::string password, std::string name);
    bool loginUser(std::string username, std::string password, std::string role);
    void logout();
    
    bool createElection(std::string type, std::string name, std::string start, std::string end, std::string region, int regions);    bool startElection(std::string electionId);
    bool endElection(std::string electionId);
    bool addCandidate(std::string electionId, std::string name, std::string partyAffiliation, std::string partySymbol = "");
    void viewResults(std::string electionId);
    void viewAllElections();
    void viewElections();    void displayRunningElections();
    
    // Helper function to display election information
    void displayElectionInfo(const std::string& electionId);
    void displayElectionSummary(const std::string& electionId);    void displayCandidateSummary(const std::string& electionId, int candidateIndex);
    void displayLiveCountdown(const std::string& electionId, int seconds = 10); // Live countdown timer
      // New menu-based selection helpers using arrays instead of vectors
    void getRunningElectionIds(std::string* electionIds, int maxCount, int& count);
    void getAllElectionIds(std::string* electionIds, int maxCount, int& count);
    void getCandidateIndices(const std::string& electionId, int* indices, int maxCount, int& count);
    
    bool castVote(std::string electionId, std::string candidateId);
    bool hasVoted(std::string electionId);
};
