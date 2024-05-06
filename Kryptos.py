import nltk
nltk.download('words')
from nltk.corpus import words
import threading
from queue import Queue
import time

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

def transposition_decrypt(ciphertext, key):
    num_rows = len(ciphertext) // key
    num_extra = len(ciphertext) % key
    
    decrypted_text = [''] * len(ciphertext)
    current_index = 0
    
    for column in range(key):
        start_index = column * num_rows + min(column, num_extra)
        for row in range(num_rows + (1 if column < num_extra else 0)):
            decrypted_text[start_index + row] = ciphertext[current_index]
            current_index += 1
            
    return ''.join(decrypted_text)

def simple_k3(ciphertext, n):
    decrypted_text = []
    total = n * len(ciphertext)
    i = 0
    while(i < total):
        if(i % n == 0):
            decrypted_text.append(ciphertext[i % len(ciphertext)])
        i += 1
    return ''.join(decrypted_text)

def getMatrix(ciphertext):
    rows = ciphertext.split()
    matrix = [list(row) for row in rows]

    return matrix

def transformMatrix(matrix, cols):
    flat_list = [item for sublist in matrix for item in sublist]
    
    new_row_count = len(flat_list) // cols + (1 if len(flat_list) % cols > 0 else 0)
    
    new_matrix = []
    for i in range(new_row_count):
        row_start = i * cols
        row_end = row_start + cols
        new_matrix.append(flat_list[row_start:row_end])
    
    return new_matrix

def rotateMatrixAnticlockwise(matrix):
    num_rows = len(matrix)
    num_cols = len(matrix[0])
    
    rotated_matrix = []
    for col in range(num_cols):
        new_row = []
        for row in range(num_rows-1, -1, -1):
            new_row.append(matrix[num_rows - row - 1][num_cols - col - 1])
        rotated_matrix.append(new_row)

    return rotated_matrix

def intended_k3(ciphertext):
    K3Matrix = getMatrix(ciphertext)
    K3MatrixRotate = rotateMatrixAnticlockwise(K3Matrix)
    K3Matrix8Col = transformMatrix(K3MatrixRotate, 8)
    plaintext = rotateMatrixAnticlockwise(K3Matrix8Col)
    return ''.join(''.join(row) for row in plaintext)

def bruteforceMatrixRotation(K4):
    K4 = getMatrix(K4)
    K4 = rotateMatrixAnticlockwise(K4)
    K4 = transformMatrix(K4, 8)
    K4 = rotateMatrixAnticlockwise(K4)
    return ''.join(''.join(row) for row in K4)


def berlinClock1213(K4):
    K4 = vigenere_decrypt(K4, "PALIMPSET", kryptosAlphabet)
    K4 = vigenere_decrypt(K4, "ABSCISSA", kryptosAlphabet)
    K4 = vigenere_decrypt(K4, "PALIMPSET", kryptosAlphabet)
    #K4 = intended_k3(K4)
    return K4

def split1213(K4):
    code1 = []
    code2 = []
    code3 = []
    while len(K4) != 0:
        code1.append(K4[:1])
        K4 = K4[1:]
        code2.append(K4[:1])
        K4 = K4[1:]
        code1.append(K4[:1])
        K4 = K4[1:]
        code3.append(K4[:1])
        K4 = K4[1:]

    K1 = vigenere_decrypt(code1, "PALIMPSET", kryptosAlphabet)
    K2 = vigenere_decrypt(code2, "ORDINATE", kryptosAlphabet)
    print(K1)
    print(K2)
    #K3 = intended_k3(code3)


K1 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD"
K2 = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKKDQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVHDWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZFKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETG"
K3 = "ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLLNOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMTWNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEYQHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOASFIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROAGRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEHAGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?"
K3Spaces = "ENDYAHROHNLSRHEOCPTEOIBI DYSHNAIACHTNREYULDSLLSLL NOHSNOSMRWXMNETPRNGATIHN RARPESLNNELEBLPIIACAEWMT WNDITEENRAHCTENEUDRETNHA EOETFOLSEDTIWENHAEIOYTEY QHEENCTAYCREIFTBRSPAMHHE WENATAMATEGYEERLBTEEFOAS FIOTUETUAEOTOARMAEERTNRT IBSEDDNIAAHTTMSTEWPIEROA GRIEWFEBAECTDDHILCEIHSIT EGOEAOSDDRYDLORITRKLMLEH AGTDHARDPNEOHMGFMFEUHEEC DMRIPFEIMEHNLSSTTRTVDOHW"
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
K4Solution = 'OBKRUOXOGHULBSOLIFBBWEASTNORTHEASTOTWTQSJQSSEKZZWATJKLUDIAWINFBBERLINCLOCKWGDKZXTJCDIGKUHUAUEKCAR'

