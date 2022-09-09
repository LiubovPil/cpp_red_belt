// Tasks from the course "Fundamentals of C++: Red Belt
// Done by LiubovPil

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
        current_time(0) {}

    void BookRoom(long long time, string& hotel_name, int client_id, int room) {
        UpdateHotelsBase(hotel_name);
        hotels_base[hotel_name].push({ time , client_id , room });
        reserved_rooms[hotel_name] += room;
        guests[hotel_name][client_id]++;

        current_time = time;
    }
    int GetClientsCount(string& hotel_name) {
        UpdateHotelsBase(hotel_name);
        if (hotels_base[hotel_name].empty())
            return 0;
        return guests[hotel_name].size();
    }
    int GetRoomsCount(string& hotel_name) {
        UpdateHotelsBase(hotel_name);
        if (hotels_base[hotel_name].empty())
            return 0;
        return reserved_rooms[hotel_name];
    }
private:
    static const int SECONDS_IN_DAYS = 86400;

    map<string, queue<BookInfo>> hotels_base;
    map<string, map<int, int>> guests;
    map<string, int> reserved_rooms;

    long long current_time;

    void UpdateHotelsBase(string& hotel_name) {
        queue<BookInfo>& hotel_bookinfo = hotels_base[hotel_name];
        while (!hotel_bookinfo.empty() && 
            hotel_bookinfo.front().booking_time <= current_time - SECONDS_IN_DAYS) {
            reserved_rooms[hotel_name] -= hotel_bookinfo.front().room_count;

            int client = hotel_bookinfo.front().client;
            guests[hotel_name][client]--;
            if (guests[hotel_name][client] == 0)
                guests[hotel_name].erase(client);

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
