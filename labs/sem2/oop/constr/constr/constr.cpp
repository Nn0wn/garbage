// constr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <iostream>
#include <memory>
#include <algorithm> // std::copy
#include <cstddef> // size_t

template<typename T>
class Array
{
public:
	// (default) constructor
	Array(size_t size = 0)
		: m_size(size)
		, m_array(m_size ? new T[m_size]() : nullptr)
	{
		log("constructor");
		for (size_t i = 0; i < m_size; i++)
			m_array[i] = i;
	}

	~Array() 
	{
		log("destructor");
		//destroy(m_array); 
	}
	//copy constructor
	Array(const Array &arr)
		: m_size(arr.size())
		, m_array(m_size ? new T[m_size]() : nullptr)
	{
		log("copy constructor");
		for (size_t i = 0; i < m_size; i++)
		{
			m_array[i] = arr.m_array[i];
		}
		//std::copy(arr.m_array, arr.m_array[m_size], m_array);
	}
	//assignment opearator
	Array& operator = (Array arr)
	{
		log("copy assignment operator");
		std::swap(m_size, arr.m_size);
		std::swap(m_array, arr.m_array);
		return *this;
	}
	//move constructor
	Array(Array&& temp)
		:m_size(temp.size())
		,m_array(m_size ? new T[m_size]() : nullptr)
	{
		log("copy move operator");
		std::swap(m_array, temp.m_array);
	}
	

		const size_t size() const
	{
		return m_size;
	}

	T& operator [](const size_t index)
	{
		assert(index < m_size);

		return m_array[index];
	}

private:

	void log(const char* msg)
	{
		std::cout << "[" << this << "] " << msg << "\n";
	}

	size_t m_size;
	std::unique_ptr<T[]> m_array;
};


Array<size_t> Create(size_t size = 0)
{
	Array<size_t> ret(size);
	return ret;
}


int main()
{
	Array<size_t> num1(3);
	for (size_t i = 0; i < num1.size(); i++)
		std::cout << num1[i] << std::endl;
	Array<size_t> num2(num1);
	for (size_t i = 0; i < num2.size(); i++)
		std::cout << num2[i] << std::endl;
	Array <size_t> num3 = Create(5);
	for (size_t i = 0; i < num3.size(); i++)
		std::cout << num3[i] << std::endl;
	Array <size_t> num4;
	num4 = num3;
	for (size_t i = 0; i < num4.size(); i++)
		std::cout << num3[i] << std::endl;
	/*for (size_t i = 0; i < num3.size(); i++)
		std::cout << num3[i] << std::endl;*/
	system("pause");
    return 0;
}

