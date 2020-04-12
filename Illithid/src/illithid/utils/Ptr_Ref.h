#pragma once
#include <utility>
#include "illithid/core/Log.h"


template <typename T>
class ptr_ref
{
	using Deleter = std::function<void( T* )>;

public:

	ptr_ref( )
		: refCount_( nullptr ), data_( nullptr ), valid_( nullptr ), deleter_( nullptr )
	{
	}

	ptr_ref( T* ptr, const Deleter& deleter = nullptr )
		: refCount_( new size_t( 1 ) ), data_( ptr ), valid_( new bool( true ) ), deleter_( deleter )
	{
	}

	ptr_ref( const ptr_ref& other )
		: refCount_( other.refCount_ ), data_( other.data_ ), valid_( other.valid_ ), deleter_( other.deleter_ )
	{
		++( *refCount_ );
	}

	ptr_ref( ptr_ref&& other )
		: refCount_( other.refCount_ ), data_( other.data_ ), valid_( other.valid_ ), deleter_( other.deleter_ )
	{
		other.data_ = nullptr;
		other.refCount_ = nullptr;
		other.valid_ = nullptr;
		other.deleter_ = nullptr;
	}

	ptr_ref& operator=( const ptr_ref& other )
	{
		this->refCount_ = other.refCount_;
		this->data_ = other.data_;
		this->valid_ = other.valid_;
		this->deleter_ = other.deleter_;

		++( *refCount_ );

		return *this;
	}

	ptr_ref& operator=( ptr_ref&& other )
	{
		this->refCount_ = other.refCount_;
		this->data_ = other.data_;
		this->valid_ = other.valid_;
		this->deleter_ = other.deleter_;

		other.data_ = nullptr;
		other.refCount_ = nullptr;
		other.valid_ = nullptr;
		other.deleter_ = nullptr;

		return *this;
	}

	~ptr_ref( )
	{
		if (valid_)
		{
			--( *refCount_ );

			if (*refCount_ <= 0)
			{
				if (*valid_)
				{
					if (deleter_)
						deleter_( data_ );
					else
						delete data_;
				}

				delete refCount_;
				delete valid_;

				data_ = nullptr;
				refCount_ = nullptr;
				valid_ = nullptr;
			}
		}
	}

	//decreases the refcount by 1
	void reset( )
	{
		if (valid_)
		{
			--( *refCount );

			if (*refCount_ <= 0)
			{
				if (*valid_)
				{
					if (deleter_)
						deleter_( data_ );
					else
						delete data_;
				}

				delete refCount_;
				delete valid_;
			}

			refCount_ = nullptr;
			data_ = nullptr;
			valid_ = nullptr;
		}
	}

	inline T* get( ) const
	{
		return data_;
	}

	inline size_t ref_count( ) const
	{
		return *refCount_;
	}

	//whether pointing to a valid memory location
	inline bool is_valid( ) const
	{
		return *valid_;
	}

	T* operator->( ) const
	{
		return data_;
	}

	T operator*( ) const
	{
		return *data_;
	}

	bool operator==( const ptr_ref& other ) const
	{
		return data_ == other.data_;
	}

	bool operator!=( const ptr_ref& other ) const
	{
		return data_ != other.data_;
	}

	template <typename... params >
	static ptr_ref<T> make( params&& ...args )
	{
		ptr_ref<T> ref;

		ref.refCount_ = new size_t( 1 );
		ref.data_ = new T( std::forward<params>( args )... );
		ref.valid_ = new bool( true );

		return ref;
	}

	void invalidate_peers( )
	{
		*valid_ = false;
	}

protected:
	size_t* refCount_;
	bool* valid_;
	T* data_;
	Deleter deleter_;
};

