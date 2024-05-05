K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
K4Solution = 'OBKRUOXOGHULBSOLIFBBWEASTNORTHEASTOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTCLOCKWGDKZXTJCDIGKUHUAUEKCAR'

def generate_key(text, key):
    key = key * (len(text) // len(key) + 1)
    return key[:len(text)]

def vigenere_decrypt(text, key, alphabet):
    decrypted_text = []
    key = generate_key(text, key)
    for i in range(len(text)):
        if text[i].isalpha():
            idx = (alphabet.index(text[i].upper()) - alphabet.index(key[i].upper()) + len(alphabet)) % len(alphabet)
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


K1 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD"
K2 = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKKDQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVHDWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZFKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETG"
K3 = "ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLLNOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMTWNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEYQHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOASFIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROAGRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEHAGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?"
K3Spaces = "ENDYAHROHNLSRHEOCPTEOIBI DYSHNAIACHTNREYULDSLLSLL NOHSNOSMRWXMNETPRNGATIHN RARPESLNNELEBLPIIACAEWMT WNDITEENRAHCTENEUDRETNHA EOETFOLSEDTIWENHAEIOYTEY QHEENCTAYCREIFTBRSPAMHHE WENATAMATEGYEERLBTEEFOAS FIOTUETUAEOTOARMAEERTNRT IBSEDDNIAAHTTMSTEWPIEROA GRIEWFEBAECTDDHILCEIHSIT EGOEAOSDDRYDLORITRKLMLEH AGTDHARDPNEOHMGFMFEUHEEC DMRIPFEIMEHNLSSTTRTVDOHW"

kryptos_alphabet = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
print("K1: ", vigenere_decrypt(K1, "PALIMPSEST", kryptos_alphabet))
print("K2: ", vigenere_decrypt(K2, "ABSCISSA", kryptos_alphabet))
print("K3 Simple: ", simple_k3(K3, 192))
print("K3 Intended: ", intended_k3(K3Spaces))

#aa = "ILNTAYESTATHCW BLHMHEHAROIEEH ISIWNTHONRSLEO OLTETYMFTEHMHD ELAAEOAERIILUV TSGCRIPEEPEKET PDNADESTEWCRFR CLRIUARBAELTMT OUPIEHBLMTIIFT EYTPNNTRRSHRGS HEELEEFEAMDOMS RRNBTWIEOTDLHL SNMECIEYTTTDON LTXLHTRGOHCYEH NHWEADCEEAERNE HCRNREYTAAADPM OAMNNSAAUIBDDI RISLELTMTNESRE HAOSEOCAEDFOAF ANNETFNTUDWAHP YHSPITEATEEEDI DSHRDEENOSIOTR NYOANOHEIBRGGM EDNRWEQWFIGEAD"

#print(vigenere_decrypt(vigenere_decrypt(K4, "ORDINATE"), "PALIMPSEST"))