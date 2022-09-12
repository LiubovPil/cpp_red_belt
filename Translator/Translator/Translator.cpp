// Tasks from the course "Fundamentals of C++: Red Belt
// Done by LiubovPil

#include "Test_runner.h"
#include <string>
#include <map>
#include <deque>

using namespace std;

class Translator {
public:
	void Add(string_view source, string_view target) {
		const string_view clone_source = GetClonedView(source);
		const string_view clone_target = GetClonedView(target);
		forward_dict[clone_source] = clone_target;
		backward_dict[clone_target] = clone_source;
	}
	string_view TranslateForward(string_view source) const {
		return Translate(forward_dict, source);
	}
	string_view TranslateBackward(string_view target) const {
		return Translate(backward_dict, target);
	}

private:
	string_view GetClonedView(string_view s) {
		for (const auto* map_ptr : { &forward_dict, &backward_dict }) {  //ищем указателей, так как содержит string_view указатели
			const auto it = map_ptr->find(s);
			if (it != map_ptr->end()) {
				return it->first;
			}
		}
		return data.emplace_back(s);
	}
	static string_view Translate(const map<string_view, string_view>& dict, string_view s) {
		if (const auto it = dict.find(s); it != dict.end()) {
			return it->second;
		}
		else {
			return {};
		}
	}
	map<string_view, string_view> forward_dict;
	map<string_view, string_view> backward_dict;
	deque<string> data;
};

void TestSimple() {
	Translator translator;
	translator.Add(string("okno"), string("casement"));
	translator.Add(string("stol"), string("desk"));
	translator.Add(string("okno"), string("window"));
	translator.Add(string("stol"), string("table"));

	ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
	ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSimple);
	return 0;
}
