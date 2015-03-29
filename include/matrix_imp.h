#include <vector>
#include <stack>

template <typename T>
Matrix<T>::Matrix(int x, int y, const T & val)
	: matrix(0), x(x), y(y)
{
	matrix = new T*[x];
	for (int i = 0; i < x; ++i)
	{
		matrix[i] = new T[y];	
		for (int j = 0; j < y; ++j)
			matrix[i][j] = val;
	}
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> & m)
	: matrix(0), x(m.x), y(m.y)
{
	matrix = new T*[x];
	for (int i = 0; i < x; ++i)
	{
		matrix[i] = new T[y];
		for (int j = 0; j < y; ++j)
			matrix[i][j] = m[i][j];
	}
}

template <typename T>
Matrix<T>::~Matrix()
{
	if (matrix != 0)
	{
		for (int i = 0; i < x; ++i)
			delete[] matrix[i];
		delete[] matrix;
	}
}

template <typename T>
Matrix<T> & Matrix<T>::operator= (const Matrix<T> & m)
{
	if (matrix != 0)
	{
		for (int i = 0; i < x; ++i)
			delete[] matrix[i];
		delete[] matrix;
	}

	x = m.x;
	y = m.y;
	
	matrix = new T*[x];
	for (int i = 0; i < x; ++i)
	{
		matrix[i] = new T[y];
		for (int j = 0; j < y; ++j)
			matrix[i][j] = m[i][j];
	}

	return *this;

}

template <typename T>
T * Matrix<T>::operator[](int position)
{
	return matrix[position];
}

template <typename T>
const T * Matrix<T>::operator[](int position) const
{
	return matrix[position];
}


