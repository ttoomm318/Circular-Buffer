#ifndef CIRCULAR_H
#define CIRCULAR_H

#include <algorithm>
#include <stdexcept>

// circular buffer class
template<class T>
class CircularBuffer
{
public:
	CircularBuffer(int capacity = 20);
	CircularBuffer(const CircularBuffer& other);
	CircularBuffer(CircularBuffer&& other);
	CircularBuffer& operator=(const CircularBuffer& other);
	CircularBuffer& operator=(CircularBuffer&& other);
	~CircularBuffer();
	void print_buffer();
	void push_back(const T& value); 
	T pop_front();
	void clear();
	bool is_empty() const;
	size_t capacity() const;
	size_t size() const;

private:
	T* buffer;
	size_t _size;
	size_t _capacity;
	int _front;
	int _back;
};

// initialize buffer with a fixed capacity, default is 20
template<class T> CircularBuffer<T>::CircularBuffer(int capacity)
: buffer(new T[capacity]), _capacity(capacity),
_size(0), _front(-1), _back(-1)
{
}

// copy constructor
template<class T> CircularBuffer<T>::CircularBuffer(const CircularBuffer& other)
: buffer(new T[other._capacity]), _capacity(other._capacity),
_size(other._size), _front(other._front), _back(other._back)
{
	for (int i = _front; i != _back; i = (i + 1) % _capacity)
		buffer[i] = other.buffer[i];
	buffer[_back] = other.buffer[_back];
}

// move constructor
template<class T> CircularBuffer<T>::CircularBuffer(CircularBuffer&& other)
: buffer(other.buffer), _capacity(other._capacity),
_size(other._size), _front(other._front), _back(other._back)
{
	other.buffer = nullptr;
}

// copy assignment operator
template<class T> CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer& other)
{
	if (&other != this) 
	{
		if (_capacity != other._capacity) 
		{
			_capacity = other._capacity;
			delete[] buffer;
			buffer = new T[other._capacity];
		}
		_size = other._size;
		_front = other._front;
		_back = other._back;
		for (int i = _front; i != _back; i = (i + 1) % _capacity)
			buffer[i] = other.buffer[i];
		buffer[_back] = other.buffer[_back];
	}
	return *this;
}

// move assignment operator
template<class T> CircularBuffer<T>& CircularBuffer<T>::operator=(CircularBuffer&& other)
{
	if (_capacity != other._capacity)
	{
		_capacity = other._capacity;
		delete[] buffer;
		buffer = other.buffer;
	}
	_size = other._size;
	_front = other._front;
	_back = other._back;
	other.buffer = nullptr;
	return *this;
}

// delete pointer to buffer
template<class T> CircularBuffer<T>::~CircularBuffer()
{
	delete[] buffer;
}

// print buffer information
template<class T> void CircularBuffer<T>::print_buffer()
{
	std::cout << "buffer size: " << _size << "/" << _capacity << std::endl;
	// skip detail information if buffer is empty
	if (_size == 0)
	{
		std::cout << "buffer is empty\n";
		return;
	}

	std::cout << "buffer content: \n";
	for (int i = _front; i != _back; i = (i + 1) % _capacity)
		std::cout << "buffer[" << i << "] = " << buffer[i] << std::endl;
	std::cout << "buffer[" << _back << "] = " << buffer[_back] << std::endl;
	std::cout << "front element: " << buffer[_front] << std::endl;
	std::cout << "back element: " << buffer[_back] << std::endl;
}

// add new data at the end
template<class T> void CircularBuffer<T>::push_back(const T& value)
{
	_back = (_back + 1) % _capacity;
	if (_front == -1 || _size == _capacity)
		_front = (_front + 1) % _capacity;
	buffer[_back] = value;
	if (_size < _capacity)
		_size++;
}

// return and remove the oldest data at the front
template<class T> T CircularBuffer<T>::pop_front()
{
	if (_size == 0)
		throw std::out_of_range("\nERROR: buffer is empty! no more item to pop.\n");
	T value = buffer[_front];
	buffer[_front] = T();
	if (--_size == 0)
		_front = _back = -1;
	else
	{
		_front = (_front + 1) % _capacity;
	}
	return value;
}

// clear the buffer and reset all fields 
template<class T> void CircularBuffer<T>::clear()
{
	for (int i = 0; i < _size;i++)
		buffer[i] = T();
	_size = 0;
	_front = _back = -1;
}

// check if the buffer is empty
template<class T> inline bool CircularBuffer<T>::is_empty() const
{
	return _size == 0;
}

// return buffer capacity 
template<class T> inline size_t CircularBuffer<T>::capacity() const
{
	return _capacity;
}

// return buffer size
template<class T> inline size_t CircularBuffer<T>::size() const
{
	return _size;
}


#endif