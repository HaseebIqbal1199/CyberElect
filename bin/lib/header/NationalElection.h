#ifndef NATIONAL_ELECTION_H
#define NATIONAL_ELECTION_H

#include "Election.h"
#include <string>

class NationalElection : public Election {
private:
    std::string country;
    int regions;

public:
    NationalElection();
    NationalElection(std::string id, std::string name, std::string start, std::string end, std::string country, int regions);

    void calculateResults() override;
    void displayResults() override;
    void saveToFile() override;
    void loadFromFile(std::string electionId) override;
};

#endif // NATIONAL_ELECTION_H
