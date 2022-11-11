#include "Test_runner.h"

#include <vector>
#include <algorithm>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end =
        adjacent_find(tokens_begin, tokens_end,
            [](const auto& left_token, const auto& right_token) {
                return left_token.IsEndSentencePunctuation()
                    && !right_token.IsEndSentencePunctuation();
            });
    return before_sentence_end == tokens_end
        ? tokens_end
        : next(before_sentence_end);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    vector<Sentence<Token>> sentences;

    auto it_begin = begin(tokens);
    const auto it_end = end(tokens);

    while (it_begin != it_end) {
        auto it_sentence = FindSentenceEnd(it_begin, it_end);
        Sentence<Token> sentence;
        for (; it_begin != it_end; ++it_begin) {
            sentence.push_back(move(*it_end));
        }
        sentences.push_back(move(sentence));
    }
    return sentences;
}


struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}
// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({
            {"Split"}, {"into"}, {"sentences"}, {"!"}
            })),
        vector<Sentence<TestToken>>({ {
            {"Split"}, {"into"}, {"sentences"}, {"!"}
        } })
    );

    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({
            {"Split"}, {"into"}, {"sentences"}, {"!", true}
            })),
        vector<Sentence<TestToken>>({ {
            {"Split"}, {"into"}, {"sentences"}, {"!", true}
        } })
    );

    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({
          {"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true},
          {"Without"}, {"copies"}, {".", true}
            })),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
            {{"Without"}, {"copies"}, {".", true}},
            })
            );
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}