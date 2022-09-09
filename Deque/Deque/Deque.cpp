// Tasks from the course "Fundamentals of C++: Red Belt
// Done by LiubovPil

#include "Test_runner.h"

#include <stdexcept>

template <typename T>
class Deque {
private:
	vector<T> deque_back;
	vector<T> deque_front;

public:
	Deque() = default;

	bool Empty() const{
		return deque_back.empty() && deque_front.empty();
	}

	size_t Size() const {
		return deque_back.size() + deque_front.size();
	}

	const T& operator[](size_t index) const {
		size_t size_df = deque_front.size();
		
		if (index < size_df)
			return deque_front[size_df - 1 - index];
		else
			return deque_back[deque_back.size() - (Size() - index)];
	}
	T& operator[](size_t index){
		size_t size_df = deque_front.size();
		
		if (index < size_df)
			return deque_front[size_df - 1 - index];
		else
			return deque_back[deque_back.size() - (Size() - index)];
	}
	const T& At(size_t index) const {
		if (index > Size())
			throw out_of_range("Out of range");  //separate method

		size_t size_df = deque_front.size();

		if (index < size_df)                        //can be replaced by return (*this)[i];
			return deque_front[size_df - 1 - index];
		else
			return deque_back[deque_back.size() - (Size() - index)];
	}
	T& At(size_t index){
		if (index > Size())
			throw out_of_range("Out of range");

		size_t size_df = deque_front.size();

		if (index < size_df)                             //can be replaced by return (*this)[i];
			return deque_front[size_df - 1 - index];
		else
			return deque_back[deque_back.size() - (Size() - index)];
	}
	const T& Front() const {
		if(deque_front.empty())
			return deque_back.front();
		else
			return deque_front.back();
	}
	T& Front() {
		if (deque_front.empty())
			return deque_back.front();
		else
			return deque_front.back();
	}
	const T& Back() const {
		if (deque_back.empty())
			return deque_front.front();
		else
			return deque_back.back();
	}
	T& Back() {
		if (deque_back.empty())
			return deque_front.front();
		else
			return deque_back.back();
	}
	void PushFront(T value) {
		deque_front.push_back(value);
	}
	void PushBack(T value) {
		deque_back.push_back(value);
	}
};
