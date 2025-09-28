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

unsigned int rotl(unsigned int x, int n) {
    return (x << n) | (x >> (32 - n));
}

void quarterRound(unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d) {
    b ^= rotl(a + d, 7);
    c ^= rotl(b + a, 9);
    d ^= rotl(c + b, 13);
    a ^= rotl(d + c, 18);
}

void generateKeyStream(unsigned int key[4], unsigned int nonce[2], unsigned int keystream[16]) {
    for(int i = 0; i < 4; i++) keystream[i] = key[i];
    for(int i = 0; i < 2; i++) keystream[4+i] = nonce[i];
    for(int i = 6; i < 16; i++) keystream[i] = 0xdeadbeef;

    for(int round=0; round<2; round++){
        quarterRound(keystream[0], keystream[4], keystream[8], keystream[12]);
        quarterRound(keystream[1], keystream[5], keystream[9], keystream[13]);
        quarterRound(keystream[2], keystream[6], keystream[10], keystream[14]);
        quarterRound(keystream[3], keystream[7], keystream[11], keystream[15]);
    }
}

string salsaEncrypt(const string &text, unsigned int key[4], unsigned int nonce[2]) {
    unsigned int keystream[16];
    generateKeyStream(key, nonce, keystream);
    string output = text;
    for(size_t i = 0; i < text.size(); i++){
        output[i] ^= ((char*)keystream)[i % 64];
    }
    return output;
}

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

string bytesToHex(const string &bytes) {
    const char *hexChars = "0123456789abcdef";
    string result = "";
    for (size_t i = 0; i < bytes.size(); i++) {
        unsigned char c = bytes[i];
        result += hexChars[(c >> 4) & 0xF];
        result += hexChars[c & 0xF];
    }
    return result;
}


