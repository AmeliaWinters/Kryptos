#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <cctype>
#include <limits>
#include <sstream>
#include <queue>

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

    toLowerCase(customAlphabet);

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


void recursive_vigenere_old(std::vector<std::string> &words, const std::string &ciphertext, const std::string &plaintext,
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

std::vector<std::vector<char>> stringToMatrix(const std::string &input, int columns) {
    std::vector<std::vector<char>> matrix;
    std::vector<char> row;

    for (size_t i = 0; i < input.size(); ++i) {
        row.push_back(input[i]);
        if (row.size() == columns || i == input.size() - 1) {
            matrix.push_back(row);
            row.clear();
        }
    }

    return matrix;
}

template <typename T>
void printMatrix(const std::vector<std::vector<T>>& matrix) {
    std::cout << "[";
    for (size_t i = 0; i < matrix.size(); ++i) {
        std::cout << "[";
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j];
            if (j != matrix[i].size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]";
        if (i != matrix.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

std::vector<std::vector<int>> rotateMatrixClockwise(const std::vector<std::vector<int>>& matrix) {
    int num_rows = matrix.size();
    int num_cols = matrix[0].size();
    
    std::vector<std::vector<int>> rotated_matrix(num_cols, std::vector<int>(num_rows));

    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            rotated_matrix[col][num_rows - row - 1] = matrix[row][col];
        }
    }

    return rotated_matrix;
}
std::vector<std::vector<int>> rotateMatrixAnticlockwise(const std::vector<std::vector<int>>& matrix) {
    int num_rows = matrix.size();
    int num_cols = matrix[0].size();
    
    std::vector<std::vector<int>> rotated_matrix(num_cols, std::vector<int>(num_rows));

    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            rotated_matrix[num_cols - col - 1][row] = matrix[row][col];
        }
    }

    return rotated_matrix;
}

std::vector<std::vector<int>> transformMatrix(const std::vector<std::vector<int>>& matrix, int cols) {
    std::vector<int> flat_list;
    for (const auto& row : matrix) {
        flat_list.insert(flat_list.end(), row.begin(), row.end());
    }

    int num_elements = flat_list.size();
    int new_row_count = num_elements / cols + (num_elements % cols != 0 ? 1 : 0);

    std::vector<std::vector<int>> new_matrix(new_row_count);
    for (int i = 0; i < new_row_count; ++i) {
        int row_start = i * cols;
        int row_end = std::min(row_start + cols, num_elements);
        new_matrix[i] = std::vector<int>(flat_list.begin() + row_start, flat_list.begin() + row_end);
    }

    return new_matrix;
}

int calculateDistance(const std::vector<int> &plaintext, const std::vector<int> &decrypted_phrase) {
    int total_distance = 0;
    for (size_t i = 0; i < plaintext.size(); ++i) {
        total_distance += std::abs(plaintext[i] - decrypted_phrase[i]);
    }
    return total_distance;
}

constexpr int char_to_index(char c) {
    return (c - 'A' + 1);
}

constexpr char index_to_char(int index) {
    return 'A' + ((index - 1) % 26); 
}


std::vector<std::vector<int>> transformCharMatrixToInt(const std::vector<std::vector<char>>& char_matrix) {
    std::vector<std::vector<int>> int_matrix;

    for (const auto& row : char_matrix) {
        std::vector<int> int_row;
        for (char c : row) {
            int_row.push_back(char_to_index(c));
        }
        int_matrix.push_back(int_row);
    }

    return int_matrix;
}

std::vector<int> flattenMatrix(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> flat_list;
    for (const auto& row : matrix) {
        flat_list.insert(flat_list.end(), row.begin(), row.end());
    }
    return flat_list;
}

bool containsSequence(const std::vector<int>& flat_list, const std::vector<int>& sequence) {
    if (sequence.empty() || sequence.size() > flat_list.size()) {
        return false;
    }
    
    return std::search(flat_list.begin(), flat_list.end(), sequence.begin(), sequence.end()) != flat_list.end();
}

int findClosestMatchIndex(const std::vector<int>& flat_list, const std::vector<int>& sequence) {
    int flat_size = flat_list.size();
    int seq_size = sequence.size();
    if (flat_size < seq_size) {
        std::cerr << "Error: flat list is smaller than sequence." << std::endl;
        return -1;
    }

    int best_index = -1;
    int smallest_difference = std::numeric_limits<int>::max();

    for (int i = 0; i <= flat_size - seq_size; ++i) {
        int current_difference = 0;

        for (int j = 0; j < seq_size; ++j) {
            current_difference += std::abs(flat_list[i + j] - sequence[j]);
        }

        if (current_difference < smallest_difference) {
            smallest_difference = current_difference;
        }
    }

    return best_index;
}

