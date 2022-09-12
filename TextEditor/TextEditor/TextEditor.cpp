// Tasks from the course "Fundamentals of C++: Red Belt
// Done by LiubovPil

#include "Test_runner.h"
#include <string>
#include <list>

using namespace std;

class Editor {
public:
    //Реализуйте конструктор по умолчанию и объявленные методы
    Editor() : pos(text.end()) {
    }
    void Left() { // сдвинуть курсор влево
        //pos = Advance(pos, -1);
        if (pos != text.begin())
            pos--;
    }
    void Right() {  // сдвинуть курсор вправо
        //pos = Advance(pos, 1);
        if (pos != text.end())
            pos++;
    }
    void Insert(char token) {  // вставить символ token
        text.insert(pos, token);
    }
    void Cut(size_t tokens = 1) {  // вырезать не более tokens символов, начиная с текущей позиции курсор
        size_t count = tokens;
        Iterator it = pos;
        buffer.clear();
        while (count > 0 && it != text.end()) {
           buffer.push_back(*it);
           it++;
           count--;
        }
        pos = text.erase(pos, it); 
        /*auto pos2 = Advance(pos, tokens);
        buffer.assign(pos, pos2);
        pos = text.erase(pos, pos2);*/
    }
    void Copy(size_t tokens = 1) { // cкопировать не более tokens символов, начиная с текущей позиции курсора
        size_t count = tokens;
        Iterator it = pos;
        buffer.clear();
        while (count > 0 && it != text.end()) {
            buffer.push_back(*it);
            it++;
            count--;
        }
        /*auto pos2 = Advance(pos, tokens);
        buffer.assign(pos, pos2);*/
    }
    void Paste() {  // вставить содержимое буфера в текущую позицию курсора
        text.insert(pos, buffer.begin(), buffer.end());
    }
    string GetText() const {  // получить текущее содержимое текстового редактора
        return { text.begin(), text.end() };
    }
private:
    using Iterator = list<char>::iterator;
    list<char> text;
    list<char> buffer;
    Iterator pos;

    Iterator Advance(Iterator it, int steps) const {
        while (steps > 0 && it != text.end()) {
            ++it;
            --steps;
        }
        while (steps < 0 && it != text.begin()) {
            --it;
            ++steps;
        }
        return it;
    }
};

void TypeText(Editor& editor, const string& text) {
    for (char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for (char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);

    /*Editor editor;
    const string text = "hello, world";
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3); // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();*/
    return 0;
}