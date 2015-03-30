/*
 * Copyright (c) 2015, Alexandre Medeiros and Guilherme Perinazzo
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// Header containing the implementation for the templated matrix class

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


