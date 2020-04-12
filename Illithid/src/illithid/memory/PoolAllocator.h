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
	}

	~Chunk( )
	{
		for (size_t i = 0; i < size_; ++i)
		{
			( buffer_ + i )->~T( );
		}

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

	void release( T* ptr )
	{
		ptr->~T( );

		void* last = reinterpret_cast<void*>( buffer_ + ( size_ - 1 ) );
		void* current = reinterpret_cast<void*>( ptr );
		if (last != current)
		{
			memcpy( current, last, sizeof( T ) );
		}

		--size_;
	}

private:
	T* buffer_;
	size_t size_, priority_;
};

template <typename T, size_t BLOCK_SIZE>
class PoolAllocator
{
	using TChunk = Chunk<T, BLOCK_SIZE>;

public:
	PoolAllocator( )
		:size_( 0 )
	{

	}

	~PoolAllocator( )
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
		memoryMap_[ ptr ] = chunk;

		//If there is no more memory in the chunk, remove it from the free list
		if (chunk->is_full( ))
		{
			freeChunks_.erase( chunk );
		}

		return ptr_ref<T>( new( ptr ) T( std::forward<params>( args )... ), [ this ] ( T* ptr )
		{
			this->release( ptr );
		} );

		++size_;
	}

	void release( T* ptr )
	{
		TChunk* chunk = memoryMap_[ ptr ];
		chunk->release( ptr );
		freeChunks_.insert( chunk );

		--size_;
	}


	void release( ptr_ref<T> ref )
	{
		TChunk* chunk = memoryMap_[ ref.get( ) ];
		chunk->release( ref.get( ) );
		freeChunks_.insert( chunk );

		ref.invalidate_peers( );
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
	std::unordered_map<T*, TChunk*> memoryMap_;

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