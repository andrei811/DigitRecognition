#pragma once
#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
#include <cstring>
#include <ctime>
#include <random>

#define ASSERT(condition, index) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition <<"  "<<index <<" @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; exit(0);} }

//#define FINDCOPY

namespace mine {

	template<class T>
	class vector {

	public:
		T* m_vect = nullptr;
		int m_size = 0;

		vector() :m_vect(nullptr), m_size(0) {}

		// builds a vector of length of len and filled with 0
		vector(int len)
		{
			if (m_vect)delete[] m_vect;

			m_vect = new T[len]();
			m_size = len;
		}

		// constructs a vector of length len and assigns to all elements val value
		vector(int len, const T& val)
		{
			if (m_vect)delete[] m_vect;
			m_vect = new T[len];
			m_size = len;
			for (int i = 0; i < m_size; i++)
				m_vect[i] = val;
		}

		// copy constructor - does nothing to v
		vector(const vector<T>& v)
		{
#ifdef FINDCOPY
			std::cout << "COPY! func: template<class T> mine::vector<T>::vector(const mine::vector<T> &v) \n";
#endif
			if (v.m_size > m_size)
			{
				if (m_vect)delete[] m_vect;
				m_vect = new T[v.m_size]();
			}

			m_size = v.m_size;

			for (int i = 0; i < m_size; i++)
				m_vect[i] = v.m_vect[i];
		}

		// move constructor - destroys v 
		vector(vector<T>&& v)
		{
			if (m_vect)delete[] m_vect;
			m_size = v.m_size;
			m_vect = v.m_vect;
			v.m_vect = nullptr;
			v.m_size = 0;
		}

		vector(const std::initializer_list<T>& li)
		{
			if (m_size < li.size())
			{
				if (m_vect)delete[] m_vect;
				m_vect = new T[li.size()];
			}
			m_size = (int)li.size();
			int k = 0;
			for (auto it = li.begin(); it != li.end(); it++)
				m_vect[k++] = *it;
		}

		// returns m_size
		int size()const
		{
			return m_size;
		}

		// resize the vector if the length is bigger than the current length
		void resize(int n)
		{
			if (m_size < n)
			{
				if (m_vect)delete[] m_vect;
				m_vect = new T[n]();
			}
			m_size = n;
		}

		// resizes and fills the array with val value
		void resize(int n, const T& val)
		{
			resize(n);

			for (int i = 0; i < m_size; i++)
				m_vect[i] = val;
		}

		// returns the last element of the array if exists, otherwise assertation fails
		T& back()
		{
			assert(m_size > 0);
			return m_vect[m_size - 1];
		}

		// returns the last element of the array if exists, otherwise assertation fails
		const T back()const
		{
			assert(m_size > 0);
			return m_vect[m_size - 1];
		}

		// decreases the size by one
		void pop_back()
		{
			if (m_size > 0)
				m_size--;
		}

		// copy the vector v into *this
		void operator=(const vector<T>& v)
		{
#ifdef FINDCOPY
			std::cout << "COPY! func: template<class T> void mine::vector<T>::operator=(const mine::vector<T> &v) \n";
#endif
			if (v.m_size > m_size)
			{
				if (m_vect)delete[] m_vect;
				m_vect = new T[v.m_size]();
			}
			m_size = v.m_size;

			for (int i = 0; i < m_size; i++)
				m_vect[i] = v.m_vect[i];
		}

		// moves the vector v into *this
		void operator=(vector<T>&& v)
		{
			if (m_vect)delete[] m_vect;
			m_vect = v.m_vect;
			m_size = v.m_size;
			v.m_vect = 0;
			v.m_size = 0;
		}

		// returns a reference to the index-th element
		T& operator[](int index)
		{
			assert(index >= 0 && index < m_size, m_size);
			return m_vect[index];
		}

		// returns a copy to the index-th element
		const T& operator[](int index) const
		{
			assert(index >= 0 && index < m_size);
			return m_vect[index];
		}

		// returns a pointer to the array of data 
		T* data()
		{
			return m_vect;
		}

		// frees memory
		~vector()
		{
			if (m_vect == nullptr)
				return;
			delete[] m_vect;
		}
	};

	template<typename T>
	void randomShuffle(vector<T>& v)
	{
		srand((unsigned int)time(0));
		T* Data = v.data();
		int Size = v.size(), pos;

		for (int i = 0; i < Size; i++)
		{
			pos = rand() % Size;

			if (pos == i)
				continue;

			swap(Data[i], Data[pos]);
		}
	}

};

