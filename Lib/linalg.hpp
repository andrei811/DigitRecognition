#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <cstring>
#include <ctime>
#include "mvector.hpp"

//#define FINDCOPY

template<class T>
class Matrix
{
protected:
	mine::vector<mine::vector<T>> m_Mat;

public:
	// default constructor
	Matrix() = default;

	// copy constructor
	Matrix(const Matrix& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> Matrix<T>::Matrix(const Matrix<T> &x)\n";
#endif
		m_Mat.resize(x.rows(), mine::vector<T>(x.cols()));

		for (int i = 0; i < x.rows(); i++)
			for (int j = 0; j < x.cols(); j++)
				m_Mat[i][j] = x.m_Mat[i][j];
	}

	// move constructor
	// Matrix(Matrix&& x)
	// {
	//     mine::swapVector<T>(m_Mat, x.m_Mat);
	// }

	// mine::vector<mine::vector<T>> copy constructor
	Matrix(const mine::vector<mine::vector<T>>& init) : m_Mat(init)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> Matrix<T>::Matrix(const mine::vector<mine::vector<T>> &init)\n";
#endif
	}

	// mine::vector<mine::vector<T>> move constructor
	Matrix(mine::vector<mine::vector<T>>&& init)
	{
		m_Mat = (mine::vector<mine::vector<T>>&&)init.m_vect;
	}

	// size constructor
	Matrix(int rows, int cols)
	{
		m_Mat = (mine::vector<mine::vector<T>>&&)mine::vector<mine::vector<T>>(rows, mine::vector<T>(cols));
	}

	// operator overloaded for accessing elements
	const mine::vector<T>& operator[](int index) const
	{
		assert(index >= 0 && index < m_Mat.size());
		return m_Mat[index];
	}

	// operator overloaded for accessing elements
	mine::vector<T>& operator[](int index)
	{
		assert(index >= 0 && index < m_Mat.size());
		return m_Mat[index];
	}

	// returns # of rows
	int rows() const
	{
		return (int)m_Mat.size();
	}

	// returns # of cols
	int cols()const
	{
		if (m_Mat.size() == 0)
			return 0;

		return m_Mat[0].size();
	}

	// assinging operator copy - does nothing to x
	void operator=(const Matrix<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> void Matrix<T>::operator=(const Matrix<T> &x)\n";
#endif
		m_Mat.resize(x.rows(), mine::vector<T>(x.cols()));
		for (int i = 0; i < x.rows(); i++)
			for (int j = 0; j < x.cols(); j++)
				m_Mat[i][j] = x.m_Mat[i][j];
	}

	Matrix operator*(const Matrix& b)const
	{
		assert(this->rows() != 0 && b.rows() != 0);
		assert(this->cols() != 0 && b.cols() != 0);
		assert(this->cols() == b.rows());

		Matrix<T> mat((int)this->rows(), (int)b.cols());
		T temp;

		for (int i = 0; i < rows(); i++)
			for (int j = 0; j < b.cols(); j++)
			{
				temp = 0;
				for (int k = 0; k < cols(); k++)
					temp += m_Mat[i][k] * b[k][j];
				mat[i][j] = temp;
			}
		return mat;
	}

	void operator*=(const Matrix& b)const
	{
		assert(this->rows() != 0 && b.rows() != 0);
		assert(this->cols() != 0 && b.cols() != 0);
		assert(this->cols() == b.rows());

		Matrix<T> mat((int)this->rows(), (int)b.cols());
		T temp;

		for (int i = 0; i < rows(); i++)
			for (int j = 0; j < b.cols(); j++)
			{
				temp = 0;
				for (int k = 0; k < cols(); k++)
					temp += (*this)[i][k] * b[k][j];
				mat[i][j] = temp;
			}
		m_Mat = (mine::vector<mine::vector<T>>&&) mat.m_Mat;
	}

	// scalar multiplication
	Matrix operator*(double nr)const
	{
		Matrix<T> mat((int)rows(), (int)cols());

		for (int i = 0; i < rows(); i++)
			for (int j = 0; j < cols(); j++)
				mat[i][j] = m_Mat[i][j] * nr;
		return mat;
	}

	void operator*=(double nr)
	{
		for (int i = 0; i < rows(); i++)
			for (int j = 0; j < cols(); j++)
				m_Mat[i][j] *= nr;
	}

	// matrix substraction
	Matrix operator-(const  Matrix& b)const
	{
		assert(this->rows() != 0 && b.rows() != 0);
		assert(this->cols() != 0 && b.cols() != 0);
		assert(this->rows() == b.rows() && this->cols() == b.cols());

		Matrix<T> mat(b.rows(), b.cols());

		for (int i = 0; i < b.rows(); i++)
			for (int j = 0; j < b.cols(); j++)
				mat[i][j] = m_Mat[i][j] - b[i][j];
		return mat;
	}

	// matrix substraction
	void operator-=(const Matrix& b)
	{
		assert(this->rows() != 0 && b.rows() != 0);
		assert(this->cols() != 0 && b.cols() != 0);
		assert(this->rows() == b.rows() && this->cols() == b.cols());

		for (int i = 0; i < b.rows(); i++)
			for (int j = 0; j < b.cols(); j++)
				m_Mat[i][j] -= b[i][j];
	}

	// matrix addition
	Matrix operator+(const Matrix& b)const
	{
		assert(this->rows() != 0 && b.rows() != 0);
		assert(this->cols() != 0 && b.cols() != 0);
		assert(this->rows() == b.rows() && this->cols() == b.cols());

		Matrix<T> mat(b.rows(), b.cols());

		for (int i = 0; i < b.rows(); i++)
			for (int j = 0; j < b.cols(); j++)
				mat[i][j] = m_Mat[i][j] + b[i][j];
		return mat;
	}

	// matrix addition
	void operator+=(const Matrix& b)
	{
		assert(this->rows() != 0 && b.rows() != 0);
		assert(this->cols() != 0 && b.cols() != 0);
		assert(this->rows() == b.rows() && this->cols() == b.cols());

		for (int i = 0; i < b.rows(); i++)
			for (int j = 0; j < b.cols(); j++)
				m_Mat[i][j] += b[i][j];
	}

	// print operator overloaded
	friend std::ostream& operator<<(std::ostream& s, const  Matrix& m)
	{
		if (m.rows() == 0 || m.cols() == 0)
		{
			s << "Empty Matrix\n";
			return s;
		}

		// bool highdim = (m.rows() > 1 && m.cols() > 1);

		// if ( highdim )
		s << "[";

		for (int i = 0; i < m.rows(); i++)
		{
			if (i != 0)
			{
				s << ",\n";

				// if ( highdim )
				s << " ";
			}
			s << "[ ";

			for (int j = 0; j < m.cols(); j++)
			{
				if (j != 0)
					s << ", ";

				s << m[i][j];
			}
			s << " ]";
		}

		// if ( highdim)
		s << "]";
		s << "\n";
		return s;
	}

	// set dimension 
	void setDim(int rows, int cols)
	{
		m_Mat.resize(rows, mine::vector<T>(cols));
	}

	// return the transposed matrix
	Matrix transpose()
	{
		Matrix<T> mat;

		mat.setDim(cols(), rows());

		for (int i = 0; i < rows(); i++)
			for (int j = 0; j < cols(); j++)
				mat.m_Mat[j][i] = m_Mat[i][j];
		return mat;
	}

	// fill the matrix with val value
	void fill(int val)
	{
		for (int i = 0; i < rows(); i++)
			for (int j = 0; j < cols(); j++)
				m_Mat[i][j] = val;
	}

};


