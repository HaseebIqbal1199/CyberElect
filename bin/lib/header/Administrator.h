// # by Rafaqat
#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"
#include <string>

class Administrator : public User {
public:
    Administrator();
    Administrator(std::string username, std::string password, std::string name, std::string id);

    bool login() override;
    void createElection(std::string type);
    void addCandidate(std::string electionId, std::string candidateName, std::string partyAffiliation);
    void viewResults(std::string electionId);
    void viewAllElections();
    void saveToFile() override;
    void loadFromFile(std::string userId) override;
};

#endif // ADMINISTRATOR_H
