#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h" 
#include <algorithm>

//#include <thread>
//#include <iostream>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	Timer::GetInstance().Init();
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timer = Timer::GetInstance();

	bool doContinue = true;
	float lag{ };

	while (doContinue) {
		const auto currentTime{ std::chrono::high_resolution_clock::now() };
		//const auto& prevTime{ timer.GetPrevFrame() };
		const float deltaTime{ std::clamp(timer.GetDeltaTime(), 0.0f, 1.0f) }; // clamp to max 1.0f in case lag spike
		const float fixedTimeStep{ timer.GetFixedTimeStep() };

		lag += deltaTime;

		doContinue = input.ProcessInput();
		
		while (lag >= fixedTimeStep) {
			lag -= fixedTimeStep;
			sceneManager.FixedUpdate();
		}
		timer.Update();
		
		sceneManager.Update();
		sceneManager.LateUpdate();

		renderer.Render();

		//const auto sleep_time = currentTime + std::chrono::milliseconds(16) - std::chrono::high_resolution_clock::now(); // 30fps : 33.3ms | 60fps : 16.6ms | 90fps : 11.1ms | 120fps : 8.3ms
		//std::this_thread::sleep_for(sleep_time);
	}
}
