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
    
    // Override getType to identify this election type
    std::string getType() const override { return "regional"; }
    
    // Type-specific data methods
    std::string getRegion() const { return region; }
    void setRegion(const std::string& reg) { region = reg; }
};

#endif // REGIONAL_ELECTION_H
