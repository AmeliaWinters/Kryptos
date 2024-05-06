import nltk
nltk.download('words')
from nltk.corpus import words
from concurrent.futures import ThreadPoolExecutor, as_completed
import time

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

# Prepare the word list
wordList = words.words()
wordList.append("kryptos")
wordList = [word.lower() for word in wordList if 4 <= len(word) <= 10]
alphabet = "abcdefghijklmnopqrstuvwxyz"

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

def get_custom_alphabet(word):
    remaining_alphabet = ''.join([char for char in alphabet if char not in word])
    result = word + remaining_alphabet
    return result

def attempt_decrypt(K4, word, full_word_list, recurse=False, prevWord=None):
    decipher_text = vigenere_decrypt(K4, word, get_custom_alphabet("kryptos"))
    if decipher_text[21:34].lower() == "eastnortheast":
        print(f"Solution found with word: {word} and prevWord: {prevWord}")
        return f"Solution found with word: {word} and prevWord: {prevWord}"
    elif recurse:
        for next_word in full_word_list:
            result = attempt_decrypt(decipher_text, next_word, full_word_list, False, word)
            if result:
                return result
    return None

def process_segment(segment, full_word_list):
    i = 0
    for word in segment:
        result = attempt_decrypt(K4, word, full_word_list, recurse=True)
        if result:
            return result
        else:
            i += 1
            print(f"Word {i} - '{word}' is not the solution. ")
    return None

def brute_force_vigenere_parallel(K4):
    # Splitting the word list into 10 equal segments
    segment_size = len(wordList) // 10
    segments = [wordList[i:i + segment_size] for i in range(0, len(wordList), segment_size)]
    
    with ThreadPoolExecutor(max_workers=10) as executor:
        # Map the process_segment function over all segments
        future_to_segment = {executor.submit(process_segment, segment, wordList): segment for segment in segments}
        for future in as_completed(future_to_segment):
            result = future.result()
            if result:
                print(result)
                break

startTime = time.time()
brute_force_vigenere_parallel(K4)
print(f"Time elapsed: {time.time() - startTime} seconds")
