#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string username;
    std::string password;
    std::string name;
    std::string id;

public:
    User();
    User(std::string username, std::string password, std::string name, std::string id);
    virtual ~User();

    std::string getUsername() const;
    std::string getPassword() const;
    std::string getName() const;
    std::string getID() const;

    bool authenticate(std::string inputUsername, std::string inputPassword) const;
    virtual bool login() = 0;
    virtual void saveToFile() = 0;
    virtual void loadFromFile(std::string userId) = 0;
};

#endif // USER_H
