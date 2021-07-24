#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <cstring>

namespace mine{

template<class T>
class vector{

    private:
    T* m_vect = nullptr;
    int m_size = 0;
    public:
    vector(int* a, int n)
    {
        m_vect = a;
        m_size = n;
        a = nullptr;
    }

    vector(const vector<T>& v)
    {
        if ( v.m_size > m_size )
            m_vect = new T[v.m_size];
        m_size = v.m_size;

        for ( int i = 0; i < m_size; i++ )
            m_vect[i] = v.m_vect[i];
    }

    vector(vector<T>&& v)
    {
        m_size = v.m_size;
        m_vect = v.m_vect;
        v.m_vect = 0;
    }

    vector(std::vector<T> v)
    {
        if ( v.size() > m_size )
            m_vect = new T[v.size()];

        m_size = v.size();
        for ( int i = 0; i < v.size(); i++ )
            m_vect[i] = v[i];
    }

    vector(int n, T val)
    {
        m_vect = new T[n];
        m_size = n;
        for ( int i = 0; i < m_size; i++ )
            m_vect[i] = val;
    }

    int size()const
    {
        return m_size;
    }

    void operator=(const vector<T>& v)
    {
        if ( v.m_size > m_size )
            m_vect = new T[v.m_size];
        m_size = v.m_size;

        for ( int i = 0; i < m_size; i++ )
            m_vect[i] = v.m_vect[i];
    }

    void operator=(vector<T>&& v)
    {
        m_size = v.m_size;
        m_vect = v.m_vect;
        v.m_vect = 0;
    }

    T& operator[](int index)
    {
        assert(index >= 0 && index < m_size);
        return m_vect[index];
    }

    const T operator[](int index) const
    {
        assert(index >= 0 && index < m_size);
        return m_vect[index];
    }

    ~vector()
    {
        delete m_vect;
    }
};

};


template<class T>
class Matrix
{
    protected:
    std::vector<mine::vector<T>> m_Mat;
    public:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

    public:
    void initRandGen(){ gen = std::mt19937(rd()); dis = std::uniform_real_distribution<double>(0.0, 1.0); }
    Matrix(){ initRandGen(); };
    Matrix(const Matrix& x)
    {
        m_Mat.resize(x.rows(), mine::vector<T>(x.cols(), 0));
        for ( int i = 0; i < x.rows(); i++ )
            for ( int j = 0; j < x.cols(); j++ )
                m_Mat[i][j] = x[i][j];
    }

    Matrix(const std::vector<mine::vector<T>>& init): m_Mat(init){ initRandGen(); };
    Matrix(int rows, int cols){ m_Mat = std::vector<mine::vector<T>>(rows, mine::vector<T>(cols, 0)); initRandGen(); }

    Matrix(Matrix&& x)
    {
        swap(m_Mat, x.m_Mat);
    }

    const mine::vector<T>& operator[](int index) const
    {
        assert(index >= 0 && index < m_Mat.size());
        return m_Mat[index];
    }

    mine::vector<T>& operator[](int index)
    {
        assert(index >= 0 && index < m_Mat.size());
        return m_Mat[index];
    }

    int rows() const
    {
        return ( int )m_Mat.size();
    }

    int cols()const
    {
        if ( m_Mat.size() == 0 )
            return 0;

        return m_Mat[0].size();
    }

    void operator=(const Matrix<T>& x)
    {
        m_Mat.resize(x.rows(), mine::vector<T>(x.cols(), 0));
        for ( int i = 0; i < x.rows(); i++ )
            for ( int j = 0; j < x.cols(); j++ )
                m_Mat[i][j] = x[i][j];
    }

    Matrix operator*(const Matrix& b)const
    {
        // cout << this->cols() << "   " << b.rows() << endl;
        assert(this->rows() != 0 && b.rows() != 0);
        assert(this->cols() != 0 && b.cols() != 0);
        assert(this->cols() == b.rows());

        Matrix<T> mat(( int )this->rows(), ( int )b.cols());
        T temp;

        for ( int i = 0; i < rows(); i++ )
            for ( int j = 0; j < b.cols(); j++ )
            {
                temp = 0;
                for ( int k = 0; k < cols(); k++ )
                    temp += (*this)[i][k] * b[k][j];
                mat[i][j] = temp;
            }
        return mat;
    }

    Matrix operator*(double nr)const
    {
        Matrix<T> mat(( int )rows(), ( int )cols());

        for ( int i = 0; i < rows(); i++ )
            for ( int j = 0; j < cols(); j++ )
                mat[i][j] = m_Mat[i][j] * nr;
        return mat;
    }

    Matrix operator-(const  Matrix& b)const
    {
        assert(this->rows() != 0 && b.rows() != 0);
        assert(this->cols() != 0 && b.cols() != 0);
        assert(this->rows() == b.rows() && this->cols() == b.cols());

        Matrix<T> mat(b.rows(), b.cols());

        for ( int i = 0; i < b.rows(); i++ )
            for ( int j = 0; j < b.cols(); j++ )
                mat[i][j] = (*this)[i][j] - b[i][j];
        return mat;
    }

    void operator-=(const Matrix& b)
    {
        assert(this->rows() != 0 && b.rows() != 0);
        assert(this->cols() != 0 && b.cols() != 0);
        assert(this->rows() == b.rows() && this->cols() == b.cols());

        for ( int i = 0; i < b.rows(); i++ )
            for ( int j = 0; j < b.cols(); j++ )
                this->m_Mat[i][j] -= b[i][j];
    }

