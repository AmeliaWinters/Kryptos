import nltk
nltk.download('words')
from nltk.corpus import words
from concurrent.futures import ThreadPoolExecutor, as_completed
import math

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

allWordList = words.words()
allWordList.insert(0, "kryptos")
allWordList = [word.lower() for word in allWordList if 4 <= len(word) <= 10]
alphabet = "abcdefghijklmnopqrstuvwxyz"

maxWorkers = 10

globalBestDistance = math.inf

def makeFiles(wordList, filesToMake):
    segment_size = len(allWordList) // maxWorkers
    segments = [allWordList[i:i + segment_size] for i in range(0, len(allWordList), segment_size)]
    i = 0
    for segment in segments:
        with open(f"wordsToCheck{i}.txt", 'w') as file:
            for word in segment:
                file.write(word + '\n')
        i+=1

def getSegmentsFromFiles():
    remainingSegments = [[] for _ in range(maxWorkers)]
    i = 0
    for i in range(maxWorkers):
        with open(f"wordsToCheck{i}.txt", 'r') as file:
            for line in file:
                remainingSegments[i].append(line.strip())
    return remainingSegments


def removeWordFromFile(wordToRemove, i):
    tempFilePath = f"wordsToCheck{i}.txt.tmp"
    found = False

    try:
        with open(f"wordsToCheck{i}.txt", 'r') as file, open(tempFilePath, 'w') as temp_file:
            for line in file:
                stripped_line = line.strip()
                if stripped_line == wordToRemove and not found:
                    found = True 
                    continue  
                temp_file.write(stripped_line + '\n')

        if found:
            import os
            os.replace(tempFilePath, f"wordsToCheck{i}.txt")
    except Exception as e:
        print(f"An error occurred: {e}")


def generateKey(text, key):
    key = ''.join([char for char in key if char.isalpha()])
    key = key * (len(text) // len(key) + 1)
    return key[:len(text)]

def vigenereDecrypt(text, key, alphabet):
    decrypted_text = []
    key = generateKey(text, key)
    for i in range(len(text)):
        if text[i].isalpha():
            idx = (alphabet.index(text[i].lower()) - alphabet.index(key[i].lower()) + len(alphabet)) % len(alphabet)
            decrypted_text.append(alphabet[idx])
        else:
            decrypted_text.append(text[i])
    return ''.join(decrypted_text)

def getCustomAlphabet(word):
    remaining_alphabet = ''.join([char for char in alphabet if char not in word])
    result = word + remaining_alphabet
    return result

def getAlphabeticalDistance(str1, str2):
    if len(str1) != len(str2):
        raise ValueError("Strings must be of the same length")
    
    totalDistance = 0
    
    for char1, char2 in zip(str1, str2):
        distance = abs(ord(char1) - ord(char2))
        totalDistance += distance
    
    return totalDistance

def writeRecord(text):
    with open("bestWords.txt", "a") as file:
        file.write(f"{text}\n")



def attemptDecrypt(K4, word, fullWordList, recurse=False, prevWord=None, bestDistance=math.inf, bestMatchWord="", bestPhrase=""):
    decipher_text = vigenereDecrypt(K4, word, getCustomAlphabet("kryptos"))

    currentDistance = getAlphabeticalDistance(decipher_text[21:34].lower(), "eastnortheast")
        
    if currentDistance < bestDistance:
        bestDistance = currentDistance
        bestMatchWord = word
        bestPhrase = decipher_text[21:34]

    if decipher_text[21:34].lower() == "eastnortheast":
        result = f"Solution found!!!!!!!!!! With word: {word} and prevWord: {prevWord}"
        print(result)
        return result, bestDistance, bestMatchWord
    elif recurse:
        for next_word in fullWordList:
            result, distance, match_word, best_phrase = attemptDecrypt(decipher_text, next_word, fullWordList, False, word, bestDistance)

            if distance < bestDistance:
                bestDistance = distance
                bestMatchWord = match_word
                bestPhrase = best_phrase

            if result:
                return result, distance, match_word, best_phrase
            
    return None, bestDistance, bestMatchWord, bestPhrase

def processSegment(segment, full_word_list, segmentIndex):
    i = 0
    for word in segment:
        result, bestDistance, bestMatchWord, bestPhrase = attemptDecrypt(K4, word, full_word_list, recurse=True)
        if result:
            return result
        else:
            i += 1
            removeWordFromFile(word, segmentIndex)
            msg = f"Word {i} - '{word}' is not the solution. Best match word is {bestMatchWord}. Which spells out {bestPhrase} with smallest distance of ->{bestDistance}"
            print(msg)
            if bestDistance < globalBestDistance:
                writeRecord(msg)
    return None

def bruteForceVigenereParallel(K4):
    segments = getSegmentsFromFiles()
    
    with ThreadPoolExecutor(max_workers=maxWorkers) as executor:
        future_to_segment = {executor.submit(processSegment, segment, allWordList, index): (index, segment) for index, segment in enumerate(segments)}

        for future in as_completed(future_to_segment):
            result = future.result()
            if result:
                print(result)
                break


bruteForceVigenereParallel(K4)

