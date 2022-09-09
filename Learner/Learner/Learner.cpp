// Tasks from the course "Fundamentals of C++: Red Belt
// Done by LiubovPil

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
private:
    vector<string> dict;
    set<string> dict_additional;

public:
    int Learn(const vector<string>& words) {
        int newWords = 0;
        for (const auto& word : words) {
            auto is_inserted = dict_additional.insert(word);
            if (is_inserted.second) {
                //if(dict_additional.find(word) == dict_additional.end())
                ++newWords;
                dict.push_back(word);
                //dict_additional.inser(word);
            }
        }
        return newWords;
    }

    vector<string> KnownWords() {
        sort(dict.begin(), dict.end());
        return dict;
        //return {dict_additional.begin(), dict_additional.end();}
    }
};

int main() {
    Learner learner;
    string line;
    while (getline(cin, line)) {
        vector<string> words;
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        cout << learner.Learn(words) << "\n";
    }
    cout << "=== known words ===\n";
    for (auto word : learner.KnownWords()) {
        cout << word << "\n";
    }
}
