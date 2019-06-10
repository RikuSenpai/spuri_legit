#include "../dependencies/common_includes.hpp"
#include "features/misc/misc.hpp"

bool in_thirdperson;
unsigned long __stdcall initial_thread(void* reserved) {

	interfaces::initialize();
	hooks::initialize();
	utilities::material_setup();

	while (!GetAsyncKeyState(VK_SCROLL))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	hooks::shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(reserved), 0);
	return 0ul;
}

bool __stdcall DllMain(void* instance, unsigned long reason_to_call, void* reserved) {
	if (reason_to_call == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, initial_thread, instance, 0, 0);
	}

	return true;
}