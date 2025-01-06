#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

std::string calculateHash(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return "Error: File not found.";
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }
    SHA256_Update(&sha256, buffer, file.gcount());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    std::ostringstream hashString;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hashString << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return hashString.str();
}

void verifyFileIntegrity(const std::string &originalHash, const std::string &filePath) {
    std::string currentHash = calculateHash(filePath);
    if (currentHash == "Error: File not found.") {
        std::cout << currentHash << std::endl;
    } else if (currentHash == originalHash) {
        std::cout << "File integrity verified. No changes detected." << std::endl;
    } else {
        std::cout << "File integrity compromised. Hashes do not match!" << std::endl;
    }
}

int main() {
    std::string filePath, originalHash;
    int choice;

    std::cout << "Enter the file path: ";
    std::cin >> filePath;
    std::cout << "Do you want to (1) Generate a hash or (2) Verify integrity? Enter 1 or 2: ";
    std::cin >> choice;

    if (choice == 1) {
        std::string fileHash = calculateHash(filePath);
        std::cout << "SHA-256 Hash: " << fileHash << std::endl;
    } else if (choice == 2) {
        std::cout << "Enter the original hash: ";
        std::cin >> originalHash;
        verifyFileIntegrity(originalHash, filePath);
    } else {
        std::cout << "Invalid option selected." << std::endl;
    }

}
