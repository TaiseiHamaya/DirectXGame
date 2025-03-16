#include "GameDemo.h"

#include "Engine/Runtime/Scene/SceneManager.h"

#include "TestCode/Scene/FactoryDemo.h"

void GameDemo::initialize() {
	SceneManager::SetFactory<FactoryDemo>();

	Framework::initialize();
}
