#pragma once

class TWindowConfig;

class TEngineConfigManager
{
public:
	TEngineConfigManager();
	~TEngineConfigManager();

	void Initialize();
	void Finalize();

	TWindowConfig* gWindowConfig = nullptr;


};