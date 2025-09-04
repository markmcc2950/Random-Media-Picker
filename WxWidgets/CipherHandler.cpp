#include "CipherHandler.h"

#include <iostream>
#include <string>

int CipherHandler::modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        // If we've found an inverse
        if ((a * x) % m == 1) {
            return x;
        }
    }
    // No inverse found
    return -1;
}

std::string CipherHandler::affineEncode(const std::string& str) {
    std::string encoded;
    for (char c : str) {
        // Only append chars we can print
        if (c >= 32 && c <= 126) {
            int x = c - 32;
            int y = (a * x + b) % m;
            encoded += (char)(y + 32);
        }
        // Otherwise append the char as it is
        else {
            encoded += c;
        }
    }
    return encoded;
}

std::string CipherHandler::affineDecode(const std::string& encStr) {
    int a_inv = modInverse(a, m);

    // No inverse was found, return early
    if (a_inv == -1) {
        std::cerr << "Error: a has no multiplicative inverse modulo m\n";
        return "";
    }

    std::string decoded;
    for (char c : encStr) {
        // Only append chars we can print
        if (c >= 32 && c <= 126) {
            int y = c - 32;
            int x = (a_inv * (y - b + m)) % m;
            decoded += (char)(x + 32);
        }
        // Otherwise append the char as it is
        else {
            decoded += c;
        }
    }
    return decoded;
}