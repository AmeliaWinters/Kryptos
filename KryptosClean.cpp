#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>

std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

std::string generateCustomAlphabet(const std::string &keyword) {
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::unordered_set<char> seen;
    std::string customAlphabet;

    for (char c : keyword) {
        char upperC = std::toupper(c);
        if (std::isalpha(upperC) && seen.find(upperC) == seen.end()) {
            customAlphabet.push_back(upperC);
            seen.insert(upperC);
        }
    }

    for (char c : alphabet) {
        if (seen.find(c) == seen.end()) {
            customAlphabet.push_back(c);
        }
    }

    return customAlphabet;
}

std::string vigenereDecrypt(const std::string &ciphertext, const std::string &key, const std::string &customAlphabet) {
    std::string decrypted;
    std::string upperKey;
    for (char k : key) {
        upperKey.push_back(std::toupper(k));
    }

    size_t keyLen = upperKey.length();
    size_t alphabetSize = customAlphabet.size();

    for (size_t i = 0; i < ciphertext.size(); ++i) {
        char cipherChar = std::toupper(ciphertext[i]);
        char keyChar = upperKey[i % keyLen];

        if (std::isalpha(cipherChar)) {
            size_t cipherIndex = customAlphabet.find(cipherChar);
            size_t keyIndex = customAlphabet.find(keyChar);

            size_t decryptedIndex = (cipherIndex + alphabetSize - keyIndex) % alphabetSize;
            decrypted.push_back(customAlphabet[decryptedIndex]);
        } else {
            decrypted.push_back(cipherChar);
        }
    }

    return decrypted;
}

int differenceCount(const std::string &str1, const std::string &str2) {
    int diffCount = 0;
    size_t len = std::min(str1.size(), str2.size());

    for (size_t i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) {
            ++diffCount;
        }
    }

    return diffCount + std::abs(static_cast<int>(str1.size()) - static_cast<int>(str2.size()));
}


void getClosestPhrases(std::vector<std::string> &words, const std::string &ciphertext, const std::string &plaintext,
                        const std::string &customAlphabet) {
    int smallestDiffCount = std::numeric_limits<int>::max();

    std::ostringstream resultBuffer;

    int count = 0;
    for (const auto &key : words) {
        ++count;
        std::string decryptedText = vigenereDecrypt(ciphertext, key, customAlphabet);
        int diffCount = differenceCount(decryptedText, plaintext);

        if (diffCount <= smallestDiffCount) {
            smallestDiffCount = diffCount;
            resultBuffer << "Word \"" << key << "\" has thus far the closest phrase \"" << decryptedText
                         << "\" which is " << smallestDiffCount << " characters away from \"" << plaintext << "\"."
                         << std::endl;
                         std::cout << "Word \"" << key << "\" has thus far the closest phrase \"" << decryptedText
                         << "\" which is " << smallestDiffCount << " characters away from \"" << plaintext << "\"."
                         << std::endl;
        }

        if (count % 1000 == 0) {
            std::cout << count << " words checked. Which spells out" << decryptedText << std::endl;
        }
    }

    std::ofstream outputFile("closest_phrase.txt");
    outputFile << resultBuffer.str();

    std::cout << "Finished checking " << count << " words." << std::endl;
}

std::unordered_map<char, int> countCharacters(const std::string &str) {
    std::unordered_map<char, int> charCount;
    for (char c : str) {
        if (std::isalpha(c)) {
            ++charCount[std::tolower(c)];
        }
    }
    return charCount;
}

bool containsAllCharacters(const std::string &decrypted, const std::string &target) {
    auto decryptedCount = countCharacters(decrypted);
    auto targetCount = countCharacters(target);

    for (const auto &pair : targetCount) {
        if (decryptedCount[pair.first] < pair.second) {
            return false;
        }
    }
    return true;
}

void checkKeysForTransposition(const std::string &ciphertext, const std::string &targetPhrase) {
    std::ifstream inputFile("wordsToCheck.txt");
    std::ofstream outputFile("keys_to_transposition.txt");
    std::vector<std::string> words;
    std::string line;

    while (std::getline(inputFile, line)) {
        words.push_back(line);
    }

    std::string keyword = "kryptos";
    std::string customAlphabet = generateCustomAlphabet(keyword);

    for (const auto &key : words) {
        std::string decryptedText = vigenereDecrypt(ciphertext, key, customAlphabet);
        if (containsAllCharacters(decryptedText, targetPhrase)) {
            outputFile << key << std::endl;
        }
    }
}

void 

int main() {
    std::ifstream inputFile("wordsToCheck.txt");
    std::vector<std::string> words;
    std::string line;

    while (std::getline(inputFile, line)) {
        words.push_back(line);
    }

    std::string keyword = "KRYPTOS";
    std::string customAlphabet = generateCustomAlphabet(keyword);

    std::string ciphertextExtract = "FLRVQQPRNGLSS";
    std::string plaintextExtract = "EASTNORTHEAST";

    std::string K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    std::string plaintextPhrases = "EASTNORTHEASTBERLINCLOCK";

    checkKeysForTransposition(K4, plaintextPhrases);
    std::cout << "Finished." << std::endl;
    std::cin.get();
    return 0;
}
