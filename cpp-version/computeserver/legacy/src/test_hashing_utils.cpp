#include "hashing_utils.h"
#include <iostream>

int main() {
    std::string password = "mypassword";
    std::string hashed = computeSHA256Hash(password);

    std::cout << "Original Password: " << password << std::endl;
    std::cout << "SHA256 Hash: " << hashed << std::endl;

    return 0;
}