template<class T>
class Vector
{
public:
	mine::vector<T> m_vect;

public:
	// default constructor
	Vector() = default;

	// creates a vector of len dimensions
	Vector(int len)
	{
		m_vect.resize(len);
	}

	// creates a vector and assigns val to all dimensions
	Vector(int len, const T& val)
	{
		m_vect.resize(len, val);
	}

	// copy constructor
	Vector(const Vector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> Vector<T>::Vector(const Vector<T> &x)\n";
#endif
		m_vect = x.m_vect;
	}

	// move constructor
	Vector(Vector<T>&& x)
	{
		m_vect = (mine::vector<T>&&) x.m_vect;
	}

	Vector(const Matrix<T>& x)
	{
		assert(x.cols() == 1 || x.rows() == 1);

		if (x.cols() == 1)
		{
			m_vect.resize(x.rows());

			for (int i = 0; i < m_vect.size(); i++)
				m_vect[i] = x[i][0];
		}
		else
		{
			m_vect.resize(x.cols());

			for (int i = 0; i < m_vect.size(); i++)
				m_vect[i] = x[0][i];
		}
	}

	Vector(const std::initializer_list<T>& li)
	{
		m_vect = mine::vector<T>(li);
	}

	Vector(const mine::vector<T>& v)
	{
		m_vect = v;
	}