    Matrix operator+(const Matrix& b)const
    {
        assert(this->rows() != 0 && b.rows() != 0);
        assert(this->cols() != 0 && b.cols() != 0);
        assert(this->rows() == b.rows() && this->cols() == b.cols());

        Matrix<T> mat(b.rows(), b.cols());

        for ( int i = 0; i < b.rows(); i++ )
            for ( int j = 0; j < b.cols(); j++ )
                mat[i][j] = this->m_Mat[i][j] + b[i][j];
        return mat;
    }

    void operator+=(const Matrix& b)
    {
        assert(this->rows() != 0 && b.rows() != 0);
        assert(this->cols() != 0 && b.cols() != 0);
        assert(this->rows() == b.rows() && this->cols() == b.cols());

        for ( int i = 0; i < b.rows(); i++ )
            for ( int j = 0; j < b.cols(); j++ )
                this->m_Mat[i][j] += b[i][j];
    }

    friend std::ostream& operator<<(std::ostream& s, const  Matrix& m)
    {
        if ( m.rows() == 0 || m.cols() == 0 )
        {
            s << "Empty Matrix\n";
            return s;
        }

        // bool highdim = (m.rows() > 1 && m.cols() > 1);

        // if ( highdim )
        s << "[";

        for ( int i = 0; i < m.rows(); i++ )
        {
            if ( i != 0 )
            {
                s << ",\n";

                // if ( highdim )
                s << " ";
            }
            s << "[ ";

            for ( int j = 0; j < m.cols(); j++ )
            {
                if ( j != 0 )
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

    void setDim(int rows, int cols)
    {
        m_Mat.resize(rows, mine::vector<T>(cols, 0));
    }

    Matrix transpose()
    {
        Matrix<T> mat;

        mat.setDim(cols(), rows());

        for ( int i = 0; i < rows(); i++ )
            for ( int j = 0; j < cols(); j++ )
                mat[j][i] = m_Mat[i][j];
        return mat;
    }

    void fill(int val)
    {
        for ( int i = 0; i < rows(); i++ )
            for ( int j = 0; j < cols(); j++ )
                m_Mat[i][j] = val;
    }


};

template<typename T>
void fillvectMat(std::vector<Matrix<T>>& v)
{
    for ( auto& mat : v )
        mat.fill(0);
}

template<typename T>
Matrix<T> operator*(double nr, const Matrix<T>& matrix)
{
    Matrix<T> mat(( int )matrix.rows(), ( int )matrix.cols());

    for ( int i = 0; i < matrix.rows(); i++ )
        for ( int j = 0; j < matrix.cols(); j++ )
            mat[i][j] = ( T )(nr * matrix[i][j]);
    return mat;
}

template<typename T>
Matrix<T> hadamaradProduct(const Matrix<T>& a, const Matrix<T>& b)
{
    assert(a.rows() == b.rows() && a.cols() == b.cols());

    Matrix<T> mat;
    mat.setDim(a.rows(), a.cols());

    for ( int i = 0; i < a.rows(); i++ )
        for ( int j = 0; j < a.cols(); j++ )
            mat[i][j] = a[i][j] * b[i][j];
    return mat;
}

Matrix<double> randMat(int rows, int cols)
{
    Matrix<double> mat(rows, cols);

    for ( int i = 0; i < rows; i++ )
        for ( int j = 0; j < cols; j++ )
            mat[i][j] = mat.dis(mat.gen);
    return mat;
}

template<typename T>
Matrix<T> identityMat(int n)
{
    Matrix<T> mat(n, n);

    for ( int i = 0; i < n; i++ )
        mat[i][i] = 1;

    return mat;
}

template<class T>
class ColumnVector: public Matrix<T>
{
    public:
    ColumnVector() = default;
    ColumnVector(const std::vector<T>& a)
    {
        setDim(( int )a.size());

        for ( int i = 0; i < ( int )a.size(); i++ )
            this->m_Mat[i][0] = a[i];
    }
    ColumnVector(const Matrix<T>& a)
    {
        assert(a.cols() == 1);
        setDim(( int )a.rows());

        for ( int i = 0; i < a.rows(); i++ )
            this->m_Mat[i][0] = a[i][0];
    }
    ColumnVector(int Dim)
    {
        this->fill(0);
        setDim(Dim);
    }
    ColumnVector(int Dim, int val)
    {
        setDim(Dim);
        this->fill(val);
    }

    const T operator[](int index) const
    {
        assert(index >= 0 && index < this->rows() && this->cols() == 1);
        return this->m_Mat[index][0];
    }

    T& operator[](int index)
    {
        assert(index >= 0 && index < this->m_Mat.size());
        return this->m_Mat[index][0];
    }

    void operator=(const ColumnVector<T>& x)
    {
        this->m_Mat.resize(x.rows(), mine::vector<T>(x.cols(), 0));
        for ( int i = 0; i < ( int )x.size(); i++ )
            (*this)[i] = x[i];
    }

    int size() const
    {
        return ( int )this->rows();
    }

    void setDim(int rows)
    {
        this->m_Mat.resize(rows, mine::vector<T>(1, 0));
    }
};

template<typename T>
ColumnVector<T> OneDim(int maxDim, int Dim)
{
    assert(Dim >= 0 && Dim < maxDim);
    ColumnVector<T> vect(maxDim);
    vect[Dim] = 1;
    return vect;
}

template<typename T>
void fillvectMat(std::vector<ColumnVector<T>>& v)
{
    for ( auto& mat : v )
        mat.fill(0);
}

template<typename T>
void operator+=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    for ( int i = 0; i < a.size(); i++ )
        a[i] += b[i];
}

template<typename T>
void operator-=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    for ( int i = 0; i < a.size(); i++ )
        a[i] -= b[i];
}

template<typename T, typename U>
std::vector<T> operator*(const U a, std::vector<T> b)
{
    for ( int i = 0; i < b.size(); i++ )
        b[i] = b[i] * a;
    return b;
}


