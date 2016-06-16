/*
* Circular Buffer of Generic Value-type
* Copyright (c) 2015
*
* Arthur: Chung-Chen (Thomas) Hsieh
* Email: ttoomm318@gmail.com
*
*/
#include "CircularBuffer.h"
#include <iostream>
#include <stdexcept>
#include <string>

// initialize buffer with a fixed capacity, default is 20
template<class T> CircularBuffer<T>::CircularBuffer(size_t capacity)
: buffer_(new T[capacity]), capacity_(capacity),
size_(0), front_(-1), back_(-1)
{
}

// copy constructor
template<class T> CircularBuffer<T>::CircularBuffer(const CircularBuffer& other)
: buffer_(new T[other.capacity_]), capacity_(other.capacity_),
size_(other.size_), front_(other.front_), back_(other.back_)
{
	for (int i = front_; i != back_; i = (i + 1) % capacity_)
		buffer_[i] = other.buffer_[i];
	buffer_[back_] = other.buffer_[back_];
}

// move constructor
template<class T> CircularBuffer<T>::CircularBuffer(CircularBuffer&& other)
: CircularBuffer()
{
	swap(*this, other);
}

// copy/move assignment operator
template<class T> CircularBuffer<T>& CircularBuffer<T>::operator=(CircularBuffer other)
{
	swap(*this, other);
	return *this;
}

// delete pointer to buffer
template<class T> CircularBuffer<T>::~CircularBuffer()
{
	delete[] buffer_;
}

// print buffer information, e.g. for diagnostic purposes
template<class T> void CircularBuffer<T>::print_buffer()
{
	std::cout << "buffer size: " << size_ << "/" << capacity_ << std::endl;
	// skip detail information if buffer is empty
	if (size_ == 0)
	{
		std::cout << "buffer is empty\n";
		return;
	}

	std::cout << "buffer content: \n";
	for (int i = front_; i != back_; i = (i + 1) % capacity_)
		std::cout << "buffer[" << i << "] = " << buffer_[i] << std::endl;
	std::cout << "buffer[" << back_ << "] = " << buffer_[back_] << std::endl;
	std::cout << "front element: " << buffer_[front_] << std::endl;
	std::cout << "back element: " << buffer_[back_] << std::endl;
}

// add new data at the end
template<class T> void CircularBuffer<T>::push_back(const T& value)
{
	back_ = (back_ + 1) % capacity_;
	if (front_ == -1 || size_ == capacity_)
		front_ = (front_ + 1) % capacity_;
	buffer_[back_] = value;
	if (size_ < capacity_)
		++size_;
}

// return and remove the oldest data at the front
template<class T> T CircularBuffer<T>::pop_front()
{
	if (size_ == 0)
		throw std::out_of_range("\nERROR: buffer is empty! no more item to pop.\n");
	T value = buffer_[front_];
	buffer_[front_] = T();
	if (--size_ == 0)
		front_ = back_ = -1;
	else
	{
		front_ = (front_ + 1) % capacity_;
	}
	return value;
}

// clear the buffer and reset all fields 
template<class T> void CircularBuffer<T>::clear()
{
	for (size_t i = 0; i < size_; ++i)
		buffer_[i] = T();
	size_ = 0;
	front_ = back_ = -1;
}

template class CircularBuffer<char>;
template class CircularBuffer<signed char>;
template class CircularBuffer<unsigned char>;
template class CircularBuffer<short>;
template class CircularBuffer<unsigned short>;
template class CircularBuffer<int>;
template class CircularBuffer<unsigned int>;
template class CircularBuffer<long>;
template class CircularBuffer<unsigned long>;
template class CircularBuffer<long long>;
template class CircularBuffer<unsigned long long>;
template class CircularBuffer<float>;
template class CircularBuffer<double>;
template class CircularBuffer<long double>;
template class CircularBuffer<std::string>;