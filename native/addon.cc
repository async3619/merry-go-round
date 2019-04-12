#include "includes.hpp"

void finalizeAddon(void* args) {
	Music::finalize();

#if defined(_DEBUG)
	MemoryManager::getInstance()->checkMemoryLeak();
#endif
}

Napi::Object intitializeAddon(Napi::Env env, Napi::Object exports) {
	napi_add_env_cleanup_hook(env, finalizeAddon, nullptr);

	return Music::initialize(env, exports);
}

NODE_API_MODULE(addon, intitializeAddon);