#include <iostream>
#include <string>

using namespace std;

string caesarEncrypt(string text, int key) {
    string result = "";
    for (char c : text) {
        if (c >= 'A' && c <= 'Z') { 
            result += char((c - 'A' + key) % 26 + 'A');
        } else if (c >= 'a' && c <= 'z') { 
            result += char((c - 'a' + key) % 26 + 'a');
        } else {
            result += c; 
        }
    }
    return result;

}

string caesarDecrypt(string text, int key) {
    return caesarEncrypt(text, 26 - key);
}


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

// ubah string hex -> string biner
string hexToBytes(const string &hex) {
    string result = "";
    for (size_t i = 0; i < hex.size(); i += 2) {
        char h1 = hex[i];
        char h2 = hex[i+1];

        int hi = (h1 >= '0' && h1 <= '9') ? h1 - '0' :
                 (h1 >= 'a' && h1 <= 'f') ? h1 - 'a' + 10 :
                 (h1 - 'A' + 10);

        int lo = (h2 >= '0' && h2 <= '9') ? h2 - '0' :
                 (h2 >= 'a' && h2 <= 'f') ? h2 - 'a' + 10 :
                 (h2 - 'A' + 10);

        result += char((hi << 4) | lo);
    }
    return result;
}

// ubah string biner -> hex
string bytesToHex(const string &bytes) {
    const char *hexChars = "0123456789abcdef";
    string result = "";
    for (size_t i = 0; i < bytes.size(); i++) {
        unsigned char c = bytes[i];
        result += hexChars[(c >> 4) & 0xF];  // nibble tinggi
        result += hexChars[c & 0xF];         // nibble rendah
    }
    return result;
}


int main() {
    int pilihan, subpilihan, key;
    string text;

    cout << "===== MENU KRIPTOGRAFI =====" << endl;
    cout << "1. Caesar Cipher" << endl;
    cout << "4. Salsa20 (Sederhana)" << endl;
    cout << "6. Exit" << endl;
    cout << "Pilih menu: ";
    cin >> pilihan;
    cin.ignore();

    if (pilihan == 1) {
        cout << "\n-- Caesar Cipher --" << endl;
        cout << "1. Enkripsi" << endl;
        cout << "2. Dekripsi" << endl;
        cout << "Pilih: ";
        cin >> subpilihan;
        cin.ignore();

        cout << "Masukkan teks: ";
        getline(cin, text);

        cout << "Key: ";
        cin >> key;

        if (subpilihan == 1) {
            cout << "Ciphertext: " << caesarEncrypt(text, key) << endl << endl;
            main();
        } else if (subpilihan == 2) {
            cout << "Plaintext: " << caesarDecrypt(text, key) << endl << endl ;
            main();
        } else {
            cout << "Pilihan tidak valid!" << endl;
        }
    } else if (pilihan == 4){
        cout << "\n--Salsa20 Cipher --" << endl;
        cout << "1. Enkripsi" << endl;
        cout << "2. Dekripsi" << endl;
        cout << "Pilih: ";
        cin >> subpilihan;
        cin.ignore();

        cout << "Masukkan plaintext: ";
        getline(cin, text);

        string key_input;
        cout << "Masukkan key 16 karakter: ";
        getline(cin, key_input);
        while(key_input.size() < 16) key_input += '\0';

        unsigned int key[4];
        for(int i=0;i<4;i++){
            key[i] = *(unsigned int*)(key_input.data()+i*4);
        }

        unsigned int nonce[2] = {0x12345678, 0x9abcdef0};

        if (subpilihan == 1) {
            string cipher = salsaEncrypt(text, key, nonce);
            cout << "Ciphertext (hex): " << bytesToHex(cipher) << endl;
            main();
        } else if (subpilihan == 2) {
            string cipher = hexToBytes(text);        
            string decrypted = salsaEncrypt(cipher, key, nonce);
            cout << "Hasil dekripsi: " << decrypted << endl;
            main();
        } else {
            cout << "Pilihan tidak valid!" << endl;
        }
    } else {
        cout << "Keluar program..." << endl;
    }

    return 0;
}
