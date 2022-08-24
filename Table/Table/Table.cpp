// Tasks from the course "Fundamentals of C++: Red Belt
// Done by Liubov Piliagina

#include "Test_runner.h"

#include<vector>

using namespace std;

template <typename T>
class Table {
private:
	vector<vector<T>> table;

public:
	Table(size_t row, size_t column) {
		Resize(row, column);
	}
	const vector<T>& operator [] (size_t row) const {
		return table[row];
	}
	vector<T>& operator [] (size_t row) {
		return table[row];
	}
	pair<size_t, size_t> Size() const {
		if (!table.empty() && !table[0].empty()) {
			return { table.size(), table[0].size() };
		}
		return { 0, 0 };
	}
	void Resize(size_t row, size_t column) {
		table.resize(row);
		for (auto& item : table) {
			item.resize(column);
		}
	}
};

void TestTable() {
	Table<int> t(1, 1);
	ASSERT_EQUAL(t.Size().first, 1u);
	ASSERT_EQUAL(t.Size().second, 1u);
	t[0][0] = 42;
	ASSERT_EQUAL(t[0][0], 42);
	t.Resize(3, 4);
	ASSERT_EQUAL(t.Size().first, 3u);
	ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestTable);
	return 0;
}
