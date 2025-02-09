#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

int main() {
    // JSON file name
    const char* filename = "reviews.json";

    // Read the entire JSON file into a string
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Parse JSON with RapidJSON
    string jsonStr = buffer.str();
    Document doc;
    if (doc.Parse(jsonStr.c_str()).HasParseError()) {
        cerr << "Error parsing JSON!" << endl;
        return 1;
    }

    // Check if "paper" exists and is an array
    if (!doc.HasMember("paper") || !doc["paper"].IsArray()) {
        cerr << "Invalid JSON format!" << endl;
        return 1;
    }

    // Iterate through papers
    for (const auto& paper : doc["paper"].GetArray()) {
        cout << "-----------------------------------\n";
        cout << "Paper ID: " << paper["id"].GetInt() << endl;
        cout << "Preliminary Decision: " << paper["preliminary_decision"].GetString() << endl;

        // Check if "review" exists
        if (paper.HasMember("review") && paper["review"].IsArray()) {
            cout << "Reviews:\n";
            for (const auto& review : paper["review"].GetArray()) {
                cout << "  Review ID: " << review["id"].GetInt() << endl;
                cout << "  Evaluation: " << review["evaluation"].GetString() << endl;
                cout << "  Review Text: " << review["text"].GetString() << endl;
                cout << "  -----------------------------------\n";
            }
        }
    }

    return 0;
}
