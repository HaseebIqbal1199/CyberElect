#include "../header/User.h"

User::User() {}

User::User(std::string uname, std::string pwd, std::string n, std::string uid)
    : username(uname), password(pwd), name(n), id(uid) {}

User::~User() {}

std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getName() const { return name; }
std::string User::getID() const { return id; }

bool User::authenticate(std::string inputUsername, std::string inputPassword) const {
    return (username == inputUsername && password == inputPassword);
}
