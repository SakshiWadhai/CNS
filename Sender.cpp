#include <bits/stdc++.h>
using namespace std;

bool isPrime(long long n)
{
    if (n < 2)
        return false;
    for (long long i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

long long gcd(long long a, long long b)
{
    return b == 0 ? a : gcd(b, a % b);
}

long long modExp(long long base, long long exp, long long mod)
{
    long long res = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

int main()
{
    long long p, q;
    cout << "Enter two prime numbers (p and q): ";
    cin >> p >> q;

    if (!isPrime(p) || !isPrime(q))
    {
        cout << "Error: Both numbers must be prime.\n";
        return 0;
    }

    auto startE = chrono::high_resolution_clock::now();

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e = 3;
    while (gcd(e, phi) != 1)
        e++;

    auto endE = chrono::high_resolution_clock::now();
    chrono::duration<double> keyTime = endE - startE;

    cout << "\nPublic Key (n, e): (" << n << ", " << e << ")\n";
    cout << "Time to generate key pair: " << keyTime.count() << " seconds\n";

    long long msg;
    cout << "\nEnter integer message: ";
    cin >> msg;

    if (msg >= n)
    {
        cout << "Error: Message must be smaller than n (" << n << ")\n";
        return 0;
    }

    auto startEnc = chrono::high_resolution_clock::now();
    long long cipher = modExp(msg, e, n);
    auto endEnc = chrono::high_resolution_clock::now();
    chrono::duration<double> encTime = endEnc - startEnc;

    cout << "\nEncrypted Message (cipher): " << cipher << "\n";
    cout << "Encryption time: " << encTime.count() << " seconds\n";

    ofstream fout("cipher.txt");
    fout << n << " " << e << "\n" << cipher;
    fout.close();

    cout << "\nSaved 'cipher.txt' for Receiver program.\n";
    cout << "Share (n, e) publicly but keep (p, q) private.\n";

    return 0;
}
