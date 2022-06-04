#pragma once

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
inline void* MemoryPool<TType, TSize>::Allocate(Tsize_t InSize)
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
		return nullptr;

	m_CurrentIdx = allocIdx;

	return &m_Pool[allocIdx];
}

template<typename TType, Tsize_t TSize>
inline void MemoryPool<TType, TSize>::Free(void* InMem)
{
	Tsize_t targetIdx = reinterpret_cast<TType*>(InMem) - m_Pool;
	m_State[targetIdx] = false;
	m_Pool[targetIdx] = TType();
}