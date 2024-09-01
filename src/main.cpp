#include <vk_engine.h>
#include <thread>

int main(int argc, char* argv[])
{
	bool isExe = false;
	if (argc >= 2) { // change this later
		isExe = true;
	}

	VulkanEngine engine;

	engine.init(isExe);	
	
	//std::jthread engine_thread(&VulkanEngine::run, engine);
	engine.run();	

	engine.cleanup();	

	return 0;
}
