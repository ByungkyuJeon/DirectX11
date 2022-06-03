#include "TEngine.h"

bool TEngine::InternalBootstrap()
{
	return false;
}

TEngine::TEngine()
{
	InternalBootstrap();
}

bool TEngine::Initiate()
{
	CreatePool<TEngine, 100>();

	return false;
}
