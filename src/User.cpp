#include "User.hpp"
#include "Table.hpp"

User::User(int UID, std::string Name, std::string Pass): userID(UID)
{
    name = Name;
    
}

User::~User()
{
}