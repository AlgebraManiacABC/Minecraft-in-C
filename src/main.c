#include "main.h"

int main(int argc, char *argv[])
{
	if(!SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "vulkan", SDL_HINT_OVERRIDE))
	{
		SDL_Log("SDL_SetHint failure: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log("SDL_Init error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	int ww = 800, wh = 600;
	Uint32 wflags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
	SDL_Window *w = SDL_CreateWindow("Title here",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ww,wh,wflags);
	if(!w)
	{
		SDL_Quit();
		SDL_Log("SDL_CreateWindow error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED);
	if(!r)
	{
		SDL_DestroyWindow(w);
		SDL_Quit();
		SDL_Log("SDL_CreateRenderer error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	if (SDL_Vulkan_LoadLibrary(NULL) == -1)
	{
		// Handle loading error
		SDL_Log("Unable to load Vulkan library: %s", SDL_GetError());
		SDL_DestroyWindow(w);
		SDL_DestroyRenderer(r);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	//Uint32 extensionCount = 0;
	//vkEnumerateInstanceExtensionProperties(NULL,&extensionCount,NULL);
	//VkExtensionProperties* extensionProperties = malloc(sizeof(VkExtensionProperties) * extensionCount);
	//vkEnumerateInstanceExtensionProperties(NULL,&extensionCount,extensionProperties);
	//printf("%d extensions supported!\n",extensionCount);
	//const char** extensions = malloc(sizeof(char*) * extensionCount);

	//for (int i = 0; i < extensionCount; i ++)
    //{
	//	extensions[i] = extensionProperties[i].extensionName;
    //    printf("%u: %s\n", i, extensions[i]);
    //}

	VkInstance instance = NULL;
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello triangle!";
	appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.pNext = NULL;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = 0;
	//createInfo.enabledExtensionCount = extensionCount;
	//createInfo.ppEnabledExtensionNames = extensions;

	if(vkCreateInstance(&createInfo,NULL,&instance) != VK_SUCCESS)
	{
		SDL_Log("Unable to create Vulkan instance: %s", SDL_GetError());
		//free(extensions);
		//free(extensionProperties);
		SDL_DestroyWindow(w);
		SDL_DestroyRenderer(r);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	uint32_t extensionCount;
	if (!SDL_Vulkan_GetInstanceExtensions(w, &extensionCount, NULL))
	{
		// Handle extension retrieval error
		SDL_Log("Unable to retrieve Vulkan extensions (1): %s", SDL_GetError());
		SDL_DestroyWindow(w);
		SDL_DestroyRenderer(r);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	const char** extensionNames = malloc(extensionCount * sizeof(const char*));
	if (!SDL_Vulkan_GetInstanceExtensions(w, &extensionCount, extensionNames))
	{
		// Handle extension retrieval error
		SDL_Log("Unable to retrieve Vulkan extensions (2): %s", SDL_GetError());
		free(extensionNames);
		SDL_DestroyWindow(w);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	VkSurfaceKHR surface;
	if(!SDL_Vulkan_CreateSurface(w,instance,&surface))
	{
		SDL_Log("Unable to create Vulkan surface: %s", SDL_GetError());
		vkDestroyInstance(instance,NULL);
		//free(extensions);
		//free(extensionProperties);
		SDL_DestroyWindow(w);
		SDL_DestroyRenderer(r);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	bool should_close = false;
	while(!should_close)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					should_close = true;
					break;
				default:
					break;
			}
			SDL_RenderClear(r);
			SDL_RenderPresent(r);
			SDL_Delay(1000/60);
		}
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	free(extensionNames);
	//free(extensionProperties);
	//free(extensions);
	return EXIT_SUCCESS;
}