	// operator overloaded for accessing elements
	const T& operator[](int index) const
	{
		assert(index >= 0 && index < m_vect.size());
		return m_vect[index];
	}

	// operator overloaded for accessing elements
	T& operator[](int index)
	{
		assert(index >= 0 && index < m_vect.size());
		return m_vect[index];
	}

	// returns # of rows
	int size() const
	{
		return (int)m_vect.size();
	}

	// assinging operator copy - does nothing to x
	void operator=(const Vector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> void Matrix<T>::operator=(const Matrix<T> &x)\n";
#endif

		m_vect = x.m_vect;
	}

	// assinging operator move - destroys x
	void operator=(Vector<T>&& x)
	{
		mine::swapVector<T>(m_vect, x.m_vect);
	}

	bool operator==(const Vector<T>& x)
	{
		assert(size() == x.size());

		for (int i = 0; i < size(); i++)
			if (m_vect[i] != x.m_vect[i])
				return 0;
		return 1;
	}

	Vector operator+(const Vector<T>& v)
	{
		assert(v.size() == size());
		Vector x(size());

		for (int i = 0; i < v.size(); i++)
			x[i] = v[i] + m_vect[i];
		return x;
	}

	Vector operator+(Vector<T>&& v)
	{
		assert(v.size() == size());

		for (int i = 0; i < v.size(); i++)
			v[i] += m_vect[i];
		return v;
	}

	// vector addition
	void operator+=(const Vector<T>& b)
	{
		assert(this->size() != 0 && b.size() != 0);

		for (int i = 0; i < b.size(); i++)
			this->m_vect[i] += b[i];
	}

	Vector operator-(const Vector<T>& v)
	{
		assert(v.size() == size());
		Vector x(size());

		for (int i = 0; i < v.size(); i++)
			x[i] = v[i] - m_vect[i];
		return x;
	}

	// vector substraction
	void operator-=(const Vector<T>& b)
	{
		assert(this->size() != 0 && b.size() != 0);

		for (int i = 0; i < b.size(); i++)
			this->m_vect[i] -= b[i];
	}

	Vector operator*(const T& val) const 
	{
		Vector<T> res(m_vect.size());

		for (int i = 0; i < size(); i++)
			res[i] = m_vect[i] * val;
		return res;
	}

	void operator*=(const T& val)
	{
		for (int i = 0; i < size(); i++)
			m_vect[i] *= val;
	}

	// print operator overloaded
	friend std::ostream& operator<<(std::ostream& s, const  Vector& m)
	{
		if (m.size() == 0)
		{
			s << "Empty Matrix\n";
			return s;
		}

		s << "[";

		for (int i = 0; i < m.size(); i++)
		{
			if (i != 0)
				s << ", ";
			s << m.m_vect[i];
		}

		s << "]\n";
		return s;
	}

	// set dimension 
	void setDim(int size)
	{
		m_vect.resize(size);
	}

	void setDim(int size, const T& val)
	{
		m_vect.resize(size, val);
	}

	// fill the vector with val value
	void fill(int val)
	{
		for (int i = 0; i < size(); i++)
			m_vect[i] = val;
	}

