#pragma once

template <typename T>
class ptr_ref
{
public:
	ptr_ref( )
		:ptr_( nullptr )
	{

	}

	ptr_ref( T** ptr )
		:ptr_( ptr )
	{
	}

	ptr_ref( const ptr_ref& other )
		: ptr_( other.ptr_ )
	{
	}

	ptr_ref( ptr_ref&& other ) noexcept
		: ptr_( other.ptr_ )
	{
		other.ptr_ = nullptr;
	}

	ptr_ref& operator=( const ptr_ref& other )
	{
		this->ptr_ = other.ptr_;
		return *this;
	}

	ptr_ref& operator=( ptr_ref&& other ) noexcept
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

	inline bool operator==( const ptr_ref& other ) const
	{
		if (other.ptr_)
		{
			return ( *ptr_ ) == *( other.ptr_ );
		}

		return ( *ptr_ == nullptr );
	}

	inline bool operator!=( const ptr_ref& other ) const
	{
		return !( operator==( other ) );
	}

	template <typename U>
	static ptr_ref<U> dyn_cast( ptr_ref<T>& other )
	{
		ptr_ref<U> dp;
		U* data = dynamic_cast<U*>( other.get_data( ) );

		if (data)
		{
			dp = ptr_ref<U>( reinterpret_cast<U**>( other.get_address( ) ) );
		}
		return dp;
	}

private:
	T** ptr_;
};




