#pragma once

#define DEFINE_MEMORY_POOL(TYPE_NAME, POOL_SIZE) static TMemoryPool<TYPE_NAME, POOL_SIZE> m_Pool; \
		
#define GENERATE_MEMORY_POOL(TYPE_NAME) \
	TMemoryPool() : m_CurrentIdx{ 0 } \
	{ \
		if(gMemoryManager == nullptr) { gMemoryManager = new TMemoryManager();} \
		gMemoryManager->RegisterPool(typeid(TYPE_NAME).name(), this); \
	}; \