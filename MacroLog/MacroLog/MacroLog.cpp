// Tasks from the course "Fundamentals of C++: Red Belt
// Done by Liubov Piliagina

#include "Test_Runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
	explicit Logger(ostream& output_stream) : os(output_stream) {
	}

	void SetLogLine(bool value) { log_line = value; }
	void SetLogFile(bool value) { log_file = value; }

	void SetFile(const string& value) { file = value; }
	void SetLine(int value) { line = value; }

	void Log(const string& message) {
		if (log_line && !log_file)
			os << "Line " << line << " ";
		else if (log_file && !log_line)
			os << file << " ";
		else if (log_line && log_file)
			os << file << ":" << line << " ";

			os << message << endl;
	}

private:
	ostream& os;
	bool log_line = false;
	bool log_file = false;

	string file = "";
	int line = 0;
};

#define LOG(logger, message)\
logger.SetFile(__FILE__);\
logger.SetLine(__LINE__);\
logger.Log(message);


void TestLog() {
	/* Для написания юнит-тестов в этой задаче нам нужно фиксировать конкретные
	номера строк в ожидаемом значении (см. переменную expected ниже). Если
	мы добавляем какой-то код выше функции TestLog, то эти номера строк меняются,
	и наш тест начинает падать � то неудобно.

	Чтобы этого избежать, мы используем специальный макрос #line
	(http://en.cppreference.com/w/cpp/preprocessor/line), который позволяет
	переопределить номер строки, а также имя файла. Благодаря ему, номера
	строк внутри функции TestLog будут фиксированы независимо от того, какой
	код мы добавляем перед ней*/
#line 1

	ostringstream logs;
	Logger l(logs);
	LOG(l, "hello");

	l.SetLogFile(true);
	LOG(l, "hello");

	l.SetLogLine(true);
	LOG(l, "hello");

	l.SetLogFile(false);
	LOG(l, "hello");

	string expected = "hello\n";
	expected += "C:\\Repos\\cpp_red_belt\\MacroLog\\MacroLog\\MacroLog.cpp hello\n";
	expected += "C:\\Repos\\cpp_red_belt\\MacroLog\\MacroLog\\MacroLog.cpp:10 hello\n";
	expected += "Line 13 hello\n";
	ASSERT_EQUAL(logs.str(), expected);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestLog);
}