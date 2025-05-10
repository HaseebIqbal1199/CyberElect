// # by Rafaqat
#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string username;
    std::string password;
    std::string name;
    std::string id;
    std::string role; // Added role attribute

public:
    User();
    User(std::string username, std::string password, std::string name, std::string id, std::string role);
    virtual ~User();

    std::string getUsername() const;
    std::string getPassword() const;
    std::string getName() const;
    std::string getID() const;
    std::string getRole() const;

    bool authenticate(std::string inputUsername, std::string inputPassword) const;
    virtual bool login() = 0;
    virtual void saveToFile() = 0;
    virtual void loadFromFile(std::string userId) = 0;
};

#endif // USER_H
