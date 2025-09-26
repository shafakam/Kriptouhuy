#include <iostream>
using namespace std;

const int BLOCK_SIZE = 16; // 16 byte

int main() {
    // ---- Data awal dari input user ----
    char plaintext[1024];
    char key[BLOCK_SIZE];
    char iv[BLOCK_SIZE];

    cout << "Masukkan plaintext: ";
    cin.getline(plaintext, 1024);

    cout << "Masukkan key (max 15 karakter): ";
    cin.getline(key, BLOCK_SIZE);

    cout << "Masukkan IV (max 15 karakter): ";
    cin >> ws;             // buang whitespace/enter yang tersisa
    cin.getline(iv, BLOCK_SIZE);

    // buffer untuk cipher & hasil dekripsi
    char ciphertext[1024];
    char decrypted[1024];

    // hitung panjang plaintext manual
    int len = 0;
    while (plaintext[len] != '\0') {
        len++;
    }

    // jumlah blok (dibulatkan ke atas)
    int numBlocks = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // --- ENKRIPSI ---
    char prev[BLOCK_SIZE]; // buat simpan blok sebelumnya (CBC)
    for (int i = 0; i < BLOCK_SIZE; i++) {
        prev[i] = iv[i]; // blok pertama pakai IV
    }

    for (int b = 0; b < numBlocks; b++) {
        char block[BLOCK_SIZE];
        
        // ambil plaintext ke blok (manual copy)
        for (int i = 0; i < BLOCK_SIZE; i++) {
            int idx = b * BLOCK_SIZE + i;
            if (idx < len) {
                block[i] = plaintext[idx];
            } else {
                block[i] = 0; // padding nol
            }
        }

        // XOR dengan prev (IV / cipher sebelumnya)
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] = block[i] ^ prev[i];
        }

        // "Enkripsi" block dengan XOR key
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] = block[i] ^ key[i];
        }

        // simpan hasil ke ciphertext
        for (int i = 0; i < BLOCK_SIZE; i++) {
            ciphertext[b * BLOCK_SIZE + i] = block[i];
        }

        // update prev dengan ciphertext terbaru
        for (int i = 0; i < BLOCK_SIZE; i++) {
            prev[i] = block[i];
        }
    }

    // tampilkan ciphertext dalam heksadesimal
    cout << "Ciphertext (hex): ";
    for (int i = 0; i < numBlocks * BLOCK_SIZE; i++) {
        unsigned char c = ciphertext[i];
        cout << hex << (int)c << " ";
    }
    cout << endl;

    // --- DEKRIPSI ---
    for (int i = 0; i < BLOCK_SIZE; i++) {
        prev[i] = iv[i]; // reset IV
    }

    for (int b = 0; b < numBlocks; b++) {
        char block[BLOCK_SIZE];

        // ambil ciphertext ke block
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] = ciphertext[b * BLOCK_SIZE + i];
        }

        // "Dekripsi" dengan XOR key
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] = block[i] ^ key[i];
        }

        // XOR lagi dengan prev
        for (int i = 0; i < BLOCK_SIZE; i++) {
            block[i] = block[i] ^ prev[i];
        }

        // simpan ke hasil dekripsi
        for (int i = 0; i < BLOCK_SIZE; i++) {
            decrypted[b * BLOCK_SIZE + i] = block[i];
        }

        // update prev dengan ciphertext sekarang
        for (int i = 0; i < BLOCK_SIZE; i++) {
            prev[i] = ciphertext[b * BLOCK_SIZE + i];
        }
    }

    // kasih null terminator biar string aman
    decrypted[len] = '\0';

    cout << "Hasil dekripsi: " << decrypted << endl;
}
