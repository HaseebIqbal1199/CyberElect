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
    
    // Override getType to identify this election type
    std::string getType() const override { return "local"; }
    
    // Type-specific data methods
    std::string getLocality() const { return locality; }
    void setLocality(const std::string& loc) { locality = loc; }
};

#endif // LOCAL_ELECTION_H
