#include <iostream>
#include <string>
using namespace std;

// Encryption: Shifts ASCII values and applies XOR with the key
string encrypt(const string &text, int key) {
    string cipher = "";
    for (char ch : text) {
        unsigned char c = static_cast<unsigned char>(ch);
        c = (c ^ key) + key;  // XOR first, then shift
        cipher += c;
    }
    return cipher;
}

// Decryption: Reverses the encryption process
string decrypt(const string &cipher, int key) {
    string plain = "";
    for (char ch : cipher) {
        unsigned char c = static_cast<unsigned char>(ch);
        c = (c - key) ^ key;  // Reverse order of operations
        plain += c;
    }
    return plain;
}

int main() {
    string text;
    int key;

    cout << "Enter text: ";
    getline(cin, text);
    cout << "Enter key (integer): ";
    cin >> key;

    string encrypted = encrypt(text, key);
    string decrypted = decrypt(encrypted, key);

    cout << "\nOriginal Text: " << text;
    cout << "\nEncrypted Text: ";
    for (unsigned char c : encrypted)
        cout << hex << (int)c << " "; 
    cout << "\nDecrypted Text: " << decrypted << endl;

    return 0;
}
