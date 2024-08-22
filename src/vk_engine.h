// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#define REGULAR_FILEPATH(path, engine) ((engine->isExe ? engine->exeFilePath : engine->debuggerFilePath) + path)
#define BACKWARDS_FILEPATH(path, engine) ((engine->isExe ? engine->exeFilePathBackwards : engine->debuggerFilePathBackwards) + path)

#include <vk_types.h>
#include <vk_descriptors.h>
#include <vk_loader.h>
#include <camera.h>
#include <string>

struct ComputePushConstants {
	glm::vec4 data1;
	glm::vec4 data2;
	glm::vec4 data3;
	glm::vec4 data4;
};

struct ComputeEffect {
	const char* name;

	VkPipeline pipeline;
	VkPipelineLayout layout;

	ComputePushConstants data;
};

struct FrameData {
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	VkFence _renderFence;
	DeletionQueue _deletionQueue;
	DescriptorAllocatorGrowable _frameDescriptors;
};

struct GPUSceneData {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
	glm::vec4 ambientColor;
	glm::vec4 sunlightDirection; // w for sun power
	glm::vec4 sunlightColor;
};

struct GLTFMetallic_Roughness {
	MaterialPipeline opaquePipeline;
	MaterialPipeline transparentPipeline;

	VkDescriptorSetLayout materialLayout;

	struct MaterialConstants {
		glm::vec4 colorFactors;
		glm::vec4 metal_rough_factors;
		//padding, we need it anyway for uniform buffers
		glm::vec4 extra[14];
	};

	struct MaterialResources {
		AllocatedImage colorImage;
		VkSampler colorSampler;
		AllocatedImage metalRoughImage;
		VkSampler metalRoughSampler;
		VkBuffer dataBuffer;
		uint32_t dataBufferOffset;
	};

	DescriptorWriter writer;

	void build_pipelines(VulkanEngine* engine);
	void clear_resources(VkDevice device);

	MaterialInstance write_material(VkDevice device, MaterialPass pass, const MaterialResources& resources, DescriptorAllocatorGrowable& descriptorAllocator);
};


struct MeshNode : public Node {

	std::shared_ptr<MeshAsset> mesh;

	virtual void Draw(const glm::mat4& topMatrix, DrawContext& ctx) override;
};


struct RenderObject {
	uint32_t indexCount;
	uint32_t firstIndex;
	VkBuffer indexBuffer;

	MaterialInstance* material;

	glm::mat4 transform;
	VkDeviceAddress vertexBufferAddress;
	Bounds bounds;
};

struct DrawContext {
	std::vector<RenderObject> OpaqueSurfaces;
	std::vector<RenderObject> TransparentSurfaces;
};

struct EngineStats {
	float frametime;
	int triangle_count;
	int drawcall_count;
	float scene_update_time;
	float mesh_draw_time;
	float init_time;
};


// 2 because we're double buffering, i think?
constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine {
public:

	bool _isInitialized{ false };
	int _frameNumber {0};
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };

	struct SDL_Window* _window{ nullptr };

	static VulkanEngine& Get();

	EngineStats stats;

	//initializes everything in the engine
	void init(bool isExe);

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	// Draw the background
	void draw_background(VkCommandBuffer cmd);

	//run main loop
	void run();

	/// <summary>
	/// Handle for the vulkan lib
	/// </summary>
	VkInstance _instance;

	/// <summary>
	/// Output handle for vulkan debug
	/// </summary>
	VkDebugUtilsMessengerEXT _debug_messenger;

	/// <summary>
	/// The chosen default GPU
	/// </summary>
	VkPhysicalDevice _chosenGPU;

	/// <summary>
	/// Vulkan device for commands
	/// </summary>
	VkDevice _device;

	/// <summary>
	/// Vulkan window surface
	/// </summary>
	VkSurfaceKHR _surface;

private:
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void init_default_data();

	VmaAllocator _allocator;

	VkExtent2D _drawExtent;
	float renderScale = 1.f;

	std::vector<ComputeEffect> backgroundEffects;
	int currentBackgroundEffect{ 0 };

	GPUSceneData sceneData;
	MaterialInstance defaultData;

	DrawContext mainDrawContext;
	std::unordered_map<std::string, std::shared_ptr<Node>> loadedNodes;

	void update_scene();

	Camera mainCamera;

public:
	GLTFMetallic_Roughness metalRoughMaterial;
	DeletionQueue _mainDeletionQueue;
	AllocatedImage _drawImage;
	AllocatedImage _depthImage;
	VkDescriptorSetLayout _gpuSceneDataDescriptorLayout;

	bool isExe = false;
	std::string exeFilePath = "../../";
	std::string exeFilePathBackwards = "..\\..\\";
	std::string debuggerFilePath = "../../../";
	std::string debuggerFilePathBackwards = "..\\..\\..\\";

	// SWAPCHAIN STUFF
public:
	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

private:
	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
	void resize_swapchain();

	// FRAMEDATA STUFF

public:
	FrameData _frames[FRAME_OVERLAP];

	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;
	
	// DESCRIPTOR STUFF
public:
	DescriptorAllocatorGrowable globalDescriptorAllocator;

	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;

private:
	void init_descriptors();

	// PIPELINE STUFF

public:
	VkPipeline _gradientPipeline;
	VkPipelineLayout _gradientPipelineLayout;

private:
	void init_pipelines();
	void init_background_pipelines();

	// IMGUI STUFF
public:
	// immediate submit structures
	VkFence _immFence;
	VkCommandBuffer _immCommandBuffer;
	VkCommandPool _immCommandPool;

	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
	void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);

private:
	void init_imgui();

	// TRIANGLE STUFF
private:
	void draw_geometry(VkCommandBuffer cmd);

	// BUFFER STUFF

public:
	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
	void destroy_buffer(const AllocatedBuffer& buffer);
	GPUMeshBuffers uploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

	// MESH STUFF
private:
	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _meshPipeline;

	//GPUMeshBuffers rectangle;

	void init_mesh_pipeline();
	std::vector<std::shared_ptr<MeshAsset>> testMeshes;

	// RESIZE STUFF
private:
	bool resize_requested = false;

	// IMAGE STUFF

public:
	AllocatedImage create_image(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
	AllocatedImage create_image(void* data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
	void destroy_image(const AllocatedImage& img);

	AllocatedImage _errorCheckerboardImage;
	AllocatedImage _whiteImage;
	AllocatedImage _blackImage;
	AllocatedImage _greyImage;

	VkSampler _defaultSamplerLinear;
	VkSampler _defaultSamplerNearest;
	VkDescriptorSetLayout _singleImageDescriptorLayout;

	// GLTF LOADING STUFF
private:
	std::unordered_map<std::string, std::shared_ptr<LoadedGLTF>> loadedScenes;
};

bool is_visible(const RenderObject& obj, const glm::mat4& viewproj);