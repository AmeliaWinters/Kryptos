# Required libraries
from nltk.corpus import words
import requests

# Download required NLTK corpus
import nltk
nltk.download('words')

# English words (NLTK corpus)
english_words = words.words()

english_words = [word.lower() for word in english_words]

# Modern Greek words (Using a publicly available dataset via URL)
# Substitute this URL with an appropriate one if needed.
modern_greek_url = 'https://raw.githubusercontent.com/huertatipografica/greekguide/master/greek-dictionary.txt'
modern_greek_words = requests.get(modern_greek_url).text.splitlines()

greek_to_english = {
    'α': 'a', 'ά': 'a', 'β': 'b', 'γ': 'g', 'δ': 'd', 'ε': 'e', 'έ': 'e',
    'ζ': 'z', 'η': 'i', 'ή': 'i', 'θ': 'th', 'ι': 'i', 'ί': 'i', 'ϊ': 'i', 'ΐ': 'i',
    'κ': 'k', 'λ': 'l', 'μ': 'm', 'ν': 'n', 'ξ': 'x', 'ο': 'o', 'ό': 'o',
    'π': 'p', 'ρ': 'r', 'σ': 's', 'ς': 's', 'τ': 't', 'υ': 'y', 'ύ': 'y', 'ϋ': 'y', 'ΰ': 'y',
    'φ': 'f', 'χ': 'ch', 'ψ': 'ps', 'ω': 'o', 'ώ': 'o',
    'Ά': 'A', 'Έ': 'E', 'Ή': 'I', 'Ί': 'I', 'Ό': 'O', 'Ύ': 'Y', 'Ώ': 'O',
    'Α': 'A', 'Β': 'B', 'Γ': 'G', 'Δ': 'D', 'Ε': 'E', 'Ζ': 'Z', 'Η': 'I', 'Θ': 'Th',
    'Ι': 'I', 'Κ': 'K', 'Λ': 'L', 'Μ': 'M', 'Ν': 'N', 'Ξ': 'X', 'Ο': 'O', 'Π': 'P',
    'Ρ': 'R', 'Σ': 'S', 'Τ': 'T', 'Υ': 'Y', 'Φ': 'F', 'Χ': 'Ch', 'Ψ': 'Ps', 'Ω': 'O'
}

def transliterate_greek_to_english(word):
    return ''.join(greek_to_english.get(char, char) for char in word)

modern_greek_words = [transliterate_greek_to_english(word).lower() for word in modern_greek_words]

# Combine all word lists into one


def generate_key(text, key):
    key = ''.join([char for char in key if char.isalpha()])
    key = key * (len(text) // len(key) + 1)
    return key[:len(text)]

def vigenere_decrypt(text, key, alphabet):
    decrypted_text = []
    key = generate_key(text, key)
    for i in range(len(text)):
        if text[i].isalpha():
            idx = (alphabet.index(text[i].lower()) - alphabet.index(key[i].lower()) + len(alphabet)) % len(alphabet)
            decrypted_text.append(alphabet[idx])
        else:
            decrypted_text.append(text[i])
    return ''.join(decrypted_text)

def bruteForceVigenereCustomAlphabet(K4, all_words):
    kryptosAlphabet = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
    kryptosAlphabet = kryptosAlphabet.lower()
    six_char_words = [word for word in all_words if 7 < len(word) < 9]

    i = 0
    for word in all_words:
        decipherText = vigenere_decrypt(K4, word, kryptosAlphabet)

        for sixWord in six_char_words:
            if sixWord in decipherText:
                print(f"cool word in decipher text! key={word} {sixWord} text = {decipherText}")

        i += 1
        if i % 10000 == 0:
            print(f"Checked {i} words")

    collect_total_word_counts(deciphered_texts, four_char_words)

if "berlin" in "coochiecoochieberlincoochie":
    print(f"coochie")

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
all_words = set(english_words + modern_greek_words)
bruteForceVigenereCustomAlphabet(K4, all_words)
                                    
