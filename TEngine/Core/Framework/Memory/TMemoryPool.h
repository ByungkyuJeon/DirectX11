#pragma once

#include <typeinfo>
#include <unordered_map>
#include "../../Types/MemoryTypes.h"
#include "MemoryDefinitions.h"

class TMemoryPool;

class TMemoryManager
{
private:
	std::unordered_map<std::string, TMemoryPool*> m_Pools;

public:
	TMemoryManager() 
	{
		
	}

	void RegisterPool(const std::string& InName, TMemoryPool* InPoolPtr)
	{
		if (m_Pools.find(InName) != m_Pools.end())
			_ASSERT(true);

		m_Pools[InName] = InPoolPtr;
	};

};

static TMemoryManager* gMemoryManager;

class TMemoryPool
{

};

template<typename TType, Tsize_t TSize>
class MemoryPool : public TMemoryPool
{
public:
	GENERATE_MEMORY_POOL

private:
	TType m_Pool[TSize];
	Tsize_t m_CurrentIdx;

public:

};

template<typename TType, Tsize_t TSize>
static void CreatePool()
{
	DEFINE_MEMORY_POOL(TType, TSize)
}

#include "MemoryPool.inl"