	~Vector()
	{
	}
};

template<class T>
class RowVector : public Vector<T>
{
public:
	// default constructor
	RowVector() = default;

	// creates a vector of len dimensions
	RowVector(int len)
	{
		this->m_vect.resize(len);
	}

	// creates a vector and assigns val to all dimensions
	RowVector(int len, const T& val)
	{
		this->m_vect.resize(len, val);
	}

	// copy constructors
	RowVector(const Vector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> ColumnVector<T>::ColumnVector(const Vector<T> &x)\n";
#endif
		this->m_vect = x.m_vect;
	}

	RowVector(const RowVector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> ColumnVector<T>::ColumnVector(const ColumnVector<T> &x)\n";
#endif
		this->m_vect = x.m_vect;
	}

	// move constructors
	RowVector(Vector<T>&& x)
	{
		this->m_vect = (mine::vector<T>&&) x.m_vect;
	}

	RowVector(RowVector<T>&& x)
	{
		this->m_vect = (mine::vector<T>&&) x.m_vect;
	}

	RowVector(const Matrix<T>& m)
	{
		assert(m.rows() == 1);

		this->setDim(m.cols());

		for (int i = 0; i < this->size(); i++)
			this->m_vect = m[0][i];
	}

	RowVector(Matrix<T>&& m)
	{
		assert(m.rows() == 1);
		this->m_vect = (mine::vector<T>&&) m[0];
	}

	// matrix multiplication

	// scalar multplication

	// RowVector multiplication 

	// ColumnVector Addition
	~RowVector()
	{
	}
};


template<class T>
class ColumnVector : public Vector<T>
{
public:
	// default constructor
	ColumnVector() = default;

	// creates a vector of len dimensions
	ColumnVector(int len)
	{
		this->m_vect.resize(len);
	}

	// creates a vector and assigns val to all dimensions
	ColumnVector(int len, const T& val)
	{
		this->m_vect.resize(len, val);
	}

	// copy constructors
	ColumnVector(const Vector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> ColumnVector<T>::ColumnVector(const Vector<T> &x)\n";
#endif
		this->m_vect = x.m_vect;
	}

	ColumnVector(const ColumnVector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> ColumnVector<T>::ColumnVector(const ColumnVector<T> &x)\n";
#endif
		this->m_vect = x.m_vect;
	}

	// move constructors
	ColumnVector(Vector<T>&& x)
	{
		this->m_vect = (mine::vector<T>&&) x.m_vect;
	}

	ColumnVector(ColumnVector<T>&& x)
	{
		this->m_vect = (mine::vector<T>&&) x.m_vect;
	}

	ColumnVector(const Matrix<T>& m)
	{
		assert(m.cols() == 1);

		this->setDim(m.rows());

		for (int i = 0; i < this->size(); i++)
			this->m_vect = m[i][0];
	}

	void operator=(ColumnVector<T>&& x)
	{
		this->m_vect = (mine::vector<T>&&)x.m_vect;
	}

	// assinging operator copy - does nothing to x
	void operator=(const ColumnVector<T>& x)
	{
#ifdef FINDCOPY
		std::cout << "COPY! funct: template<class T> void Matrix<T>::operator=(const Matrix<T> &x)\n";
#endif

		m_vect = x.m_vect;
	}

	Matrix<T> operator*(const RowVector<T>& v)
	{
		Matrix<T> mat(size(), v.size());
		for (int i = 0; i < size(); i++)
			for (int j = 0; j < v.size(); j++)
				mat[i][j] = this->m_vect[i] * v.m_vect[j];
		return mat;
	}

	~ColumnVector()
	{
	}
};

template<typename T>
ColumnVector<T> operator*(const Matrix<T>& m, const ColumnVector<T>& b)
{
	assert(m.rows() != 0 && b.size() != 0 && m.cols() != 0);
	assert(m.cols() == b.size());

	ColumnVector<T> mat((int)m.rows());
	T temp;

	for (int i = 0; i < m.rows(); i++)
	{
		temp = 0;
		for (int k = 0; k < m.cols(); k++)
			temp += m[i][k] * b[k];
		mat[i] = temp;
	}
	return mat;
}

