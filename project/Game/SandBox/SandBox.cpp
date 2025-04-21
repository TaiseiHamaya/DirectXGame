#include "SandBox.h"

#include "Engine/Runtime/Scene/SceneManager.h"

#include "./Scene/FactoryDemo.h"

void SandBox::initialize() {
	SceneManager::SetFactory<FactoryDemo>();

	Framework::initialize();
}
