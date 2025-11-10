#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
using namespace std;

string simple_hash(const string &s) {
    uint32_t h = 5381;
    for (unsigned char c : s) {
        h = ((h << 5) + h) + c;
    }
    stringstream ss;
    ss << hex << nouppercase << setw(8) << setfill('0') << (h & 0xFFFFFFFF);
    return ss.str();
}

pair<string,string> sender(const string &msg) {
    string digest = simple_hash(msg);
    return { msg, digest };
}

bool receiver_check(const string &msg, const string &sent_digest) {
    string my_digest = simple_hash(msg);
    cout << "[RECEIVER] Recomputed digest: " << my_digest << "\n";
    return my_digest == sent_digest;
}

int main() {
    cout << "Enter message to send: ";
    string message;
    getline(cin, message);

    auto sent = sender(message);
    cout << "\n[SENDER] Message: " << sent.first << "\n";
    cout << "[SENDER] Simple-hash: " << sent.second << "\n\n";

    
    cout << "[NETWORK] Delivering message (no tampering)...\n";
    bool ok = receiver_check(sent.first, sent.second);
    cout << (ok ? "Result: Hashes match : integrity OK.\n\n" : "Result: Hash mismatch!\n\n");

    
    cout << "Demonstrating tampering (flip first byte if exists)...\n";
    string tampered = sent.first;
    if (!tampered.empty()) tampered[0] ^= 0x01; 
    cout << "[NETWORK] Tampered message: " << tampered << "\n";
    bool ok2 = receiver_check(tampered, sent.second);
    cout << (ok2 ? "Result: Hashes match (unexpected).\n" : "Result: Hash mismatch : tampering detected.\n");

    return 0;
}
