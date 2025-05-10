#ifndef ELECTION_MANAGER_H
#define ELECTION_MANAGER_H

#include "Election.h"
#include "LocalElection.h"
#include "NationalElection.h"
#include "RegionalElection.h"
#include <string>
#include <vector>

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
    bool electionExists(std::string electionId);
    
    // Methods for accessing elections
    std::vector<std::string> getRunningElectionIds();
    std::vector<std::string> getAllElectionIds();
};

#endif // ELECTION_MANAGER_H
