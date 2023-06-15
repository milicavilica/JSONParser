#pragma once
#include<iostream>

template <typename T>
class Vector
{
	T* data;
	size_t capacity;
	size_t size;
	void copyFrom(const Vector& other);
	void free();
	void moveFrom(Vector&& other);
	void resize(size_t cap);
public:
	Vector();
	Vector(size_t n);
	Vector(size_t n, const T& elem);
	Vector(const Vector& other);
	Vector(Vector&& other);
	Vector& operator=(const Vector& other);
	Vector& operator=(Vector&& other);
	void push_back(const T& elem);
	void pop_back();
	const T& operator[](size_t ind) const;
	T& operator[](size_t ind);
	bool empty() const;
	void setSize(size_t _size);
	void setCapacity(size_t _capacity);
	size_t getSize() const;
	size_t getCapacity() const;
	void clear();
	~Vector();
};

template <typename T>
void Vector<T>::copyFrom(const Vector& other)
{
	data = new T[other.capacity];
	size = other.size;
	capacity = other.capacity;
	for (size_t i = 0; i < size; i++)
	{
		data[i] = other.data[i];
	}
}
template <typename T>
void Vector<T>::free()
{
	delete[] data;
	data = nullptr;
	size = capacity = 0;
}
template <typename T>
void Vector<T>::moveFrom(Vector&& other)
{
	data = other.data;
	other.data = nullptr;
	size = other.size;
	capacity = other.capacity;
}
template <typename T>
void Vector<T>::resize(size_t cap)
{
	T* newData = new T[cap];
	for (size_t i = 0; i < capacity; i++)
	{
		newData[i] = data[i];
	}
	delete[] data;
	data = newData;
	capacity = cap;
}
template <typename T>
Vector<T>::Vector()
{
	data = new T[8];
	capacity = 8;
	size = 0;
}
template <typename T>
Vector<T>::Vector(size_t n)
{
	capacity = n;
	size = n;
	data = new T[n];
}
template <typename T>
Vector<T>::Vector(size_t n, const T& elem) : Vector(n)
{
	for (size_t i = 0; i < n; i++)
	{
		data[i] = elem;
	}
}
template <typename T>
Vector<T>::Vector(const Vector& other)
{
	copyFrom(other);
}
template <typename T>
Vector<T>::Vector(Vector&& other)
{
	moveFrom(std::move(other));
}
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}
template <typename T>
void Vector<T>::push_back(const T& elem)
{
	resize(2 * capacity);
	data[size++] = elem;
}
template <typename T>
void Vector<T>::pop_back()
{
	size--;
}
template <typename T>
const T& Vector<T>::operator[](size_t ind) const
{
	return data[ind];
}
template <typename T>
T& Vector<T>::operator[](size_t ind)
{
	return data[ind];
}
template <typename T>
bool Vector<T>::empty() const
{
	return size == 0;
}
template <typename T>
void Vector<T>::setSize(size_t _size)
{
	if (_size < 0)
		_size = 0;
	size = _size;
}
template <typename T>
void Vector<T>::setCapacity(size_t _capacity)
{
	if (_capacity < 0)
		_capacity = 0;
	capacity = _capacity;
}
template <typename T>
size_t Vector<T>::getSize() const
{
	return size;
}
template <typename T>
size_t Vector<T>::getCapacity() const
{
	return capacity;
}
template <typename T>
void Vector<T>::clear()
{
	size = 0;
}
template <typename T>
Vector<T>::~Vector()
{
	free();
}