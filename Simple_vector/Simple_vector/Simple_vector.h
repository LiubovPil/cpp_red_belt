#pragma once

#include <cstdlib>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector {
public:
	SimpleVector() {
		data = nullptr;
	}

	SimpleVector(const SimpleVector& other) 
		: data(new T[other.capacity]),
		size(other.size),
		capacity(other.capacity)
	{
		copy(other.begin(), other.end(), begin());
	}

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

	void operator=(const SimpleVector& other) {
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
	}

	/*void operator=(const SimpleVector& other) {
		delete[] data;
		data = new T[other.capacity];
		size = other.size;
		capacity = other.capacity;
		copy(other.begin(), other.end(), begin());
	}*/

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
	}

private:
	T* data;
	size_t capacity = 0;
	size_t size = 0;
};

