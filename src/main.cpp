#include <vk_engine.h>

int main(int argc, char* argv[])
{
	bool isExe = false;
	if (argc >= 2) { // change this later
		isExe = true;
	}

	VulkanEngine engine;

	engine.init(isExe);	
	
	engine.run();	

	engine.cleanup();	

	return 0;
}
