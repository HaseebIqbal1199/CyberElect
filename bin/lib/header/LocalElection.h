#ifndef LOCAL_ELECTION_H
#define LOCAL_ELECTION_H

#include "Election.h"
#include <string>

class LocalElection : public Election {
private:
    std::string locality;

public:
    LocalElection();
    LocalElection(std::string id, std::string name, std::string start, std::string end, std::string locality);

    void calculateResults() override;
    void displayResults() override;
    void saveToFile() override;
    void loadFromFile(std::string electionId) override;
};

#endif // LOCAL_ELECTION_H
