#pragma once
#include <memory>
#include <unordered_map>
#include <list>
#include <utility>
#include "illithid/core/Log.h"


template <typename T>
class PoolAllocator
{
public:
	PoolAllocator( size_t initial_capacity = 8 )
		:capacity_( initial_capacity ), size_( 0 )
	{
		buffer_ = reinterpret_cast<T*>( malloc( initial_capacity * sizeof( T ) ) );
		deleter_ = [ this ] ( T* ptr )
		{
			release( ptr );
		};
	}

	~PoolAllocator( )
	{
		if (validityMap_.size( ) > 0)
		{
			DestroyElements( );
		}
		free( buffer_ );

		buffer_ = nullptr;
		size_ = capacity_ = 0;
	}

	void set_custom_deleter( std::function<void( T* )> deleter )
	{
		deleter_ = deleter;
	}

	template <typename... Args>
	inline std::shared_ptr<T> make_shared( Args&& ...args )
	{
		T* ptr = FindFreeMemory( );
		++size_;

		return std::shared_ptr<T>( new( ptr ) T( std::forward<Args>( args )... ), deleter_ );
	}

	void release( std::shared_ptr<T>& sptr )
	{
		release( sptr.get( ) );
		sptr.reset( );
	}

	template <typename... Args>
	inline T* make_raw( Args&& ...args )
	{
		T* ptr = FindFreeMemory( );
		++size_;

		return ( new( ptr ) T( std::forward<Args>( args )... ) );
	}

	void release( T* ptr )
	{
		size_t index = static_cast<size_t>( ptr - buffer_ );

		if (validityMap_[ index ])
		{
			validityMap_[ index ] = false;
			freeList_.push_back( index );
			--size_;

			ptr->~T( );
		}
	}

	inline size_t size( ) const
	{
		return size_;
	}

	inline size_t capacity( ) const
	{
		return capacity_;
	}

	inline T* data( ) const
	{
		return buffer_;
	}

	inline bool is_valid( T* ptr ) const
	{
		return ( validityMap_[ static_cast<size_t>( ptr - buffer_ ) ] );
	}

	void DebugValidity( )
	{
		for (size_t i = 0; i < size_; ++i)
		{
			//Destroy only if the memory contains a valid object of type T
			if (validityMap_[ i ])
			{
				IL_CORE_TRACE( "valid: {0}", (void*) ( buffer_ + i ) );
			}
			else
			{
				IL_CORE_TRACE( "Invalid: {0}", (void*) ( buffer_ + i ) );
			}
		}
	}

private:
	T* buffer_;
	size_t size_, capacity_;
	std::unordered_map<size_t, bool> validityMap_;
	std::list<size_t> freeList_;

	std::function<void( T* )> deleter_;

	void DestroyElements( )
	{
		for (size_t i = 0; i < size_; ++i)
		{
			//Destroy only if the memory contains a valid object of type T
			if (validityMap_[ i ])
			{
				( buffer_ + i )->~T( );
			}
		}
	}

	T* FindFreeMemory( )
	{
		size_t index = 0;

		//free fragmented memory available
		if (freeList_.size( ) > 0)
		{
			index = freeList_.front( );
			freeList_.pop_front( );
		}
		else
		{
			//Not enough memory already available in the buffer
			if (size_ == capacity_)
			{
				capacity_ *= 2;

				T* temp_buffer = reinterpret_cast<T*>( malloc( capacity_ * sizeof( T ) ) );
				memcpy( temp_buffer, buffer_, size_ * sizeof( T ) );
				DestroyElements( );
				free( buffer_ );
				buffer_ = temp_buffer;
			}

			index = size_;
		}

		validityMap_[ index ] = true;
		return ( buffer_ + index );
	}
};