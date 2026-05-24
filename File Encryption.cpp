// ============================================================
//  CodeAlpha Internship -- Task 2: File Encryption/Decryption
//  Author : [Your Name]
//  Date   : 2026
//  Desc   : Encrypts/decrypts text files using Caesar Cipher
//           + XOR layer for extra security
// ============================================================

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// CipherEngine Class
class CipherEngine {
private:
    int  caesarShift;
    char xorKey;

    char caesarEncryptChar(char c, int shift) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            return (char)((c - base + shift + 26) % 26 + base);
        }
        return c;
    }

    char caesarDecryptChar(char c, int shift) {
        return caesarEncryptChar(c, 26 - (shift % 26));
    }

    char xorChar(char c) {
        return c ^ xorKey;
    }

public:
    CipherEngine(int shift, char key)
        : caesarShift(shift % 26), xorKey(key) {}

    string encryptText(const string& text) {
        string result = text;
        for (char& c : result) {
            c = caesarEncryptChar(c, caesarShift);
            c = xorChar(c);
        }
        return result;
    }

    string decryptText(const string& text) {
        string result = text;
        for (char& c : result) {
            c = xorChar(c);
            c = caesarDecryptChar(c, caesarShift);
        }
        return result;
    }
};

// FileProcessor Class
class FileProcessor {
public:
    static bool readFile(const string& path, string& content) {
        ifstream file(path, ios::binary);
        if (!file.is_open()) {
            cerr << "\n  [ERROR] Cannot open file: " << path << "\n";
            return false;
        }
        content.assign((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
        file.close();
        return true;
    }

    static bool writeFile(const string& path, const string& content) {
        ofstream file(path, ios::binary);
        if (!file.is_open()) {
            cerr << "\n  [ERROR] Cannot write to file: " << path << "\n";
            return false;
        }
        file << content;
        file.close();
        return true;
    }
};

void getSettings(int& shift, char& xorKey) {
    cout << "\n  -- Cipher Settings ------------------\n";
    cout << "  Caesar shift (1-25, e.g. 3): ";
    cin  >> shift;
    shift = max(1, min(25, shift));
    cout << "  XOR key character (e.g. K) : ";
    cin  >> xorKey;
    cout << "  -------------------------------------\n";
}

void encryptFile() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string inputPath, outputPath;
    int shift; char xorKey;

    cout << "\n  Input file path  : ";
    getline(cin, inputPath);
    cout << "  Output file path : ";
    getline(cin, outputPath);
    if (outputPath.empty()) outputPath = "encrypted.txt";

    cin.clear();
    getSettings(shift, xorKey);

    string content;
    if (!FileProcessor::readFile(inputPath, content)) return;

    CipherEngine engine(shift, xorKey);
    string encrypted = engine.encryptText(content);
    string header = "ENCRYPTED|SHIFT=" + to_string(shift) +
                    "|XOR=" + string(1, xorKey) + "\n";

    if (FileProcessor::writeFile(outputPath, header + encrypted)) {
        cout << "\n  File encrypted successfully!\n";
        cout << "  Output: " << outputPath << "\n";
        cout << "  IMPORTANT: Remember shift (" << shift
             << ") and XOR key (" << xorKey << ") to decrypt!\n";
    }
}

void decryptFile() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string inputPath, outputPath;
    int shift; char xorKey;

    cout << "\n  Encrypted file path : ";
    getline(cin, inputPath);
    cout << "  Output file path    : ";
    getline(cin, outputPath);
    if (outputPath.empty()) outputPath = "decrypted.txt";

    cin.clear();
    getSettings(shift, xorKey);

    string content;
    if (!FileProcessor::readFile(inputPath, content)) return;

    size_t headerEnd = content.find('\n');
    string body = content;
    if (headerEnd != string::npos &&
        content.substr(0, 9) == "ENCRYPTED") {
        body = content.substr(headerEnd + 1);
    }

    CipherEngine engine(shift, xorKey);
    string decrypted = engine.decryptText(body);

    if (FileProcessor::writeFile(outputPath, decrypted)) {
        cout << "\n  File decrypted successfully!\n";
        cout << "  Output: " << outputPath << "\n";
    }
}

void previewFile() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string path;
    cout << "\n  File path to preview: ";
    getline(cin, path);

    string content;
    if (!FileProcessor::readFile(path, content)) return;

    cout << "\n  -- Preview (first 300 chars) --------\n";
    cout << content.substr(0, min((size_t)300, content.size()));
    if (content.size() > 300) cout << "\n  ... (truncated)";
    cout << "\n  -------------------------------------\n";
    cout << "  Total size: " << content.size() << " bytes\n";
}

void showMenu() {
    cout << "\n  ==================================\n";
    cout << "    FILE ENCRYPTION / DECRYPTION\n";
    cout << "    CodeAlpha Internship Task 2\n";
    cout << "  ==================================\n";
    cout << "  1. Encrypt a file\n";
    cout << "  2. Decrypt a file\n";
    cout << "  3. Preview file contents\n";
    cout << "  0. Exit\n";
    cout << "  ==================================\n";
    cout << "  Your choice: ";
}

int main() {
    cout << "\n  Algorithm: Caesar Cipher + XOR layer\n";
    cout << "  Works on any plain text file (.txt, .csv, .md, etc.)\n";

    int choice;
    do {
        showMenu();
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Enter a valid number: ";
        }
        switch (choice) {
            case 1: encryptFile(); break;
            case 2: decryptFile(); break;
            case 3: previewFile(); break;
            case 0: cout << "\n  Goodbye!\n\n"; break;
            default: cout << "  Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}