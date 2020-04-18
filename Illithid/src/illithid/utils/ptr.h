#pragma once

template <typename T>
class ptr
{
public:
	ptr( )
		:data_( nullptr )
	{
	}

	ptr( T* ptr )
		:data_( ptr )
	{
	}

	ptr( const ptr& other )
		: data_( other.data_ )
	{
	}

	ptr( ptr&& other ) noexcept
		: data_( other.data_ )
	{
		other.data_ = nullptr;
	}

	ptr& operator=( const ptr& other )
	{
		this->data_ = other.data_;
		return *this;
	}

	ptr& operator=( ptr&& other ) noexcept
	{
		this->data_ = other.data_;
		other.data_ = nullptr;

		return *this;
	}

	inline T* operator->( ) const
	{
		return  data_;
	}

	inline T operator*( ) const
	{
		return *data_;
	}

	inline T* get( ) const
	{
		return data_;
	}

	inline bool operator==( const ptr& other ) const
	{
		return data_ == other.data_;
	}

	inline bool operator!=( const ptr& other ) const
	{
		return data_ != other.data_;
	}

	template <typename U>
	inline static ptr<U> dyn_cast( ptr<T>& other )
	{
		return ptr<U>( dynamic_cast<U*>( other.get( ) ) );
	}

private:
	T* data_;
};




