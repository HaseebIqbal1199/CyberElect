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
    
    // Override getType to identify this election type
    std::string getType() const override { return "national"; }
    
    // Type-specific data methods
    std::string getCountry() const { return country; }
    int getRegions() const { return regions; }
    void setCountry(const std::string& c) { country = c; }
    void setRegions(int r) { regions = r; }
};

#endif // NATIONAL_ELECTION_H
