#ifndef MATRIX_H
#define MATRIX_H

template <typename T>
struct Matrix
{
	T ** matrix;
	int x, y;

	Matrix(int x, int y, const T & val = T());
	Matrix(const Matrix<T> & m);
	~Matrix();

	Matrix & operator= (const Matrix<T> & m);

	T * operator[](int position);
	const T * operator[](int position) const;
};

#include "matrix_imp.h"


#endif