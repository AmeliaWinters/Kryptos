#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include <unordered_set>

// Function to generate the repeating key
std::string generateKey(const std::string &text, const std::vector<int> &key) {
    std::string key_str;
    for (int k : key) {
        key_str += 'A' + (k - 1); // Convert numeric value back to character
    }
    while (key_str.length() < text.length()) {
        key_str += key_str;
    }
    return key_str.substr(0, text.length());
}


std::string getCustomAlphabet(const std::string& word, const std::string& alphabet) {
    // Create a set of characters in the input word
    std::unordered_set<char> used_chars(word.begin(), word.end());
    std::string remaining_alphabet;

    // Add only characters from alphabet that are not in the word set
    for (char c : alphabet) {
        if (used_chars.find(c) == used_chars.end()) {
            remaining_alphabet += c;
        }
    }

    // Concatenate the input word and the remaining alphabet
    return word + remaining_alphabet;
}

// Vigenere decrypt function using numerical values (A = 1, B = 2, etc.)
std::vector<int> vigenereDecrypt(const std::vector<int> &text, const std::vector<int> &key, int alphabet_length) {
    std::vector<int> decrypted_text;
    for (size_t i = 0; i < text.size(); ++i) {
        int idx = (text[i] - key[i % key.size()] + alphabet_length) % alphabet_length;
        if (idx == 0) idx = alphabet_length; // Wrap around if index is zero
        decrypted_text.push_back(idx);
    }
    return decrypted_text;
}

int calculateDistance(const std::vector<int> &plaintext, const std::vector<int> &decrypted_phrase) {
    int total_distance = 0;
    for (size_t i = 0; i < plaintext.size(); ++i) {
        total_distance += std::abs(plaintext[i] - decrypted_phrase[i]);
    }
    return total_distance;
}

constexpr int char_to_index(char c) {
    return (c - 'a' + 1); // A = 1, B = 2, ..., Z = 26
}

constexpr char index_to_char(int index) {
    return 'a' + ((index - 1) % 26); // Adjusting to 1-based index
}

// Brute force Vigenere decryption
void bruteForceVigenere(const std::vector<int> &keyphrase, const std::vector<int> &plaintext, const std::string &alphabet) {
    int alphabet_length = alphabet.size();
    std::ofstream outfile("K4Solution.txt");
    std::ofstream bestsFile("bests.txt");

    int i = 0;
    
    int best_distance = 1000000;
    for (int length = 4; length <= 10; ++length) {
        std::vector<int> key(length, 1);
        
        while (true) {
            std::vector<int> decrypted_phrase = vigenereDecrypt(keyphrase, key, alphabet_length);

            int total_distance = calculateDistance(plaintext, decrypted_phrase);

            // Check if decryption matches plaintext
            if (decrypted_phrase == plaintext) {
                std::cout << "Solution found! Key: ";
                outfile << "Solution found! Key: ";

                for (int k : key) {
                    std::cout << k << ' ';
                     outfile << k << ' ';
                }

                std::cout << std::endl;
                outfile << std::endl;
                outfile.close();
                return;
            }

            if(best_distance > total_distance){
                best_distance = total_distance;
                std::string stringKey(length, 'A'); // Initialize the string with the correct length
                for (size_t i = 0; i < key.size(); ++i) {
                    stringKey[i] = index_to_char(key[i]);
                }
                bestsFile << "New best key found: " << stringKey << " with a distance of " << best_distance << " Which decrypts to:";

                for (int k : decrypted_phrase) {
                    bestsFile << index_to_char(k);
                }
                bestsFile << std::endl;
            }
            
            if(i % 1000000 == 0 && i != 0) {
                std::cout << "Words checked: " << i/1000000 <<  "m" <<std::endl;
            }

            // Increment the key
            int pos = length - 1;
            while (pos >= 0 && key[pos] == 26) {
                key[pos] = 1;
                --pos;
            }
            if (pos < 0) break;
            ++key[pos];
            ++i;
        }
         std::cout << "Checked all lengths " << length <<std::endl;
    }
    bestsFile.close();
}

int main() {
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::string K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    std::string keyphrase = "flrvqqprngkss";
    std::string plain_text_phrase = "eastnortheast";
    
    std::vector<int> numeric_keyphrase(keyphrase.size());
    std::vector<int> numeric_plain_text_phrase(plain_text_phrase.size());
    for (size_t i = 0; i < plain_text_phrase.size(); ++i) {
        numeric_plain_text_phrase[i] = char_to_index(plain_text_phrase[i]);
        numeric_keyphrase[i] = char_to_index(keyphrase[i]);
    }

    

    bruteForceVigenere(numeric_keyphrase, numeric_plain_text_phrase, getCustomAlphabet("kryptos", alphabet));

    std::cin.get();
    return 0;
}
