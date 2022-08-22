// Tasks from the course "Fundamentals of C++: Red Belt
// Done by Liubov Piliagina

#include <string>
#include <vector>
using namespace std;

#define UNIQ_ID_HELPER(line) name_with_num_##line
#define UNIQ_ID_HELPER_PREV(line) UNIQ_ID_HELPER(line)
#define UNIQ_ID UNIQ_ID_HELPER_PREV(__LINE__)// Р РµР°Р»РёР·СѓР№С‚Рµ СЌС‚РѕС‚ РјР°РєСЂРѕСЃ С‚Р°Рє, С‡С‚РѕР±С‹ С„СѓРЅРєС†РёСЏ main РєРѕРјРїРёР»РёСЂРѕРІР°Р»Р°СЃСЊ

int main() {
	int UNIQ_ID = 0;
	string UNIQ_ID = "hello";
	vector<string> UNIQ_ID = { "hello", "world" };
	vector<int> UNIQ_ID = { 1, 2, 3, 4 };
}

