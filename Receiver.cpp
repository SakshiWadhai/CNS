#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

long long modExp(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

// Extended Euclid to get modular inverse
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long modInverse(long long e, long long phi) {
    long long x, y;
    long long g = extgcd(e, phi, x, y);
    if (g != 1) return -1;
    x %= phi;
    if (x < 0) x += phi;
    return x;
}

int main() {
    ifstream fin("cipher.txt");
    if (!fin) {
        cout << "Error: 'cipher.txt' not found.\n";
        return 0;
    }

    long long n, e;
    fin >> n >> e;
    long long cipher;
    if (!(fin >> cipher)) {
        cout << "Error: cipher not found in file.\n";
        return 0;
    }
    fin.close();

    cout << "Received Public Key (n, e): (" << n << ", " << e << ")\n";
    cout << "Cipher: " << cipher << "\n";

    long long p, q;
    cout << "\nEnter p and q (same primes used by sender): ";
    cin >> p >> q;

    if (p * q != n) {
        cout << "Error: Product of p and q must equal n.\n";
        return 0;
    }

    long long phi = (p - 1) * (q - 1);

    auto startD = chrono::high_resolution_clock::now();
    long long d = modInverse(e, phi);
    auto endD = chrono::high_resolution_clock::now();
    chrono::duration<double> dKeyTime = endD - startD;

    if (d == -1) {
        cout << "Error: Modular inverse does not exist. (e, phi) not coprime.\n";
        return 0;
    }

    cout << "\nPrivate Key (d): " << d << "\n";
    cout << "Time to compute private key: " << dKeyTime.count() << " seconds\n";

    auto startDec = chrono::high_resolution_clock::now();
    long long message = modExp(cipher, d, n);
    auto endDec = chrono::high_resolution_clock::now();
    chrono::duration<double> decTime = endDec - startDec;

    cout << "\nDecrypted Integer Message: " << message << "\n";
    cout << "Decryption time: " << decTime.count() << " seconds\n";

    return 0;
}
