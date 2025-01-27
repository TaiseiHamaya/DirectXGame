#define NOMINMAX
#include <windows.h>

#include <memory>

#include "TestCode/GameDemo.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<Framework> framework =
		std::make_unique<GameDemo>();

	framework->run();

	framework.reset();

	return 0;
}
