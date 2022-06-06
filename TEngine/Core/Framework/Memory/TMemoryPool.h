#pragma once

#include <typeinfo>
#include <unordered_map>
#include "../../Types/MemoryTypes.h"
#include "../Logger/StatLogger/StatLogger.h"
#include "../Logger/ErrorLogger/ErrorCode.h"


#define DEFINE_MEMORY_POOL(TYPE_NAME, POOL_SIZE) static TMemoryPool<TYPE_NAME, POOL_SIZE> m_Pool;

#define GENERATE_MEMORY_POOL \
	TMemoryPool() : m_CurrentIdx{ 0 } \
	{ \
		if(gMemoryManager == nullptr) { gMemoryManager = new TMemoryManager();} \
		gMemoryManager->RegisterPool(typeid(*this).name(), this); \
	}; \

class TMemoryPoolBase;

class TMemoryManager
{
private:
	std::unordered_map<std::string, TMemoryPoolBase*> m_Pools;

public:
	TMemoryManager() 
	{
		
	}

	void RegisterPool(const std::string& InName, TMemoryPoolBase* InPoolPtr)
	{
		if (m_Pools.find(InName) != m_Pools.end())
			_ASSERT(false);
		
		m_Pools[InName] = InPoolPtr;
	};

	template<typename TType>
	inline TType* CreateObject();

	template<typename TType>
	inline void DestroyObject(TType* InMem);

	inline size_t GetPoolCount() const;
	inline size_t GetTotalPoolSize() const;

};

static TMemoryManager* gMemoryManager;

class TMemoryPoolBase
{
public:
	virtual inline void* Allocate(Tsize_t InSIze) = 0;
	virtual inline void Free(void* InMem) = 0;
};

template<typename TType, Tsize_t TSize>
class TMemoryPool : public TMemoryPoolBase
{
public:
	GENERATE_MEMORY_POOL

private:
	TType m_Pool[TSize];
	bool m_State[TSize];
	Tsize_t m_CurrentIdx;

public:
	virtual inline void* Allocate(Tsize_t InSIze);
	virtual inline void Free(void* InMem);
};

template<typename TType, Tsize_t TSize>
static void CreatePool()
{
	DEFINE_MEMORY_POOL(TType, TSize)
}

#include "MemoryPool.inl"