template<typename T>
Vector<T> operator*(const RowVector<T>& a, const Matrix<T>& b)
{
	assert(a.size() != 0 && b.cols() != 0);
	assert(a.size() == b.rows());

	Vector<T> ans(b.cols());

	for (int i = 0; i < b.cols(); i++)
	{
		ans[i] = 0;
		for (int j = 0; j < b.rows(); j++)
			ans[i] += a[j] * b[j][i] ;
	}
		
	return ans;
}

template<typename T>
Vector<T> operator*(T a, Vector<T> b)
{
	Vector<T> x(b.size());

	for (int i = 0; i < b.size(); i++)
		x[i] = a * b[i];
	return x;
}

Vector<double> randVect(int size)
{
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<double> dis;
	gen = std::mt19937(rd());
	dis = std::uniform_real_distribution<double>(-1.0, 1.0);

	Vector<double> mat(size);

	for (int i = 0; i < size; i++)
		mat[i] = dis(gen);
	return mat;
}

template<typename T>
void fillvectMat(mine::vector<Matrix<T>>& v)
{
	for (int i = 0; i < v.size(); i++)
		v[i].fill(0);
}

template<typename T>
Matrix<T> operator*(double nr, const Matrix<T>& matrix)
{
	Matrix<T> mat((int)matrix.rows(), (int)matrix.cols());

	for (int i = 0; i < matrix.rows(); i++)
		for (int j = 0; j < matrix.cols(); j++)
			mat[i][j] = (T)(nr * matrix[i][j]);
	return mat;
}

template<typename T>
Matrix<T> hadamaradProduct(const Matrix<T>& a, const Matrix<T>& b)
{
	assert(a.rows() == b.rows() && a.cols() == b.cols());

	Matrix<T> mat;
	mat.setDim(a.rows(), a.cols());

	for (int i = 0; i < a.rows(); i++)
		for (int j = 0; j < a.cols(); j++)
			mat[i][j] = a[i][j] * b[i][j];
	return mat;
}

template<typename T>
ColumnVector<T> hadamaradProduct(const ColumnVector<T>& a, const ColumnVector<T>& b)
{
	assert(a.size() == b.size());

	ColumnVector<T> mat;
	mat.setDim(a.size());

	for (int i = 0; i < a.size(); i++)
			mat[i] = a[i] * b[i];
	return mat;
}


Matrix<double> randMat(int rows, int cols)
{
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<double> dis;
	gen = std::mt19937(rd());
	dis = std::uniform_real_distribution<double>(-1.0, 1.0);

	Matrix<double> mat(rows, cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			mat[i][j] = dis(gen);
	return mat;
}

template<typename T>
Matrix<T> identityMat(int n)
{
	Matrix<T> mat(n, n);

	for (int i = 0; i < n; i++)
		mat[i][i] = 1;

	return mat;
}

template<typename T>
ColumnVector<T> OneDim(int maxDim, int Dim)
{
	assert(Dim >= 0 && Dim < maxDim);
	ColumnVector<T> vect(maxDim);
	vect[Dim] = 1;
	return vect;
}

template<typename T>
void fillvectMat(mine::vector<ColumnVector<T>>& v)
{
	for (int i = 0; i < v.size(); i++)
		v[i].fill(0);
}


template<typename T>
void operator+=(mine::vector<T>& a, const mine::vector<T>& b)
{
	assert(a.size() == b.size());

	for (int i = 0; i < a.size(); i++)
		a[i] += b[i];
}

template<typename T>
void operator-=(mine::vector<T>& a, const mine::vector<T>& b)
{
	assert(a.size() == b.size());

	for (int i = 0; i < a.size(); i++)
		a[i] -= b[i];
}

template<typename T, typename U>
mine::vector<T> operator*(const U a, const mine::vector<T>& b)
{
	mine::vector<T> c(b.size());
	for (int i = 0; i < b.size(); i++)
		c[i] = a * b[i];
	return c;
}
