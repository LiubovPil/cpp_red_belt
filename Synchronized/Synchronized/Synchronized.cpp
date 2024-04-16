#include "Test_runner.h"

#include <numeric>
#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>

using namespace std;

// Реализуйте шаблон Synchronized<T>.
// Метод GetAccess должен возвращать структуру, в которой есть поле T& value.
template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(move(initial)) 
    {
    }

    struct Access {
        T& ref_to_value;
        lock_guard<mutex> guard;
    };

    Access GetAccess() {
        return {value, lock_guard(m)};
    }
private:
    T value;
    mutex m;
};

void TestConcurrentUpdate() {
    Synchronized<string> common_string;

    const size_t add_count = 50000;
    auto updater = [&common_string, add_count] {
        for (size_t i = 0; i < add_count; ++i) {
            auto access = common_string.GetAccess();
            access.ref_to_value += 'a';
        }
    };

    auto f1 = async(updater);
    auto f2 = async(updater);

    f1.get();
    f2.get();

    ASSERT_EQUAL(common_string.GetAccess().ref_to_value.size(), 2 * add_count);
}

vector<int> Consume(Synchronized<deque<int>>& common_queue) {
    vector<int> got;

    for (;;) {
        deque<int> q;

        {
            // Мы специально заключили эти две строчки в операторные скобки, чтобы
            // уменьшить размер критической секции. Поток-потребитель захватывает
            // мьютекс, перемещает всё содержимое общей очереди в свою
            // локальную переменную и отпускает мьютекс. После этого он обрабатывает
            // объекты в очереди за пределами критической секции, позволяя
            // потоку-производителю параллельно помещать в очередь новые объекты.
            //
            // Размер критической секции существенно влияет на быстродействие
            // многопоточных программ.
            auto access = common_queue.GetAccess();
            q = move(access.ref_to_value);
        }

        for (int item : q) {
            if (item > 0) {
                got.push_back(item);
            }
            else {
                return got;
            }
        }
    }
}

void TestProducerConsumer() {
    Synchronized<deque<int>> common_queue;

    auto consumer = async(Consume, ref(common_queue));

    const size_t item_count = 100000;
    for (size_t i = 1; i <= item_count; ++i) {
        common_queue.GetAccess().ref_to_value.push_back(i);
    }
    common_queue.GetAccess().ref_to_value.push_back(-1);

    vector<int> expected(item_count);
    iota(begin(expected), end(expected), 1);
    ASSERT_EQUAL(consumer.get(), expected);
}

double myPow(double x, int n) {
    if (n == 0)
        return 1.0;

    return myPow(x, n - 1) * x;
}

int helper(unordered_map<int, int>& dp, vector<int>& days, vector<int>& costs,
    int& minDayInWeek, int& minDayInMonth, size_t currDay) {
    if (currDay > days.size() - 1) {
        return 0;
    }

    if (dp[days[currDay]] != -1) {
        int value = dp[days[currDay]];
        return dp[days[currDay]];
    }

    int costDay = costs[0] + helper(dp, days, costs, minDayInWeek, minDayInMonth, currDay + 1);
    int costWeek = INT_MAX, costMonth = INT_MAX;
    if (currDay + minDayInWeek < days.size() && days[currDay + minDayInWeek] - days[currDay] < 7) {
        size_t j = minDayInWeek;
        while (currDay + j < days.size() && days[currDay + j] - days[currDay] < 7) {
            ++j;
        }
        costWeek = costs[1] + helper(dp, days, costs, minDayInWeek, minDayInMonth,
            currDay + j);
    }
    if (currDay + minDayInMonth < days.size() && days[currDay + minDayInMonth] - days[currDay] < 30) {
        size_t j = minDayInMonth;
        while (currDay + j < days.size() && days[currDay + j] - days[currDay] < 30 ) {
            ++j;
        }
        costMonth = costs[2] + helper(dp, days, costs, minDayInWeek, minDayInMonth, currDay + j);
    }

    return dp[days[currDay]] = min(costDay, min(costWeek, costMonth));
}

int mincostTickets(vector<int>& days, vector<int>& costs) {
    //int minDayInWeek = (costs[1] / costs[0]) + (costs[1] % costs[0] ? 1 : 0);
    //int minDayInMonth = (costs[2] / costs[0]) + (costs[2] % costs[2] ? 1 : 0);
    int minDayInWeek = (costs[1] / costs[0]);
    int minDayInMonth = (costs[2] / costs[0]);

    unordered_map<int, int> dp;

    for (int day : days) {
        dp[day] = -1;
    }

    return helper(dp, days, costs, minDayInWeek, minDayInMonth, 0);
}

/*void CheckBouquets(vector<int>& bouquets, priority_queue<int, vector<int>, greater<int>>& minHeap, priority_queue<int>& pq, int maxInterval, int minInterval, int count, int index) {
    if (pq.size() < count && index >= bouquets.size())
        return;
    else if (pq.size() >= count && index < bouquets.size()) {
        while (pq.size() > count)
            pq.pop();
        minHeap.push(pq.top());
    }

    pq.push(bouquets[index]);
    CheckBouquets(bouquets, minHeap, pq, maxInterval, minInterval, count, index + minInterval);
    CheckBouquets(bouquets, minHeap, pq, maxInterval, minInterval, count, index + maxInterval);
}*/