string vigenereEncrypt(string text, string key) {
    string result = "";
    int keyIndex = 0;
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char k = tolower(key[keyIndex % key.size()]) - 'a';
            result += char((c - base + k) % 26 + base);
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

string vigenereDecrypt(string text, string key) {
    string result = "";
    int keyIndex = 0;
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char k = tolower(key[keyIndex % key.size()]) - 'a';
            result += char((c - base - k + 26) % 26 + base);
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

string xorCipher(string text, char key) {
    string result = text;
    for (size_t i = 0; i < text.size(); i++) {
        result[i] = text[i] ^ key;
    }
    return result;
}

int main() {
    int pilihan, subpilihan, key;
    string text, keyV, key_input;
    char keyX;

    cout << "===== MENU KRIPTOGRAFI =====" << endl;
    cout << "1. Caesar Cipher" << endl;
    cout << "2. Vigenere Cipher" << endl; 
    cout << "3. XOR Cipher" << endl;     
    cout << "4. Salsa20 (Sederhana)" << endl;
    cout << "5. Super Enkripsi" << endl;
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

        if (subpilihan == 1) {
            cout << "Masukkan Plaintext: ";
            getline(cin, text);
            cout << "Key: ";
            cin >> key;
            cout << "Ciphertext: " << caesarEncrypt(text, key) << endl << endl;
        } else if (subpilihan == 2) {
            cout << "Masukkan Ciphertext: ";
            getline(cin, text);
            cout << "Key: ";
            cin >> key;
            cout << "Plaintext: " << caesarDecrypt(text, key) << endl << endl ;
        } else {
            cout << "Pilihan tidak valid!" << endl << endl;
        }
        main();
    } 
    
    else if (pilihan == 2) { 
        cout << "\n-- Vigenere Cipher --" << endl;
        cout << "1. Enkripsi" << endl;
        cout << "2. Dekripsi" << endl;
        cout << "Pilih: ";
        cin >> subpilihan;
        cin.ignore();
        if (subpilihan == 1){
            cout << "Masukkan Plaintext: ";
            getline(cin, text);
            cout << "Masukkan key (kata): ";
            getline(cin, keyV);
            cout << "Ciphertext: " << vigenereEncrypt(text, keyV) << endl << endl;
        } else if (subpilihan == 2){
            cout << "Masukkan Ciphertext: ";
            getline(cin, text);
            cout << "Masukkan key (kata): ";
            getline(cin, keyV);
            cout << "Plaintext: " << vigenereDecrypt(text, keyV) << endl << endl;
        } else {
            cout << "Pilihan tidak valid!" << endl << endl;
        }
        main();
    }

    else if (pilihan == 3) { 
        cout << "\n-- XOR Cipher --" << endl;
        cout << "1. Enkripsi" << endl;
        cout << "2. Dekripsi" << endl;
        cout << "Pilih: ";
        cin >> subpilihan;
        cin.ignore();

        cout << "Masukkan teks: ";
        getline(cin, text);

        char key;
        cout << "Masukkan key (1 karakter): ";
        cin >> key;

        if (subpilihan == 1) {
            string hasil = xorCipher(text, key);
            cout << "Ciphertext (hasil enkripsi): " << bytesToHex(hasil) << endl << endl;
        } 
        else if (subpilihan == 2) {
            string cipher = hexToBytes(text);        
            string decrypted = xorCipher(cipher, key);
            cout << "Plaintext (hasil dekripsi): " << decrypted << endl << endl;
        } 
        else {
            cout << "Pilihan tidak valid!" << endl;
        }
        main();
    }
        

    else if (pilihan == 4){
        cout << "\n--Salsa20 Cipher --" << endl;
        cout << "1. Enkripsi" << endl;
        cout << "2. Dekripsi" << endl;
        cout << "Pilih: ";
        cin >> subpilihan;
        cin.ignore();

        if (subpilihan == 1) {
            cout << "Masukkan Plaintext: ";
            getline(cin, text);

            cout << "Masukkan key 16 karakter: ";
            getline(cin, key_input);
            while(key_input.size() < 16) key_input += '\0';

            unsigned int key[4];
            for(int i=0;i<4;i++){
                key[i] = *(unsigned int*)(key_input.data()+i*4);
            }
            unsigned int nonce[2] = {0x12345678, 0x9abcdef0};
            string cipher = salsaEncrypt(text, key, nonce);
            cout << "Ciphertext (hex): " << bytesToHex(cipher) << endl << endl;
        } else if (subpilihan == 2) {
            cout << "Masukkan Ciphertext: ";
            getline(cin, text);

            cout << "Masukkan key 16 karakter: ";
            getline(cin, key_input);
            while(key_input.size() < 16) key_input += '\0';

            unsigned int key[4];
            for(int i=0;i<4;i++){
                key[i] = *(unsigned int*)(key_input.data()+i*4);
            }

        unsigned int nonce[2] = {0x12345678, 0x9abcdef0};
            string cipher = hexToBytes(text);        
            string decrypted = salsaEncrypt(cipher, key, nonce);
            cout << "Hasil dekripsi: " << decrypted << endl << endl;
        } else {
            cout << "Pilihan tidak valid!" << endl << endl;
        }
        main();
    } 
    else if (pilihan == 5) {
    cout << "\n-- Super Enkripsi --" << endl;
    cout << "1. Enkripsi" << endl;
    cout << "2. Dekripsi" << endl;
    cout << "Pilih: ";
    cin >> subpilihan;
    cin.ignore();

    if (subpilihan == 1) {
        cout << "Masukkan Plaintext: ";
        getline(cin, text);

        cout << "Key Caesar (int): ";
        cin >> key;
        text = caesarEncrypt(text, key);
        cout << "Hasil Caesar: " << text << endl << endl;
        cin.ignore();

        cout << "Key Vigenere (kata): ";
        getline(cin, keyV);
        text = vigenereEncrypt(text, keyV);
        cout << "Hasil Vigenere: " << text << endl << endl;

        cout << "Key XOR (1 karakter): ";
        cin >> keyX;
        text = xorCipher(text, keyX);
        cout << "Hasil XOR: " << text << endl << endl;
        cin.ignore();

        cout << "Key Salsa20 (16 karakter): ";
        getline(cin, key_input);
        while(key_input.size() < 16) key_input += '\0';

        unsigned int keySalsa[4];
        for(int i=0; i<4; i++){
            keySalsa[i] = *(unsigned int*)(key_input.data()+i*4);
        }
        unsigned int nonce[2] = {0x12345678, 0x9abcdef0};

        string cipher = salsaEncrypt(text, keySalsa, nonce);
        cout << "Ciphertext (hex): " << bytesToHex(cipher) << endl << endl;
        main();
    } else if (subpilihan == 2) {
        cout << "Masukkan Ciphertext (hex dari Salsa): ";
        getline(cin, text);

        cout << "Key Salsa20 (16 karakter): ";
        getline(cin, key_input);
        while(key_input.size() < 16) key_input += '\0';

        unsigned int keySalsa[4];
        for(int i=0; i<4; i++){
            keySalsa[i] = *(unsigned int*)(key_input.data()+i*4);
        }
        unsigned int nonce[2] = {0x12345678, 0x9abcdef0};

        string cipherBytes = hexToBytes(text);
        text = salsaEncrypt(cipherBytes, keySalsa, nonce);
        cout << "Setelah Salsa: " << text << endl << endl;

        cout << "Key XOR (1 karakter): ";
        cin >> keyX;
        text = xorCipher(text, keyX);
        cout << "Setelah XOR: " << text << endl << endl;
        cin.ignore();

        cout << "Key Vigenere (kata): ";
        getline(cin, keyV);
        text = vigenereDecrypt(text, keyV);
        cout << "Setelah Vigenere: " << text << endl << endl;

        cout << "Key Caesar (int): ";
        cin >> key;
        text = caesarDecrypt(text, key);
        cout << "Plaintext Akhir: " << text << endl << endl;
        main();
    } else {
        cout << "Pilihan tidak valid!" << endl << endl;
        main();
    }
}
    
    else {
        cout << "Keluar program..." << endl;
    }

    return 0;
}
