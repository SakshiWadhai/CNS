#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// ✅ Check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

// ✅ GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// ✅ Modular inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

// ✅ Fast modular exponentiation
long long powerMod(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            res = (res * base) % mod;
        exp /= 2;
        base = (base * base) % mod;
    }
    return res;
}

// ✅ Simple hash (numeric messages only)
long long simpleHash(string msg) {
    long long hash = 0;
    for (char c : msg)
        hash = (hash * 31 + (c - '0')) % 100000; // numeric hash
    return hash;
}

// ✅ RSA encryption (encrypt number directly)
long long rsaEncryptNum(long long message, int e, int n) {
    return powerMod(message, e, n);
}

// ✅ RSA decryption (decrypt number directly)
long long rsaDecryptNum(long long cipher, int d, int n) {
    return powerMod(cipher, d, n);
}

int main() {
    srand(time(0));

    cout << "Generating RSA key pair...\n";

    int p = 1, q = 1;
    while (!isPrime(p)) p = rand() % 100 + 50;
    while (!isPrime(q) || p == q) q = rand() % 100 + 50;

    int n = p * q;
    int phi = (p - 1) * (q - 1);

    int e = rand() % (phi - 2) + 2;
    while (gcd(e, phi) != 1)
        e = rand() % (phi - 2) + 2;

    int d = modInverse(e, phi);

    cout << "\nRSA Key Pair Generated:\n";
    cout << "Public Key (e, n): (" << e << ", " << n << ")\n";
    cout << "Private Key (d, n): (" << d << ", " << n << ")\n";

    // 🔸 Numeric input
    string msgStr;
    cout << "\nEnter a numeric message (digits only): ";
    cin >> msgStr;

    // Convert message string to number
    long long message = stoll(msgStr);

    if (message >= n) {
        cout << "\nError: Message must be smaller than n (" << n << ").\n";
        return 0;
    }

    // Step 1: Encrypt message using receiver's public key
    long long encrypted = rsaEncryptNum(message, e, n);

    // Step 2: Create hash & signature using sender’s private key
    long long hashVal = simpleHash(msgStr);
    long long signature = rsaEncryptNum(hashVal, d, n);

    cout << "\n🔹 Encrypted Message: " << encrypted;
    cout << "\n🔹 Digital Signature: " << signature << endl;

    // Step 3: Receiver decrypts message using private key
    long long decrypted = rsaDecryptNum(encrypted, d, n);

    // Step 4: Receiver verifies signature using sender’s public key
    long long decryptedHash = rsaDecryptNum(signature, e, n);
    long long receiverHash = simpleHash(to_string(decrypted));

    cout << "\nDecrypted Message: " << decrypted;
    cout << "\nHash from Signature: " << decryptedHash;
    cout << "\nReceiver's Hash: " << receiverHash << endl;

    if (decryptedHash == receiverHash)
        cout << "\nSignature is valid. Message integrity verified.\n";
    else
        cout << "\nInvalid Signature. Message may have been tampered with.\n";

    return 0;
}