void CheckBouquets(vector<int>& bouquets, priority_queue<int, vector<int>, greater<int>>& minHeap,
    multiset<int>& ms, int maxInterval, int minInterval, int count, int index) {
    if (index >= bouquets.size()) {
        if (ms.size() >= count) {
            while (ms.size() > count)
                ms.erase(--ms.end());
            minHeap.push(*(--ms.end()));
        }
        return;
    }

    ms.insert(bouquets[index]);

    for (size_t i = minInterval; i <= maxInterval; ++i) {
        CheckBouquets(bouquets, minHeap, ms, maxInterval, minInterval, count, index + i);
        if (i == maxInterval)
            ms.extract(bouquets[index]);
    }
}
int minDays(vector<int>& bloomDay, int m, int k) {
    if (bloomDay.size() < m * k)
        return -1;

    vector<int> bouquets;
    for (size_t i = 0; i <= (bloomDay.size() - k); ++i) {
        int count = 1;
        int maxDay = bloomDay[i];
        while (count < k) {
            maxDay = max(maxDay, bloomDay[i + count]);
            ++count;
        }
        bouquets.push_back(maxDay);
    }

    if (m == bouquets.size())
    {
        int maxDay = bouquets[0];
        for (size_t i = 0; i < bouquets.size(); ++i) {
            maxDay = max(maxDay, bouquets[i]);
        }
        return maxDay;
    }

    int maxInterval = bouquets.size() - (k * (m - 2));
    if (maxInterval <= k || maxInterval > bouquets.size())
        maxInterval = k + 1;
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (size_t i = 0; i <= maxInterval; ++i) {
        multiset<int> ms;
        CheckBouquets(bouquets, minHeap, ms, maxInterval, k, m, i);
    }

    if (!minHeap.empty())
        return minHeap.top();
    else
        return -1;
}

vector<vector<char>> digitsInLetter = { {'a', 'b', 'c'}, {'d', 'e', 'f'}, {'g', 'h', 'i'},
{'j', 'k', 'l'}, {'m', 'n', 'o'}, {'p', 'q', 'r', 's'}, {'t', 'u', 'v'}, {'w', 'x', 'y', 'z'} };

void AddingLetter(vector<string>& result, string& combination, string digits, int current) {
    if (current >= digits.size()) {
        result.push_back(combination);
        return;
    }
    vector<char> temp = digitsInLetter[digits[current] - '2'];
    for (char ch : temp) {
        combination.push_back(ch);
        AddingLetter(result, combination, digits, ++current);
        combination.pop_back();
    }
}

    vector<string> letterCombinations(string digits) {
        if (digits.empty())
            return {};

        vector<string> result;
        string combination;
        AddingLetter(result, combination, digits, 0);
        return result;
    }

int main() {
    //vector<int> days = { 1,4,6,9,10,11,12,13,14,15,16,17,18,20,21,22,23,27,28 };
    //vector<int> costs = { 3,13,45 };
    //vector<int> days = { 1,2,3,4,5,6,7,8,9,10,30,31 };
    //vector<int> costs = { 2,7,15 };
    //vector<int> days = { 3,5,6,8,9,10,11,12,13,14,15,16,20,21,23,25,26,27,29,30,33,34,35,36,38,39,40,42,45,46,47,48,49,51,53,54,56,
    //    57,58,59,60,61,63,64,67,68,69,70,72,74,77,78,79,80,81,82,83,84,85,88,91,92,93,96 };
    //vector<int> costs = { 3,17,57 };
    //cout << "Good!" << mincostTickets(days, costs);
    //vector<int> blooms = { 7,7,7,7,12,7,7 };
    //vector<int> blooms = { 1,10,3,10,2 };   // m = 3, k = 1
    //vector<int> blooms = { 1,10,3,10,2 };
    //vector<int> blooms = { 5,37,55,92,22,52,31,62,99,64,92,53,34,84,93,50,28 }; //m = 8, k = 2
    //vector<int> blooms = { 81, 23, 10, 90, 68, 43, 81, 10, 92, 65, 47, 57, 51, 74, 61, 79, 18, 52, 74, 90}; //m = 2, k = 7
    //vector<int> blooms = { 15, 85, 48, 88, 93, 72, 32, 19, 89, 100, 22, 5, 29, 76, 2, 17, 77, 70, 14, 64, 25,
    //    71, 23, 43, 31, 9, 2, 79, 97, 30, 41, 48, 25, 16 }; //m = 11, k = 3 
    //vector<int> blooms = { 1000000000,1000000000 }; //m = 1, k = 1 
    //vector<int> blooms = { 40, 74, 42, 94, 90, 9, 29, 45, 32, 35, 42, 71, 73, 47, 49, 83, 72, 64, 66, 100, 
    //    31, 35, 23, 24, 96, 9, 71, 37, 95, 26, 25, 54, 65, 45, 92, 88, 38, 80 }; //m = 2, k = 13 
    //vector<int> blooms = { 79,30,15,94,80,52,14,4,81,62,40,47,44,98,73,63,36,1,66,83,66,36,35,64,45,21,76,4,79,36,57,20,86,15,19}; //m = 35, k = 1 
    //vector<int> blooms = { 38, 56, 29, 87, 54, 97, 42, 18, 44, 25, 57, 100, 97, 88, 39, 87, 95, 9, 84, 9, 60, 74,
    //    37, 45, 92, 9, 36, 95, 26, 74, 45, 23, 47, 63, 89, 24, 19, 74, 77, 77, 30, 2, 99}; //m = 21, k = 2 
    //cout << "Good!" << minDays(blooms, 21, 2);
    vector<string> str = letterCombinations("23");
    return 0;
}