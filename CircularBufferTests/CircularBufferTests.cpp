/*
* Circular Buffer of Generic Value-type
* Copyright (c) 2016
*
* Arthur: Chung-Chen (Thomas) Hsieh
* Email: ttoomm318@gmail.com
*
*/
#include "CppUnitTest.h"
#include "CircularBuffer.h"
#include <sstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CircularBufferTests
{
	template<class T>
	class CircularBufferStub : public CircularBuffer<T>
	{
	public:
		explicit CircularBufferStub(size_t capacity = 20) : CircularBuffer(capacity) {}
		CircularBufferStub(const CircularBufferStub& other) : CircularBuffer(other) 
		{
			copy_constructed_ = true;
		}
		CircularBufferStub(CircularBufferStub&& other) : CircularBuffer(other) 
		{
			move_constructed_ = true;
		}
		CircularBufferStub& operator=(CircularBufferStub other) 
		{
			assigned_ = true;
			return (CircularBufferStub&)CircularBuffer::operator=(other);
		}

		T& operator[](size_t index) { return buffer_[index]; }
		const T& operator[](size_t index) const { return buffer_[index]; }
		bool is_copy_constructed() const { return copy_constructed_; }
		bool is_move_constructed() const { return move_constructed_; }
		bool is_assigned() const { return assigned_; }
	private:
		bool copy_constructed_ = false;
		bool move_constructed_ = false;
		bool assigned_ = false;
	};

	CircularBufferStub<std::string> MoveSemanticsStub()
	{
		CircularBufferStub<std::string> buffer(5);
		std::string str = "test";
		std::string another_str = "another";
		buffer.push_back(str);
		buffer.push_back(another_str);
		buffer.push_back(str);
		buffer.push_back(another_str);
		buffer.push_back(str);
		buffer.push_back(another_str);
		return buffer;
	}

	TEST_CLASS(CircularBufferTests)
	{
	public:

		TEST_METHOD(Constructor_RaisesInvalidArgumentException_ForZeroCapacity)
		{
			// Arrange


			// Act
			try
			{
				CircularBuffer<double> buffer(0);
				Assert::Fail(L"No exception for zero capacity buffer creation.", LINE_INFO());
			}
			catch (std::invalid_argument)
			{
			}
			catch (...)
			{
				Assert::Fail(L"Wrong exception for zero capacity buffer creation.", LINE_INFO());
			}

			// Assert
		}

		TEST_METHOD(Constructor_CreatesEmptyBufferOfExpectedLength_WithSpecifiedCapacity)
		{
			// Arrange


			// Act
			CircularBuffer<std::string> buffer(10);

			// Assert
			Assert::IsTrue(buffer.capacity() == 10);
			Assert::IsTrue(buffer.size() == 0);
			Assert::IsTrue(buffer.is_empty());
		}

		TEST_METHOD(Constructor_CreatesEmptyBufferOfDefaultLength_WithoutSpecifiedCapacity)
		{
			// Arrange


			// Act
			CircularBuffer<int> buffer;

			// Assert
			Assert::IsTrue(buffer.capacity() == 20);
			Assert::IsTrue(buffer.size() == 0);
			Assert::IsTrue(buffer.is_empty());
		}

		TEST_METHOD(CopyConstructor_CreatesDeepCopyOfSpecifiedBuffer_OfLValaueReference)
		{
			// Arrange
			CircularBufferStub<int> source_buffer;
			source_buffer.push_back(5);
			source_buffer.push_back(6);
			source_buffer.push_back(7);

			// Act
			CircularBufferStub<int> buffer(source_buffer);

			// Assert
			Assert::IsTrue(buffer.capacity() == 20);
			Assert::IsTrue(buffer.size() == 3);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == 5);
			Assert::IsTrue(buffer[1] == 6);
			Assert::IsTrue(buffer[2] == 7);
			Assert::IsTrue(buffer.is_copy_constructed());
		}

		TEST_METHOD(MoveConstructor_CreatesDeepCopyOfSpecifiedBuffer_OfRValaueReference)
		{
			// Arrange
			std::string str = "test";
			std::string another_str = "another";

			// Act
			CircularBufferStub<std::string> buffer(MoveSemanticsStub());

			// Assert
			Assert::IsTrue(buffer.capacity() == 5);
			Assert::IsTrue(buffer.size() == 5);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == another_str);
			Assert::IsTrue(buffer[1] == another_str);
			Assert::IsTrue(buffer[2] == str);
			Assert::IsTrue(buffer[3] == another_str);
			Assert::IsTrue(buffer[4] == str);
			Assert::IsTrue(buffer.is_move_constructed());
		}

		TEST_METHOD(AssignmentOperator_PerformsDeepCopyOfSpecifiedBuffer_OfLValaueReference)
		{
			// Arrange
			CircularBufferStub<int> source_buffer;
			source_buffer.push_back(5);
			source_buffer.push_back(6);
			source_buffer.push_back(7);
			CircularBufferStub<int> buffer;

			// Act
			buffer = source_buffer;

			// Assert
			Assert::IsTrue(buffer.capacity() == 20);
			Assert::IsTrue(buffer.size() == 3);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == 5);
			Assert::IsTrue(buffer[1] == 6);
			Assert::IsTrue(buffer[2] == 7);
			Assert::IsTrue(buffer.is_assigned());
		}

		TEST_METHOD(AssignmentOperator_PerformsDeepCopyOfSpecifiedBuffer_OfRValaueReference)
		{
			// Arrange
			std::string str = "test";
			std::string another_str = "another";
			CircularBufferStub<std::string> buffer;

			// Act
			buffer = std::move(MoveSemanticsStub());

			// Assert
			Assert::IsTrue(buffer.capacity() == 5);
			Assert::IsTrue(buffer.size() == 5);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == another_str);
			Assert::IsTrue(buffer[1] == another_str);
			Assert::IsTrue(buffer[2] == str);
			Assert::IsTrue(buffer[3] == another_str);
			Assert::IsTrue(buffer[4] == str);
			Assert::IsTrue(buffer.is_assigned());
		}

		TEST_METHOD(PopFront_RemovesExpectedElementFromBufferAndReturnsIt)
		{
			// Arrange
			CircularBufferStub<std::string> buffer;
			std::string str = "test string";
			std::string dumpStr = "dump";
			buffer.push_back(str);
			buffer.push_back(dumpStr);
			buffer.push_back(dumpStr);
			buffer.push_back(dumpStr);

			// Act
			std::string ret = buffer.pop_front();

			// Assert
			Assert::IsTrue(buffer.capacity() == 20);
			Assert::IsTrue(buffer.size() == 3);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == "");
			Assert::IsTrue(buffer[1] == dumpStr);
			Assert::IsTrue(buffer[2] == dumpStr);
			Assert::IsTrue(buffer[3] == dumpStr);
			Assert::AreEqual(ret, str);
		}

		TEST_METHOD(PopFront_RaisesOutOfRangeException_ForEmptyBuffer)
		{
			// Arrange
			CircularBuffer<std::string> buffer(10);

			// Act
			try
			{
				std::string str = buffer.pop_front();
				Assert::Fail(L"No exception for an empty pop.", LINE_INFO());
			}
			catch (std::out_of_range)
			{
			}
			catch (...)
			{
				Assert::Fail(L"Wrong exception for an empty pop.", LINE_INFO());
			}

			// Assert
		}

		TEST_METHOD(PushBack_InsertsElementsAtExpectedPositions_ForElementCountWithinCapacity)
		{
			// Arrange
			CircularBufferStub<std::string> buffer(10);
			std::string str0 = "test string 0";
			std::string str1 = "test string 1";
			std::string str2 = "test string 2";

			// Act
			buffer.push_back(str0);
			buffer.push_back(str1);
			buffer.push_back(str2);

			// Assert
			Assert::IsTrue(buffer.capacity() == 10);
			Assert::IsTrue(buffer.size() == 3);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == str0);
			Assert::IsTrue(buffer[1] == str1);
			Assert::IsTrue(buffer[2] == str2);
		}

		TEST_METHOD(PushBack_OverwritesExistingElements_ForElementCountExceedingCapacity)
		{
			// Arrange
			CircularBufferStub<int> buffer(5);

			// Act
			buffer.push_back(0);
			buffer.push_back(1);
			buffer.push_back(2);
			buffer.push_back(3);
			buffer.push_back(4);
			buffer.push_back(5);
			buffer.push_back(6);

			// Assert
			Assert::IsTrue(buffer.capacity() == 5);
			Assert::IsTrue(buffer.size() == 5);
			Assert::IsFalse(buffer.is_empty());
			Assert::IsTrue(buffer[0] == 5);
			Assert::IsTrue(buffer[1] == 6);
			Assert::IsTrue(buffer[2] == 2);
			Assert::IsTrue(buffer[3] == 3);
			Assert::IsTrue(buffer[4] == 4);
		}

		TEST_METHOD(Clear_ResetsAllElementsToDefaultValues)
		{
			// Arrange
			CircularBufferStub<int> buffer(5);
			buffer.push_back(0);
			buffer.push_back(1);
			buffer.push_back(2);
			buffer.push_back(3);
			buffer.push_back(4);
			buffer.push_back(5);
			buffer.push_back(6);

			// Act
			buffer.clear();

			// Assert
			Assert::IsTrue(buffer.capacity() == 5);
			Assert::IsTrue(buffer.size() == 0);
			Assert::IsTrue(buffer.is_empty());
			Assert::IsTrue(buffer[0] == 0);
			Assert::IsTrue(buffer[1] == 0);
			Assert::IsTrue(buffer[2] == 0);
			Assert::IsTrue(buffer[3] == 0);
			Assert::IsTrue(buffer[4] == 0);
		}
	};
}