#include "TEngineConfigManager.h"
#include "TWindowConfig.h"

TEngineConfigManager::TEngineConfigManager()
{

}

TEngineConfigManager::~TEngineConfigManager()
{

}

void TEngineConfigManager::Initialize()
{
	gWindowConfig = new TWindowConfig();
	gWindowConfig->Initialize();


}

void TEngineConfigManager::Finalize()
{
	gWindowConfig->Finalize();
	delete gWindowConfig;
	gWindowConfig = nullptr;


}