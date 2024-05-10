#include "hashing_utils.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

std::string computeSHA256Hash(const std::string& input) {
    CryptoPP::SHA256 hash;
    std::string digest;
    
    CryptoPP::StringSource(input, true,
        new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(digest), false)));

    return digest;
}