int findClosestMatch(const std::vector<int> original_flat_list, const std::vector<int> sequence) {
    std::vector<int> flat_list;
    std::copy_if(original_flat_list.begin(), original_flat_list.end(), std::back_inserter(flat_list), 
                 [](int num) { return num >= 0; });

    int flat_size = flat_list.size();
    int seq_size = sequence.size();
    if (flat_size < seq_size) {
        std::cerr << "Error: flat list is smaller than sequence after removing negative values." << std::endl;
        return -1;
    }

    int smallest_difference = std::numeric_limits<int>::max();

    for (int i = 0; i <= flat_size - seq_size; ++i) {
        int current_difference = 0;

        for (int j = 0; j < seq_size; ++j) {
            current_difference += std::abs(flat_list[i + j] - sequence[j]);
        }

        if (current_difference < smallest_difference) {
            smallest_difference = current_difference;
        }
    }

    return smallest_difference;
}

int alphabeticalDistance(const std::string &str1, const std::string &str2) {
    // Check that both strings have the same length
    if (str1.length() != str2.length()) {
        throw std::invalid_argument("Both strings must have the same length.");
    }

    int totalDistance = 0;

    // Loop through both strings and compute the distance between corresponding characters
    for (size_t i = 0; i < str1.length(); ++i) {
        char char1 = toupper(str1[i]);
        char char2 = toupper(str2[i]);

        // Ensure both characters are alphabetical
        if (!isalpha(char1) || !isalpha(char2)) {
            throw std::invalid_argument("Both strings must contain only alphabetical characters.");
        }

        // Calculate the alphabetical index of each character ('A' starts at 0)
        int index1 = char1 - 'A';
        int index2 = char2 - 'A';

        // Calculate the difference between the two characters
        int difference = std::abs(index2 - index1);

        // Sum the difference
        totalDistance += difference;
    }

    return totalDistance;
}

void writeSolution (std::string decryptedText, std::string keyword, std::vector<int> sequence) {
    std::ofstream outputFile("solution.txt");
    outputFile << "Solution Found! ";
    outputFile << "Plaintext: " << decryptedText;
    outputFile << " Keyword: " << keyword;
    outputFile << " Sequence: " ;
    for(int i = 0; i < sequence.size(); i++) {
        outputFile << sequence[i] << " ";
    }
    outputFile << std::endl;
}

void write_best(std::string decryptedText, std::string keyword, int score, std::vector<int> sequence) {
    std::ofstream outputFile("bests transposition.txt");
    outputFile << "Best thus far! ";
    outputFile << "Plaintext: " << decryptedText;
    outputFile << " Keyword: " << keyword;
    outputFile << " Sequence: " ;
    for(int i = 0; i < sequence.size(); i++) {
        outputFile << sequence[i] << " ";
    }
    outputFile << std::endl;
}

std::string flattenMatrixToString(const std::vector<std::vector<int>>& matrix) {
    std::string result;
    for (const auto& row : matrix) {
        for (int val : row) {
            if (val >= 1 && val <= 26) {
                result += static_cast<char>('A' + val - 1);
            }
        }
    }
    return result;
}

void transpose_search_word(std::string decryptedText, std::string word, std::vector<int> sequence, int& thingsChecked, int& bestScore) {
    for(int i = 2; i < 10; i++) {
        std::vector<std::vector<char>> K4Matrix = stringToMatrix(decryptedText, i);
        std::vector<std::vector<int>> K4IntMatrix = transformCharMatrixToInt(K4Matrix);
        std::vector<std::vector<int>> rotatedMatrix = rotateMatrixClockwise(K4IntMatrix);
        
        if(++thingsChecked % 100000 == 0) {
            std::cout << "Checked " << thingsChecked/1000 << "k words." << std::endl;
        }

        int wordScore = findClosestMatch(flattenMatrix(rotatedMatrix), {2, 5, 18, 12, 9, 14});
        if (wordScore <= 2) {
            bestScore = wordScore;
            std::string text = flattenMatrixToString(rotatedMatrix);

            std::cout << "Best score: " << bestScore << " word used: " << word << " text says: " << text << std::endl;
        }

        if(containsSequence(flattenMatrix(rotatedMatrix), {2, 5, 18, 12, 9, 14})) {
            writeSolution(decryptedText, word, sequence);
        } else if (sequence.size() < 2) {
            sequence.push_back(i);
            transpose_search_word(decryptedText, word, sequence, thingsChecked, bestScore);
        }
    }
}

