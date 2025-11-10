#include<bits/stdc++.h>
using namespace std;

// Caesar Cipher
string caesarEncrypt(string text, int key)
{
    string result = "";
    for (char c : text)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            result += (c - base + key) % 26 + base;
        }
        else
            result += c;
    }
    return result;
}
string caesarDecrypt(string text, int key) { return caesarEncrypt(text, 26 - key); }

// Monoalphabetic Cipher
string monoEncrypt(string text, string sub)
{
    string result = "";
    for (char c : text)
    {
        if (isalpha(c))
        {
            bool upper = isupper(c);
            c = tolower(c);
            result += upper ? toupper(sub[c - 'a']) : sub[c - 'a'];
        }
        else
            result += c;
    }
    return result;
}
string monoDecrypt(string text, string sub)
{
    string rev(26, ' ');
    for (int i = 0; i < 26; i++)
        rev[sub[i] - 'a'] = 'a' + i;
    string result = "";
    for (char c : text)
    {
        if (isalpha(c))
        {
            bool upper = isupper(c);
            c = tolower(c);
            result += upper ? toupper(rev[c - 'a']) : rev[c - 'a'];
        }
        else
            result += c;
    }
    return result;
}

// Polyalphabetic Cipher (Vigenère)
string vigenereEncrypt(string text, string key)
{
    string result = "";
    int j = 0;
    for (char c : text)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            result += (c - base + (tolower(key[j % key.size()]) - 'a')) % 26 + base;
            j++;
        }
        else
            result += c;
    }
    return result;
}
string vigenereDecrypt(string text, string key)
{
    string result = "";
    int j = 0;
    for (char c : text)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            result += (c - base - (tolower(key[j % key.size()]) - 'a') + 26) % 26 + base;
            j++;
        }
        else
            result += c;
    }
    return result;
}

// Rail Fence Cipher
string railFenceEncrypt(string text, int rails)
{
    if (rails <= 1)
        return text;
    vector<string> rail(rails);
    int dir = 1, row = 0;
    for (char c : text)
    {
        rail[row] += c;
        if (row == 0)
            dir = 1;
        else if (row == rails - 1)
            dir = -1;
        row += dir;
    }
    string res = "";
    for (auto &r : rail)
        res += r;
    return res;
}
string railFenceDecrypt(string text, int rails)
{
    if (rails <= 1)
        return text;
    vector<vector<int>> pos(rails);
    int dir = 1, row = 0;
    for (int i = 0; i < text.size(); i++)
    {
        pos[row].push_back(i);
        if (row == 0)
            dir = 1;
        else if (row == rails - 1)
            dir = -1;
        row += dir;
    }
    string res(text.size(), ' ');
    int idx = 0;
    for (int i = 0; i < rails; i++)
        for (int j : pos[i])
            res[j] = text[idx++];
    return res;
}

// Vernam Cipher
// Vernam Cipher (Hex Representation)
string vernamEncrypt(string text, string key)
{
    string result = "";
    for (int i = 0; i < text.size(); i++)
    {
        unsigned char c = text[i];
        unsigned char k = key[i % key.size()];
        unsigned char enc = c ^ k;
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << (int)enc;
        result += ss.str();  // append hex format
    }
    return result;
}

string vernamDecrypt(string hexText, string key)
{
    string result = "";
    for (size_t i = 0, j = 0; i < hexText.size(); i += 2, j++)
    {
        string hexPair = hexText.substr(i, 2);
        unsigned char enc = stoi(hexPair, nullptr, 16);
        unsigned char dec = enc ^ key[j % key.size()];
        result += dec;
    }
    return result;
}


int main()
{
    string text, key, encrypted, decrypted;
    string monoKey = "phqgiumeaylnofdxjkrcvstzwb";
    int choice;
    while (true)
    {
        cout << "\n--- Classical Encryption Techniques ---\n";
        cout << "1. Caesar Cipher\n2. Monoalphabetic Cipher\n3. Polyalphabetic (Vigenere)\n4. Rail Fence\n5. Vernam Cipher\n6. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 6)
            break;
        cout << "Enter text: ";
        getline(cin, text);

        switch (choice)
        {
        case 1:
        {
            int key;
            cout << "Enter key (1-25): ";
            cin >> key;
            cin.ignore();
            encrypted = caesarEncrypt(text, key);
            decrypted = caesarDecrypt(encrypted, key);
            break;
        }
        case 2:
            encrypted = monoEncrypt(text, monoKey);
            decrypted = monoDecrypt(encrypted, monoKey);
            break;
        case 3:
            cout << "Enter keyword: ";
            getline(cin, key);
            encrypted = vigenereEncrypt(text, key);
            decrypted = vigenereDecrypt(encrypted, key);
            break;
        case 4:
        {
            int rails;
            cout << "Enter number of rails: ";
            cin >> rails;
            cin.ignore();
            encrypted = railFenceEncrypt(text, rails);
            decrypted = railFenceDecrypt(encrypted, rails);
            break;
        }
        case 5:
            cout << "Enter key (same length or shorter): ";
            getline(cin, key);
            encrypted = vernamEncrypt(text, key);
            decrypted = vernamDecrypt(encrypted, key);
            break;
        default:
            cout << "Invalid choice!\n";
            continue;
        }

        cout << "Encrypted Text: " << encrypted << "\n";
        cout << "Decrypted Text: " << decrypted << "\n";
    }
    return 0;
}