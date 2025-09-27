#include <iostream>
#include <string>
using namespace std;

// Rotate left 32-bit sederhana
unsigned int rotl(unsigned int x, int n) {
    return (x << n) | (x >> (32 - n));
}

// Quarter round sederhana (mirip Salsa20)
void quarterRound(unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d) {
    b ^= rotl(a + d, 7);
    c ^= rotl(b + a, 9);
    d ^= rotl(c + b, 13);
    a ^= rotl(d + c, 18);
}

// Generate keystream sederhana
void generateKeyStream(unsigned int key[4], unsigned int nonce[2], unsigned int keystream[16]) {
    for(int i = 0; i < 4; i++) keystream[i] = key[i];
    for(int i = 0; i < 2; i++) keystream[4+i] = nonce[i];
    for(int i = 6; i < 16; i++) keystream[i] = 0xdeadbeef; // filler

    // 2 round sederhana
    for(int round=0; round<2; round++){
        quarterRound(keystream[0], keystream[4], keystream[8], keystream[12]);
        quarterRound(keystream[1], keystream[5], keystream[9], keystream[13]);
        quarterRound(keystream[2], keystream[6], keystream[10], keystream[14]);
        quarterRound(keystream[3], keystream[7], keystream[11], keystream[15]);
    }
}

// Enkripsi/dekripsi (XOR dengan keystream)
string salsaEncrypt(const string &text, unsigned int key[4], unsigned int nonce[2]) {
    unsigned int keystream[16];
    generateKeyStream(key, nonce, keystream);
    string output = text;
    for(size_t i = 0; i < text.size(); i++){
        output[i] ^= ((char*)keystream)[i % 64]; // 16*4 byte keystream
    }
    return output;
}

int main() {
    string plaintext;
    cout << "Masukkan plaintext: ";
    getline(cin, plaintext);

    string key_input;
    cout << "Masukkan key 16 karakter: ";
    getline(cin, key_input);
    while(key_input.size() < 16) key_input += '\0';

    unsigned int key[4];
    for(int i=0;i<4;i++){
        key[i] = *(unsigned int*)(key_input.data()+i*4);
    }

    unsigned int nonce[2] = {0x12345678, 0x9abcdef0};

    string cipher = salsaEncrypt(plaintext, key, nonce);
    cout << "Ciphertext (hex): ";
    for(unsigned char c : cipher) printf("%02x", c);
    cout << endl;

    string decrypted = salsaEncrypt(cipher, key, nonce);
    cout << "Hasil dekripsi: " << decrypted << endl;

    return 0;
}