void transpose_search(std::string K4, std::vector<std::string> transpositionWords, std::string alphabet, std::vector<int> sequence = {}) {
    int i = 0;
    int bestScore = 1000;
    for (const auto& word : transpositionWords) {
        std::string decryptedText = vigenereDecrypt(K4, word, alphabet);
        transpose_search_word(decryptedText, word, sequence, i, bestScore);
    }
}

void recursive_vigenere_helper(const std::vector<std::string> &words, const std::string &ciphertext, const std::string &plaintext,
                               const std::string &customAlphabet, int &smallestDiffCount, std::ofstream &outputFile,
                               int &smallestAlphabeticalDistance, std::vector<std::string> keySequence = {}) {
    static long long count = 0;
    

    for (const auto &key : words) {
        ++count;
        std::string decryptedText = vigenereDecrypt(ciphertext, key, customAlphabet);
        int diffCount = differenceCount(decryptedText.substr(21, 13), plaintext);
        int alphabeticalDist = alphabeticalDistance(decryptedText.substr(21, 13), plaintext);
        keySequence.push_back(key);

        if (diffCount < smallestDiffCount) {
            smallestDiffCount = diffCount;
            std::ostringstream resultBuffer;
            resultBuffer << "Depth " << keySequence.size() << " | Keys: ";
            for (const auto &k : keySequence) {
                resultBuffer << k << " ";
            }
            resultBuffer << " Closest phrase \"" << decryptedText.substr(21, 13)
                         << "\" which is " << smallestDiffCount << " characters away from \"" << plaintext << "\"."
                         << std::endl;
            std::cout << resultBuffer.str();
            outputFile << resultBuffer.str();
            outputFile.flush();
        }

        if (alphabeticalDist < smallestAlphabeticalDistance){
            smallestAlphabeticalDistance = alphabeticalDist;
            std::ostringstream resultBuffer;
            resultBuffer << "Depth " << keySequence.size() << " | Keys: ";
            for (const auto &k : keySequence) {
                resultBuffer << k << " ";
            }
            resultBuffer << " Closest phrase \"" << decryptedText.substr(21, 13)
                         << "\" which has " << alphabeticalDist << " alphabetical distance from \"" << plaintext << "\"."
                         << std::endl;
            std::cout << resultBuffer.str();
            outputFile << resultBuffer.str();
            outputFile.flush();
        }

        if (count % 1000000 == 0) {
            if (!keySequence.empty()) {
                std::cout << count / 1000000 << " million words checked. Current key: " << keySequence[0] << std::endl;
            } else {
                std::cout << count / 1000000 << " million words checked. Current key: " << key << std::endl;
            }
        }

        if (diffCount != 0 && keySequence.size() < 2) {
            std::vector<std::string> remainingWords(words.begin(), words.end());
            remainingWords.erase(std::remove(remainingWords.begin(), remainingWords.end(), key), remainingWords.end());
            recursive_vigenere_helper(remainingWords, decryptedText, plaintext, customAlphabet, smallestDiffCount, outputFile, 
                                      smallestAlphabeticalDistance,  keySequence);
        }

        if (diffCount == 0) {
            std::cout << "Perfect match found with keys: ";
            for (const auto &k : keySequence) {
                std::cout << k << " ";
            }
            std::cout << " at depth " << keySequence.size() << std::endl;
            return;
        }

        keySequence.pop_back();
    }
}

void recursive_vigenere(std::vector<std::string> &words, const std::string &ciphertext, const std::string &plaintext,
                        const std::string &customAlphabet) {
    int smallestDiffCount = std::numeric_limits<int>::max();
    std::ofstream outputFile("closest_phrase_recursive.txt", std::ofstream::app);

    std::vector<std::string> keySequence;
    int smallestAlphabeticalDistance = std::numeric_limits<int>::max();
    recursive_vigenere_helper(words, ciphertext, plaintext, customAlphabet, smallestDiffCount, outputFile, smallestAlphabeticalDistance);

    std::cout << "Finished checking " << words.size() << " words." << std::endl;
}


