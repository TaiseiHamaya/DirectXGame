#define NOMINMAX
#include <windows.h>

#include <memory>

#include "Engine/Application/Framework.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<Framework> framework =
		std::make_unique<Framework>();

	framework->run();

	framework.reset();

	return 0;
}