kryptosAlphabet = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
#print("K1: ", vigenere_decrypt(K1, "PALIMPSEST", kryptos_alphabet))
#print("K2: ", vigenere_decrypt(K2, "ABSCISSA", kryptos_alphabet))
#print("K3 Simple: ", simple_k3(K3, 192))
#print("K3 Intended: ", intended_k3(K3Spaces))
#print("K4: ", berlinClock1213(K4))
#print(vigenere_decrypt(vigenere_decrypt(K4, "ORDINATE"), "PALIMPSEST"))
#print("K4: ", split1213(K4))
#print(K4Solution[21:34])

wordList = words.words()
wordList.append("kryptos")
wordList = [word.lower() for word in wordList if 4 <= len(word) <= 10]
alphabet = "abcdefghijklmnopqrstuvwxyz"


def getCustomAlphabet(word):
    remaining_alphabet = ''.join([char for char in alphabet if char not in word])
    
    result = word + remaining_alphabet
    return result

def hasNoRepeatedCharacters(word):
    return len(set(word)) == len(word)

nonRepeatedWords = [word.lower() for word in wordList if hasNoRepeatedCharacters(word)]


def bruteForceVigenereCustomAlphabet(K4, depth=1, maxDepth=2):
    if depth > maxDepth:
        return
     
    i = 0
    for alphabetWord in nonRepeatedWords:
        if hasNoRepeatedCharacters(alphabetWord):
            for word in wordList:
                decipherText1 = vigenere_decrypt(K4, word, getCustomAlphabet(alphabetWord))

                if(decipherText1[21:34].lower() == "eastnortheast"):
                    print("solution found!", word)
                    return

                if depth < maxDepth:
                        print(f"----------K4 and word {word} with alphabetWord {alphabetWord}, not the solution. Run depth 2----------")
                        bruteForceVigenereCustomAlphabet(decipherText1, depth + 1, maxDepth)
                
                if i % 1000000 == 0 and i != 0:
                    print(f"Iteration {i/1000000}m: Tested alphabet '{alphabetWord}', text {K4[0:10]} word '{word}', not a solution yet.")
                
                i += 1

def bruteForceVigenere(K4, depth=1, maxDepth=2, prevWord=None, i = 0):
    for word in wordList:
        decipherText = vigenere_decrypt(K4, word, getCustomAlphabet("kryptos"))

        if(decipherText[21:34].lower() == "eastnortheast"):
            print(f"solution found! word: {word} and previous word: {prevWord}")
            return

        if depth < maxDepth:
                print(f"----------K4 and word {word}, not the solution. Run depth 2----------")
                bruteForceVigenere(decipherText, depth + 1, maxDepth, word, i)
                i += len(wordList)

        if i != 0 and i % 1000000 == 0:
            print(f"Iteration {i/1000000}m - prevWord: '{prevWord}' - text '{K4[0:10]}' - word '{word}' wordsLeft: - not a solution yet.")
            print(f"time elapsed {time.time() - startTime}")
        
        i += 1

startTime = time.time()
bruteForceVigenere(K4)

#Notes

#07:08:44 x Layer Two
#F   L    R   V    Q     Q    P   R     N     G    K      S S
#E   A    S   T    N     O    R   T     H     E    A      S T
#1 -11/15 1 -2/24 -3/23 -2/24 1 2/-24 -6/20 -2/24  -10/14 0 1
#1   15   1   24   23    24   1   2     20    24    14    0 1

#N  Y P V  T  T   M  Z F P  K
#B  E R L  I  N   C  L O C  K
#14 6 2 16 15 20 10 12 9 13 0

