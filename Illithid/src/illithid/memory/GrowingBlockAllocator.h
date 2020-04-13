#pragma once
#include <list>
#include <unordered_map>
#include <set>
#include "illithid/utils/ptr_ref.h"

template <typename T, size_t BLOCK_SIZE>
struct Chunk
{
public:

	explicit Chunk( size_t priority )
		: priority_( priority ), buffer_( nullptr ), size_( 0 )
	{
		buffer_ = reinterpret_cast<T*>( malloc( BLOCK_SIZE * sizeof( T ) ) );
		addresses_.resize( BLOCK_SIZE, nullptr );
	}

	~Chunk( )
	{
		for (size_t i = 0; i < size_; ++i)
		{
			( buffer_ + i )->~T( );
		}

		addresses_.clear( );
		free( buffer_ );
		buffer_ = nullptr;
	}

	inline bool is_full( ) const
	{
		return  size_ == BLOCK_SIZE;
	}

	inline bool size( ) const
	{
		return size_;
	}

	inline size_t priority( ) const
	{
		return priority_;
	}

	T* allocate( )
	{
		T* ptr = ( buffer_ + size_ );
		++size_;
		return ptr;
	}

	T* get_pointer_at( size_t index )
	{
		return ( buffer_ + index );
	}

	void set_address( T** ptr )
	{
		addresses_[ size_ - 1 ] = ptr;
	}

	void release( T* ptr, Chunk<T, BLOCK_SIZE>* lastChunk, const std::function<void( T** )>& OnSwap )
	{
		ptr->~T( );

		void* src = reinterpret_cast<void*>( lastChunk->buffer_ + ( lastChunk->size_ - 1 ) );
		void* current = reinterpret_cast<void*>( ptr );
		if (src != current)
		{
			memcpy( current, src, sizeof( T ) );

			*( lastChunk->addresses_[ lastChunk->size_ - 1 ] ) = reinterpret_cast<T*>( current );
			OnSwap( lastChunk->addresses_[ lastChunk->size_ - 1 ] );
		}
		--lastChunk->size_;
	}

private:
	T* buffer_;
	std::vector<T**> addresses_;
	size_t size_, priority_;
};

template <typename T, size_t BLOCK_SIZE>
class GrowingBlockAllocator
{
	using TChunk = Chunk<T, BLOCK_SIZE>;
public:
	GrowingBlockAllocator( )
		:size_( 0 )
	{

	}

	~GrowingBlockAllocator( )
	{
		freeChunks_.clear( );
		for each (TChunk * chunk in chunks_)
		{
			delete chunk;
		}
		chunks_.clear( );
		memoryMap_.clear( );
	}

	template <typename... params>
	ptr_ref<T> instantiate( params&& ...args )
	{
		if (freeChunks_.empty( ))
		{
			TChunk* chunk = new TChunk( chunks_.size( ) );
			chunks_.push_back( chunk );
			freeChunks_.insert( chunk );
		}

		TChunk* chunk = *( freeChunks_.begin( ) );
		T* ptr = chunk->allocate( );

		//If there is no more memory in the chunk, remove it from the free list
		if (chunk->is_full( ))
		{
			freeChunks_.erase( chunk );
		}

		ptr_ref<T> ref( new( ptr ) T( std::forward<params>( args )... ), [ this ] ( ptr_ref<T>& ref )
		{
			this->release( ref );
		} );

		chunk->set_address( ref.get_address( ) );
		memoryMap_[ ref.get_address( ) ] = chunk;

		++size_;
		return ref;
	}

	void release( ptr_ref<T>& ref )
	{
		TChunk* chunk = memoryMap_[ ref.get_address( ) ];
		TChunk* lastChunk = nullptr;

		for (std::vector<TChunk*>::reverse_iterator rit = chunks_.rbegin( );
			  rit != chunks_.rend( ); ++rit)
		{
			lastChunk = *( rit );
			size_t size = lastChunk->size( );
			if (size > 0)
			{
				break;
			}
		}

		chunk->release( ref.get( ), lastChunk, [ & ] ( T** ptr )
		{
			this->memoryMap_[ ptr ] = chunk;
		} );

		if (!lastChunk->is_full( ))
		{
			freeChunks_.insert( lastChunk );
		}

		if (chunk->is_full( ))
		{
			freeChunks_.erase( chunk );
		}

		ref.set( nullptr );
		--size_;
	}

	inline size_t size( ) const
	{
		return size_;
	}

	T* operator[]( size_t i )
	{
		size_t ptr_index = i % BLOCK_SIZE;
		size_t chunk_index = ( i - ptr_index ) / BLOCK_SIZE;

		return chunks_[ chunk_index ]->get_pointer_at( ptr_index );
	}

private:
	std::vector<TChunk*> chunks_;
	std::unordered_map<T**, TChunk*> memoryMap_;

	struct Comparator
	{
	public:
		inline bool operator()( TChunk* lhs, TChunk* rhs ) const
		{
			return lhs->priority( ) < rhs->priority( );
		}
	};

	std::set<TChunk*, Comparator> freeChunks_;
	size_t size_;
};