#include <iostream>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

using namespace std;

class DataRedundancySystem {
private:
    vector<string> database;
    unordered_set<string> hashSet;

    // Simple hash function (for demonstration)
    string generateHash(const string &data) {
        hash<string> hasher;
        return to_string(hasher(data));
    }

    // Similarity check
    double similarity(string str1, string str2) {
        transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
        transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

        stringstream ss1(str1), ss2(str2);
        unordered_set<string> set1, set2;
        string word;

        while (ss1 >> word) set1.insert(word);
        while (ss2 >> word) set2.insert(word);

        int intersection = 0;
        for (auto &w : set1) {
            if (set2.count(w)) intersection++;
        }

        return (double)intersection / max((int)set1.size(), 1);
    }

public:
    string validateAndInsert(string newData) {

        string newHash = generateHash(newData);

        // Duplicate check
        if (hashSet.count(newHash)) {
            return "Duplicate Data - Rejected";
        }

        // Similarity check
        for (auto &record : database) {
            if (similarity(newData, record) > 0.8) {
                return "Possible False Positive - Review Required";
            }
        }

        // Insert unique data
        database.push_back(newData);
        hashSet.insert(newHash);

        return "Data Inserted Successfully";
    }

    void displayDatabase() {
        cout << "\nStored Data in Database:\n";
        for (auto &data : database) {
            cout << "- " << data << endl;
        }
    }
};

int main() {
    DataRedundancySystem system;
    string input;

    cout << "=== Data Redundancy Removal System ===\n";
    cout << "Type 'exit' to stop\n\n";

    while (true) {
        cout << "Enter data: ";
        getline(cin, input);

        if (input == "exit") {
            break;
        }

        cout << system.validateAndInsert(input) << endl;
    }

    system.displayDatabase();

    return 0;
}