void bfs_vigenere(const std::vector<std::string> &words, const std::string &ciphertext, const std::string &plaintext,
                  const std::string &customAlphabet) {
    int smallestDiffCount = std::numeric_limits<int>::max();
    std::ofstream outputFile("closest_phrase_bfs.txt", std::ofstream::app);
    std::queue<std::pair<std::string, std::vector<std::string>>> queue;

    // Initialize queue with the original ciphertext and empty key sequence
    queue.push({ciphertext, {}});

    int count = 0;
    const size_t maxDepth = 5;  // Maximum depth to limit the BFS levels

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        const std::string &currentText = current.first;
        const std::vector<std::string> &keySequence = current.second;

        for (const auto &key : words) {
            ++count;
            std::string decryptedText = vigenereDecrypt(currentText, key, customAlphabet);
            int diffCount = differenceCount(decryptedText.substr(21, 13), plaintext);
            std::vector<std::string> newKeySequence = keySequence;
            newKeySequence.push_back(key);

            if (diffCount < smallestDiffCount) {
                smallestDiffCount = diffCount;
                std::ostringstream resultBuffer;
                resultBuffer << "Depth " << newKeySequence.size() << " | Keys: ";
                for (const auto &k : newKeySequence) {
                    resultBuffer << k << " ";
                }
                resultBuffer << " Closest phrase \"" << decryptedText.substr(21, 13)
                             << "\" which is " << smallestDiffCount << " characters away from \"" << plaintext << "\"."
                             << std::endl;
                std::cout << resultBuffer.str();
                outputFile << resultBuffer.str();
                outputFile.flush();
            }

            if (count % 1000000 == 0) {
                std::cout << count / 1000000 << " million words checked. Current phrase: " << decryptedText << std::endl;
            }

            if (diffCount == 0) {
                std::cout << "Perfect match found with keys: ";
                for (const auto &k : newKeySequence) {
                    std::cout << k << " ";
                }
                std::cout << " at depth " << newKeySequence.size() << std::endl;
                return;
            }

            if (newKeySequence.size() < maxDepth) {
                queue.push({decryptedText, newKeySequence});
            }
        }
    }

    std::cout << "Finished checking " << count << " words." << std::endl;
}

int scoreDecryptedText(const std::string &decryptedText, const std::string &targetPhrase) {
    return decryptedText.find(targetPhrase) != std::string::npos ? 0 : 100;  // Simplified scoring
}

struct SearchNode {
    std::string text;
    std::vector<std::string> keys;
    int score;
};

auto compare = [](const SearchNode &lhs, const SearchNode &rhs) {
    return lhs.score > rhs.score;
};

void prioritised_search(const std::vector<std::string> &words, const std::string &ciphertext, const std::string &targetPhrase,
                        const std::string &customAlphabet) {
    std::priority_queue<SearchNode, std::vector<SearchNode>, decltype(compare)> queue(compare);
    queue.push({ciphertext, {}, 0});
    const int maxDepth = 5;

    while (!queue.empty()) {
        SearchNode node = queue.top();
        queue.pop();

        if (node.score == 0) {
            std::cout << "Perfect match found with keys: ";
            for (const auto &key : node.keys) std::cout << key << " ";
            std::cout << "\n";
            return;
        }

        if (node.keys.size() < maxDepth) {
            for (const auto &key : words) {
                std::string decryptedText = vigenereDecrypt(node.text, key, customAlphabet);
                std::vector<std::string> newKeys = node.keys;
                newKeys.push_back(key);
                int newScore = scoreDecryptedText(decryptedText, targetPhrase);
                queue.push({decryptedText, newKeys, newScore});
            }
        }
    }
}

int main() {
    std::ifstream allWordsFile("EnglishAndGreekWords.txt");
    std::vector<std::string> allWords;
    std::string line;

    while (std::getline(allWordsFile, line)) {
        allWords.push_back(line);
    }

    std::ifstream transpositionWordsFile("keys_to_transposition.txt");
    std::vector<std::string> transpositionWords;

    while (std::getline(transpositionWordsFile, line)) {
        transpositionWords.push_back(line);
    }

    std::string keyword = "KRYPTOS";
    std::string customAlphabet = generateCustomAlphabet(keyword);

    std::string ciphertextExtract = "FLRVQQPRNGLSS";
    std::string plaintextExtract = "EASTNORTHEAST";

    std::string K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    std::string plaintextPhrases = "EASTNORTHEASTBERLINCLOCK";

    recursive_vigenere(allWords, K4, plaintextExtract, customAlphabet);
   // bfs_vigenere(allWords, K4, plaintextExtract, customAlphabet);

    std::cout << "Finished checking all words." << std::endl;
    std::cin.get();
    return 0;
}
