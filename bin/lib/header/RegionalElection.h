#ifndef REGIONAL_ELECTION_H
#define REGIONAL_ELECTION_H

#include "Election.h"
#include <string>

class RegionalElection : public Election {
private:
    std::string region;

public:
    RegionalElection();
    RegionalElection(std::string id, std::string name, std::string start, std::string end, std::string region);

    void calculateResults() override;
    void displayResults() override;
    void saveToFile() override;
    void loadFromFile(std::string electionId) override;
};

#endif // REGIONAL_ELECTION_H
