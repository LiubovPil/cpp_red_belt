#include "Test_runner.h"
#include "Profiler.h"

#include <map>
#include <vector>
#include <string>
#include <future>
#include <iterator>
#include <algorithm>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (auto& [word, frequency] : other.word_frequences)
        {
            word_frequences[word] += frequency;
        }
    }
};

//create line for iteration
vector<string> Split(const string& line) {
    // http://en.cppreference.com/w/cpp/iterator/istream_iterator
    istringstream line_splitter(line);
    return { istream_iterator<string>(line_splitter), istream_iterator<string>() };
}

//create stats from each line
Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    for (const string& word : Split(line)) {
        if (key_words.count(word) > 0) {
            result.word_frequences[word]++;
        }
    }
    return result;
}

//create stats from batch - piece of data
Stats ExploreBatch(const set<string>& key_words, vector<string> lines) {
    Stats result;
    for (const string& line : lines) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    const size_t max_batch_size = 5000;
    
    vector<string> batch;
    batch.reserve(max_batch_size);

    vector<future<Stats>> futures;

    //Эта версия getline использует \n в качестве разделителя
    for(string line; getline(input, line); ) {
        batch.push_back(move(line));
        if (batch.size() >= max_batch_size) {
            //ref - передача константного объекта по ссылке
            futures.push_back(
                async(ExploreBatch, ref(key_words), move(batch))
            );
            batch.reserve(max_batch_size);
        }
    }
    
    Stats stats;
    if (!batch.empty()) {
        stats += ExploreBatch(key_words, move(batch));
    }
    for (auto& future: futures) {
        stats += future.get();
    }

    return stats;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    // key_words и input будут переданы по ссылке, а не скопированы
    //return async(ExploreKeyWordsSingleThread, ref(key_words), ref(input)).get();
//}

void TestBasic() {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
    ss << "Goondex is good, but yangle better\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
      {"yangle", 7},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

void TestBasicSingleThread() {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
    ss << "Goondex is good, but yangle better\n";

    const auto stats = ExploreKeyWordsSingleThread(key_words, ss);
    const map<string, int> expected = {
      {"yangle", 7},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}


int main() {
    TestRunner tr; 
    {
        LOG_DURATION("TestBasic");
        RUN_TEST(tr, TestBasic);
    }
    {
        LOG_DURATION("TestBasicSingleThread");
        RUN_TEST(tr, TestBasicSingleThread);
    }
}
