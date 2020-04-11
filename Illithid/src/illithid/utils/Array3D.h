#pragma once
#include <stdint.h>

template <typename T, size_t capacity_x, size_t capacity_y, size_t capacity_z>
class Array3D
{
public:
	Array3D( );
	~Array3D( );

	T& At( size_t x, size_t y, size_t z );
	void Set( size_t x, size_t y, size_t z, const T& value );

private:
	T* data_;
	size_t capacity_;
};

template<typename T, size_t capacity_x, size_t capacity_y, size_t capacity_z>
inline Array3D<T, capacity_x, capacity_y, capacity_z>::Array3D( )
	:capacity_( capacity_x* capacity_y* capacity_z )
{
	data_ = new T[ capacity_ ];
}

template<typename T, size_t capacity_x, size_t capacity_y, size_t capacity_z>
inline Array3D<T, capacity_x, capacity_y, capacity_z>::~Array3D( )
{
	delete[ ] data_;
}

template<typename T, size_t capacity_x, size_t capacity_y, size_t capacity_z>
inline T& Array3D<T, capacity_x, capacity_y, capacity_z>::At( size_t x, size_t y, size_t z )
{
	return data_[ ( capacity_z * capacity_y * x ) + ( capacity_z * y ) + z ];
}

template<typename T, size_t capacity_x, size_t capacity_y, size_t capacity_z>
inline void Array3D<T, capacity_x, capacity_y, capacity_z>::Set( size_t x, size_t y, size_t z, const T& value )
{
	data_[ ( capacity_z * capacity_y * x ) + ( capacity_z * y ) + z ] = value;
}




