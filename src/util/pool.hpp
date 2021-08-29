#pragma once

#include <stdint.h>
#include <stdlib.h>

#ifdef DEBUG
#define POOL_STATS
#endif

// not the best pool, doesn't help with memory fragmentation, but does reduce alloc times once things are up and running
// NOT THREAD SAFE
class Pool
{
private:
	void** pool;
	uint32_t head;
	uint64_t current;
	const uint32_t maxSize;
	const uint32_t typeSize;
#ifdef POOL_STATS
	uint64_t mallocs;
	uint64_t allocs;
#endif
public:
	inline Pool(const uint32_t max_size, const uint32_t type_size) : maxSize(max_size), typeSize(type_size)
	{
		pool	= (void**)malloc(max_size * sizeof(void*));
		head 	= 0;
	#ifdef POOL_STATS
		mallocs	= 0;
		allocs	= 0;
		current = 0;
	#endif
	}

	inline ~Pool()
	{
		for (uint32_t i = 0; i < head; i++) {
			free(pool[i]);
		}
		free(pool);
	}

	inline void* p_alloc()
	{
	#ifdef POOL_STATS
		allocs++;
	#endif
		if (head == 0) {
		#ifdef POOL_STATS
			mallocs++;
		#endif
			current++;
			return malloc(typeSize);
		}
		return pool[--head];
	}

	inline void p_free(void* ptr)
	{
		if (head == maxSize) {
			current--;
			free(ptr);
			return;
		}
		pool[head++] = ptr;
	}

	inline void p_trim(const uint32_t size)
	{
		for (uint32_t i = size; i < head; i++) {
			free(pool[i]);
		}
		head = size;
	}

	inline uint32_t poolCount()
	{
		return head;
	}

	inline uint64_t inCirculation()
	{
		return current-head;
	}

	inline uint64_t totalCount()
	{
		return current;
	}

	// ========== //
	// POOL STATS //
	// ========== //

	inline uint64_t allocCount()
	{
	#ifdef POOL_STATS
		return allocs;
	#else
		return 0;
	#endif
	}

	inline uint64_t mallocCount()
	{
	#ifdef POOL_STATS
		return mallocs;
	#else
		return 0;
	#endif
	}

	inline void resetStats()
	{
	#ifdef POOL_STATS
		allocs 	= 0;
		mallocs = 0;
	#endif
	}
};