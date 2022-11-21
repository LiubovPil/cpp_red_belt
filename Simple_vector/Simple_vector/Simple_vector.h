#pragma once

#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

template <typename T>
class SimpleVector {
public:
	//old version
	//SimpleVector() {
	//	data = nullptr;
	//}
	SimpleVector() = default;

	//old version with copy
	//SimpleVector(const SimpleVector& other) 
	//	: data(new T[other.capacity]),
	//	size(other.size),
	//	capacity(other.capacity)
	//{
	//	copy(other.begin(), other.end(), begin());
	//}
	
	//delete version with copy
	//SimpleVector(const SimpleVector<T>& other) = delete;

	/*//version with move - we take datas from "other" object
	SimpleVector(SimpleVector<T>&& other) 
		: data(other.data),
		size(other.size),
		capacity(other.capacity) 
	{
		other.data = nullptr;
		other.size = other.capacity = 0;
	}*/

	explicit SimpleVector(size_t size):
	data(new T[size]),
	capacity(size),
	size(size){
	}

	~SimpleVector() {
		delete[] data;
	}

	T& operator[](size_t index) {
		return data[index];
	}

	/*//some ideas
	void operator=(const SimpleVector& other) {
	delete[] data;
	data = new T[other.capacity];
	size = other.size;
	capacity = other.capacity;
	copy(other.begin(), other.end(), begin());
	}*/

	/*//version with copy
	void operator=(const SimpleVector<T>& other) {
		if (this != &other) {
			if (other.size <= capacity) {
				size = other.size;
				copy(other.begin(), other.end(), begin());
			}
			else {   //идиома copy-and-swap.
				SimpleVector<T> tmp(other);
				swap(tmp.data, data);
				swap(tmp.size, size);
				swap(tmp.capacity, capacity);
			}
		}
	}*/

	//version with move
	void operator=(SimpleVector<T>&& other) {
		delete[] data;

		data = other.data;
		size = other.size;
		capacity = other.capacity;

		other.data = nullptr;
		other.size = other.capacity = 0;
	}

	T* begin() {
		return data;
	}
	T* end() {
		return data + size;
	}

	const T* begin() const {
		return data; 
	}

	const T* end() const {
		return data + size;
	}

	size_t Size() const {
		return size;
	}
	size_t Capacity() const {
		return capacity;
	}

	/*//version with copy
	void PushBack(const T& value) {
		if (size >= capacity){
			size_t new_capacity = capacity == 0 ? 1 : 2 * capacity;
			T* new_data = new T[new_capacity];
			copy(begin(), end(), new_data);
			delete[] data;
			data = new_data;
			capacity = new_capacity;
		}
		data[size++] = value;
	}*/

	//version with move>
	void PushBack(T value) {
		if (size >= capacity) {
			ExpandIfNeeded();
		}
		data[size++] = move(value);
	}

private:
	T* data;
	size_t capacity = 0;
	size_t size = 0;

	void ExpandIfNeeded() {
		auto new_cap = capacity == 0 ? 1 : 2 * capacity;
		auto new_data = new T[new_cap];
		// используем перемещение
		move(begin(), end(), new_data);
		delete[] data;
		data = new_data;
		capacity = new_cap;
	}
};

