// Tasks from the course "Fundamentals of C++: Red Belt
// Done by Liubov Piliagina

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include <queue>
#include <map>
#include <string>

using namespace std;
using namespace chrono;

struct BookInfo {
    long long booking_time;
    int client;
    int room_count;
};

class HotelManager {
public:
    HotelManager(): 
        start_time(steady_clock::now()),
        current_time(0) {}

    void BookRoom(long long time, string& hotel_name, int client_id, int room) {
        UpdateHotelsBase(hotel_name);
        hotels_base[hotel_name].push({ time , client_id , room });

        current_time = time;
    }
    int GetClientsCount(string& hotel_name) {
        UpdateHotelsBase(hotel_name);
        if (hotels_base[hotel_name].empty())
            return 0;
        return hotels_base[hotel_name].size();
    }
    int GetRoomsCount(string& hotel_name) {
        UpdateHotelsBase(hotel_name);
        queue<BookInfo> hotel_bookinfo = hotels_base[hotel_name];
        int rooms = 0;
        if (!hotel_bookinfo.empty()) {
            while (hotel_bookinfo.size() != 0) {
                rooms += hotel_bookinfo.front().room_count;
                hotel_bookinfo.pop();
            }
        }
        return rooms;
    }
private:
    map<string, queue<BookInfo>> hotels_base;

    steady_clock::time_point start_time;
    long long current_time;

    void UpdateHotelsBase(string& hotel_name) {
        queue<BookInfo>& hotel_bookinfo = hotels_base[hotel_name];
        if (!hotel_bookinfo.empty()) {
            while (hotel_bookinfo.front().booking_time <= current_time - 86400)
                hotel_bookinfo.pop();
        }
    }
};

int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio, а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            long long time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id;
            cin >> client_id;
            int room;
            cin >> room;
            manager.BookRoom(time, hotel_name, client_id, room);
        }
        else if (query_type == "CLIENTS") {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.GetClientsCount(hotel_name) << "\n";
        }
        else if (query_type == "ROOMS") {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.GetRoomsCount(hotel_name) << "\n";
        }
    }

    return 0;
}
