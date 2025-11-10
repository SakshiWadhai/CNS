#include <iostream>
#include <fstream>
using namespace std;

int main() {
    string filename, outname;
    char key;

    cout << "Enter image filename to encrypt/decrypt: ";
    cin >> filename;
    cout << "Enter output filename: ";
    cin >> outname;
    cout << "Enter single character key: ";
    cin >> key;

    ifstream input(filename, ios::binary);
    ofstream output(outname, ios::binary);

    if (!input || !output) {
        cout << "Error: File not found or cannot be opened!" << endl;
        return 1;
    }

    char ch;
    while (input.get(ch)) {
        ch = ch ^ key;  // XOR encryption/decryption
        output.put(ch);
    }

    input.close();
    output.close();

    cout << "\nProcess completed successfully!\n";
    cout << "Output file: " << outname << endl;
    cout << "(Run again with same key to decrypt the image)\n";

    return 0;
}
