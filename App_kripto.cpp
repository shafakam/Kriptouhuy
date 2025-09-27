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

int main() {
    int pilihan, subpilihan, key;
    string text;

    cout << "===== MENU KRIPTOGRAFI =====" << endl;
    cout << "1. Caesar Cipher" << endl;
    cout << "2. Exit" << endl;
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
    } else {
        cout << "Keluar program..." << endl;
    }

    return 0;
}