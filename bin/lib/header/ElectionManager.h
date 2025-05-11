#ifndef ELECTION_MANAGER_H
#define ELECTION_MANAGER_H

#include "Election.h"
#include "LocalElection.h"
#include "NationalElection.h"
#include "RegionalElection.h"
#include <string>

class ElectionManager {
private:
    static const int MAX_ELECTIONS = 20;
    Election* elections[MAX_ELECTIONS];
    int electionCount;
    
    // Helper method to update an election in the unified file
    void updateElectionFile(Election* election);

public:
    ElectionManager();
    ~ElectionManager();    bool createElection(std::string type, std::string name, std::string start, std::string end, std::string region = "", int regions = 0);
    void displayActiveElections();
    void displayAllElections();
    Election* getElection(std::string electionId);
    Election* getElectionById(const std::string& electionId); // Alias for getElection
    bool addCandidateToElection(std::string electionId, std::string name, std::string party, std::string symbol = "");
    void viewElectionResults(std::string electionId);
    void loadAllElections();
    bool electionExists(std::string electionId);    bool updateElection(Election* election); // Public method to update an election    bool setElectionStatus(std::string electionId, bool isActive); // Method to update election status
    
    // Methods for accessing elections using arrays
    void getRunningElectionIds(std::string* electionIds, int maxCount, int& count);
    void getAllElectionIds(std::string* electionIds, int maxCount, int& count);
};

#endif // ELECTION_MANAGER_H
