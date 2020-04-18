#pragma once

template <typename T>
class dptr
{
public:
	dptr( )
		:ptr_( nullptr )
	{

	}

	dptr( T** ptr )
		:ptr_( ptr )
	{
	}

	dptr( const dptr& other )
		: ptr_( other.ptr_ )
	{
	}

	dptr( dptr&& other ) noexcept
		: ptr_( other.ptr_ )
	{
		other.ptr_ = nullptr;
	}

	dptr& operator=( const dptr& other )
	{
		this->ptr_ = other.ptr_;
		return *this;
	}

	dptr& operator=( dptr&& other ) noexcept
	{
		this->ptr_ = other.ptr_;
		other.ptr_ = nullptr;

		return *this;
	}

	inline T* operator->( ) const
	{
		return ( *ptr_ );
	}

	inline T* get_data( ) const
	{
		return ( *ptr_ );
	}

	inline T** get_address( ) const
	{
		return ptr_;
	}

	inline bool operator==( const dptr& other ) const
	{
		if (other.ptr_)
		{
			return ( *ptr_ ) == *( other.ptr_ );
		}

		if (ptr_)
		{
			return ( *ptr_ == nullptr );
		}

		return ptr_ == nullptr;

	}

	inline bool operator!=( const dptr& other ) const
	{
		return !( operator==( other ) );
	}

	template <typename U>
	static dptr<U> dyn_cast( dptr<T>& other )
	{
		dptr<U> dp;
		U* data = dynamic_cast<U*>( other.get_data( ) );

		if (data)
		{
			dp = dptr<U>( reinterpret_cast<U**>( other.get_address( ) ) );
		}
		return dp;
	}

private:
	T** ptr_;
};




