#pragma once
#include <utility>

template <typename T>
class Ptr_Ref
{
public:

	Ptr_Ref( )
		: refCount_( nullptr ), data_( nullptr )
	{
	}

	Ptr_Ref( const Ptr_Ref& other )
		: refCount_( other.refCount_ ), data_( other.data_ )
	{
		++( *refCount_ );
	}

	Ptr_Ref( Ptr_Ref&& other )
		: refCount_( other.refCount_ ), data_( other.data_ )
	{
		other.data_ = nullptr;
		other.refCount_ = nullptr;
	}

	Ptr_Ref& operator=( const Ptr_Ref& other )
	{
		this->refCount_ = other.refCount_;
		this->data_ = other.data_;

		++( *refCount_ );
		return *this;
	}

	Ptr_Ref& operator=( Ptr_Ref&& other )
	{
		this->refCount_ = other.refCount_;
		this->data_ = other.data_;

		other.data_ = nullptr;
		other.refCount_ = nullptr;
		return *this;
	}

	void Reset( )
	{
		if (refCount_)
		{
			--( *refCount );

			if (*refCount_ <= 0)
			{
				delete data_;
				delete refCount_;;
			}

			refCount_ = nullptr;
			data_ = nullptr;
		}
	}

	template <typename... Args>
	static Ptr_Ref<T> make( Args&& ...args )
	{
		Ptr_Ref<T> ptr;

		ptr.refCount_ = new size_t( 1 );
		ptr.data_ = new T( std::forward<Args>( args )... );

		return ptr;
	}

	T* operator->( ) const
	{
		return data_;
	}

	T operator*( ) const
	{
		return *data_;
	}

	bool operator==( const Ptr_Ref& other ) const
	{
		return data_ == other.data_;
	}

	bool operator!=( const Ptr_Ref& other ) const
	{
		return data_ != other.data_;
	}

	virtual ~Ptr_Ref( )
	{
		if (refCount_)
		{
			--( *refCount_ );

			if (*refCount_ <= 0)
			{
				delete data_;
				delete refCount_;

				data_ = nullptr;
				refCount_ = nullptr;
			}
		}
	}

protected:
	size_t* refCount_;
	T* data_;

};