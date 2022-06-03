#pragma once

#define REGISTER_MEMORY_POOL(TYPE_NAME_STR) \
		if(gMemoryManager == nullptr) { gMemoryManager = new TMemoryManager();} \
		gMemoryManager->RegisterPool(TYPE_NAME_STR, &this->m_Pool); \

#define DEFINE_MEMORY_POOL(TYPE_NAME, POOL_SIZE) static MemoryPool<TYPE_NAME, POOL_SIZE> m_Pool; \
		
#define GENERATE_MEMORY_POOL \
	MemoryPool() : m_CurrentIdx{ 0 } \
	{ \
		if(gMemoryManager == nullptr) { gMemoryManager = new TMemoryManager();} \
		gMemoryManager->RegisterPool(typeid(*this).name(), this); \
	}; \