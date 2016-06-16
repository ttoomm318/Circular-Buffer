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
#include <string>

// Test integer buffer
void test_int_buffer() 
{
	std::cout << "Integer buffer of default size 20: \n";
	CircularBuffer<int>* buffer = new CircularBuffer<int>();
	buffer->print_buffer();

	std::cout << "\n----add 8 elements----\n";
	for (int i = 0; i < 8; ++i)
		buffer->push_back(i * 3 + 1);
	buffer->print_buffer();

	try
	{
		std::cout << "\n----retrieve/pop first elements----\n";
		int temp = buffer->pop_front();
		std::cout << "retrieved element: " << temp << "\n";
		buffer->print_buffer();

		std::cout << "\n----buffer created with copy constructor----\n";
		CircularBuffer<int> buffer_ctor = *buffer;
		buffer_ctor.print_buffer();
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what();
	}

	std::cout << "\n----add 15 elements----\n";
	for (int i = 0; i < 15; ++i)
		buffer->push_back(i * 2 + 1);
	buffer->print_buffer();

	std::cout << "\n----buffer assigned with copy assignment operator----\n";
	CircularBuffer<int> buffer_copy;
	buffer_copy = *buffer;
	buffer_copy.print_buffer();

	std::cout << "\n----clear the buffer----\n";
	buffer->clear();
	buffer->print_buffer();

	delete buffer;
}

// Test string buffer
void test_string_buffer()
{
	std::cout << "\nString buffer of size 10: \n";
	CircularBuffer<std::string>* buffer = new CircularBuffer<std::string>(10);
	buffer->print_buffer();

	std::cout << "\n----add 6 elements----\n";
	for (int i = 0; i < 6; ++i)
		buffer->push_back("item " + std::to_string(i));
	buffer->print_buffer();

	std::cout << "\n----add 7 elements----\n";
	for (int i = 0; i < 7; ++i)
		buffer->push_back("item " + std::to_string(i+10));
	buffer->print_buffer();

	std::cout << "\n----buffer created with move constructor----\n";
	CircularBuffer<std::string> buffer_ctor = *buffer;
	CircularBuffer<std::string> buffer_mtor = std::move(buffer_ctor);
	buffer_mtor.print_buffer();

	try
	{
		std::cout << "\n----retrieve/pop first 3 elements----\n";
		for (int i = 0; i < 3; ++i) {
			std::string temp = buffer->pop_front();
			std::cout << "retrieved element " << i + 1 << ": " << temp << "\n";
		}
		buffer->print_buffer();

		std::cout << "\n----buffer assigned with move assignment operator----\n";
		CircularBuffer<std::string> buffer_move;
		//buffer_move = [buffer](){ return *buffer; }();
		//buffer_move.print_buffer();
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what();
	}

	std::cout << "\n----clear the buffer----\n";
	buffer->clear();
	buffer->print_buffer();

	delete buffer;
}

// Test entry
void main() {
	std::cout << "*Demostrating Circular Buffer*\n\n";

	test_int_buffer();
	test_string_buffer();
}