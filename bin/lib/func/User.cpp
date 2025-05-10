// # by Rafaqat
#include "../header/User.h"

User::User() {}

User::User(std::string uname, std::string pwd, std::string n, std::string uid, std::string r)
    : username(uname), password(pwd), name(n), id(uid), role(r) {}

User::~User() {}

std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getName() const { return name; }
std::string User::getID() const { return id; }
std::string User::getRole() const { return role; }

bool User::authenticate(std::string inputUsername, std::string inputPassword) const {
    return (username == inputUsername && password == inputPassword);
}
