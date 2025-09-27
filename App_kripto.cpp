#include <iostream>
#include <string>
using namespace std;

const int BLOCK_SIZE = 16; // 16 byte

// fungsi enkripsi
string encryptCBC(const string &plaintext, const string &key, const string &iv, int &numBlocks) {
    int len = plaintext.size();
    numBlocks = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;

    string ciphertext(numBlocks * BLOCK_SIZE, '\0');
    string prev = iv;

    for (int b = 0; b < numBlocks; b++) {
        string block(BLOCK_SIZE, '\0');
        // ambil plaintext ke blok
        for (int i = 0; i < BLOCK_SIZE; i++) {
            int idx = b * BLOCK_SIZE + i;
            if (idx < len) block[i] = plaintext[idx];
        }
        // XOR dengan prev
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] ^= prev[i];
        }
        // XOR dengan key
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] ^= key[i];
        }
        // simpan ke ciphertext
        for (int i = 0; i < BLOCK_SIZE; i++) {
            ciphertext[b * BLOCK_SIZE + i] = block[i];
        }
        // update prev
        prev = block;
    }
    return ciphertext;
}

// fungsi dekripsi
string decryptCBC(const string &ciphertext, const string &key, const string &iv, int numBlocks, int plainLen) {
    string decrypted(numBlocks * BLOCK_SIZE, '\0');
    string prev = iv;

    for (int b = 0; b < numBlocks; b++) {
        string block(BLOCK_SIZE, '\0');
        // ambil ciphertext ke blok
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] = ciphertext[b * BLOCK_SIZE + i];
        }
        // XOR dengan key
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] ^= key[i];
        }
        // XOR dengan prev
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] ^= prev[i];
        }
        // simpan hasil
        for (int i = 0; i < BLOCK_SIZE; i++) {
            decrypted[b * BLOCK_SIZE + i] = block[i];
        }
        // update prev
        prev = ciphertext.substr(b * BLOCK_SIZE, BLOCK_SIZE);
    }
    decrypted.resize(plainLen); // buang padding
    return decrypted;
}

// fungsi print ciphertext
void printCiphertext(const string &ciphertext, int numBlocks) {
    cout << "Ciphertext (hex): ";
    for (int i = 0; i < numBlocks * BLOCK_SIZE; i++) {
        unsigned char c = ciphertext[i];
        cout << hex << (int)c << " ";
    }
    cout << endl;
}

int main() {
    string plaintext, key, iv;
    cout << "Masukkan plaintext: ";
    getline(cin, plaintext);

    cout << "Masukkan key (16 karakter): ";
    getline(cin, key);
    while (key.size() != BLOCK_SIZE) {
        cout << "Error: Key harus tepat 16 karakter!\n";
        cout << "Masukkan key (16 karakter): ";
        getline(cin, key);
    }

    cout << "Masukkan IV (16 karakter): ";
    getline(cin, iv);
    while (iv.size() != BLOCK_SIZE) {
        cout << "Error: IV harus tepat 16 karakter!\n";
        cout << "Masukkan IV (16 karakter): ";
        getline(cin, iv);
    }

    int numBlocks;
    string ciphertext = encryptCBC(plaintext, key, iv, numBlocks);

    printCiphertext(ciphertext, numBlocks);

    string decrypted = decryptCBC(ciphertext, key, iv, numBlocks, plaintext.size());
    cout << "Hasil dekripsi: " << decrypted << endl;
}
