#include "Test_runner.h"

#include <algorithm>
#include <deque>
#include <map>
#include <string>
#include <vector>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename String>
using Key = String;

template <typename String>
Key<String> ComputeStringKey(const String& string) {
	String chars = string;
	sort(begin(chars), end(chars));
	chars.erase(unique(begin(chars), end(chars)), end(chars));
	return chars;
}

//разбить задачу на две части - посмотреть, какие буквы есть в слове, и собрать итоговый вектор
template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
	//удобный способ сгруппировать имеющиеся слова по буквам, содержащимся в них
	//ключи = слова из букв слова(без повторений)
	map<Key<String>, Group<String>> groups_map;
	for (String& string : strings) {
		groups_map[ComputeStringKey(string)].push_back(move(string));
	}

	//создаем итоговый вектор из Group - перемещаем из словаря по ключям целую Group(отбрасываем ключи)
	vector<Group<String>> groups;
	for (auto& [key, group] : groups_map) {
		groups.push_back(move(group));
	}
	return groups;
}



void TestGroupingABC() {
	vector<string> strings = { "caab", "abc", "cccc", "bacc", "c" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 2);
	sort(begin(groups), end(groups));  // Порядок групп не имеет значения
	ASSERT_EQUAL(groups[0], vector<string>({ "caab", "abc", "bacc" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "cccc", "c" }));
}

void TestGroupingReal() {
	vector<string> strings = { "law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 4);
	sort(begin(groups), end(groups));  // Порядок групп не имеет значения
	ASSERT_EQUAL(groups[0], vector<string>({ "laptop", "paloalto" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "law", "wall", "awl" }));
	ASSERT_EQUAL(groups[2], vector<string>({ "port" }));
	ASSERT_EQUAL(groups[3], vector<string>({ "top", "pot" }));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestGroupingABC);
	RUN_TEST(tr, TestGroupingReal);
	return 0;
}