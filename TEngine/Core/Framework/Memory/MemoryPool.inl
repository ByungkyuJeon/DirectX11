#pragma once

template<typename TType>
inline TType* TMemoryManager::CreateObject()
{
	TMemoryPoolBase* targetMemoryPool = m_Pools.find(typeid(TType).name());
	if (targetMemoryPool != m_Pools.end())
	{
		PRINT_STAT(ErrorCodeToString(EXISTING_OBJECT));
		_ASSERT(false);
		return IntToPtr(EXISTING_OBJECT);
	}

	return targetMemoryPool->Allocate(sizeof(TType));
}

template<typename TType>
inline void TMemoryManager::DestroyObject(TType* InMem)
{
	TMemoryPoolBase* targetMemoryPool = m_Pools.find(typeid(TType).name());
	if (targetMemoryPool == m_Pools.end())
	{
		PRINT_STAT(ErrorCodeToString(NOT_EXISTING_OBJECT));
		_ASSERT(false);
		return IntToPtr(NOT_EXISTING_OBJECT);
	}

	targetMemoryPool->Free(InMem);
}

inline size_t TMemoryManager::GetPoolCount() const
{
	return m_Pools.size();
}

inline size_t TMemoryManager::GetTotalPoolSize() const
{
	size_t totalSize = 0;

	for (const auto& pool : m_Pools)
	{
		totalSize += sizeof(*pool.second);
	}

	return totalSize;
}

template<typename TType, Tsize_t TSize>
inline void* TMemoryPool<TType, TSize>::Allocate(Tsize_t InSize)
{
	Tsize_t allocIdx = INVALID_ALLOCATION;
	for(Tsize_t idx = m_CurrentIdx + 1; idx < TSize; idx++)
	{
		if (m_State[idx])
			continue;

		allocIdx = idx;
		break;
	}

	if (allocIdx == INVALID_ALLOCATION)
	{
		for (Tsize_t idx = 0; idx <= m_CurrentIdx; idx++)
		{
			if (m_State[idx])
				continue;

			allocIdx = idx;
			break;
		}
	}

	if (allocIdx == INVALID_ALLOCATION)
	{
		PRINT_STAT(ErrorCodeToString(OUT_OF_MEMORY));
		_ASSERT(false);
		return IntToPtr(OUT_OF_MEMORY);
	}

	m_CurrentIdx = allocIdx;

	return &m_Pool[allocIdx];
}

template<typename TType, Tsize_t TSize>
inline void TMemoryPool<TType, TSize>::Free(void* InMem)
{
	Tsize_t targetIdx = reinterpret_cast<TType*>(InMem) - m_Pool;
	m_State[targetIdx] = false;
	m_Pool[targetIdx] = TType();
}