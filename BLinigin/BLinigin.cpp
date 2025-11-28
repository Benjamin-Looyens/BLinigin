#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "BLinigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h" 
#include <algorithm>
#include "ServiceLocator.h"
#include "SoundSystem_Impl.h"
#include <thread>
//#include <vld.h>

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

	SDL_MIXER_VERSION(&version)
	printf("We compiled against SDL_mixer version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *Mix_Linked_Version();
	printf("We are linking against SDL_mixer version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::BLinigin::BLinigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw std::runtime_error(std::string("SDL_mixer initialization error: ") + Mix_GetError());
	}

	Mix_AllocateChannels(40); // set amount of audio channels (large enough number to have a lot of sounds simultaneously able to play)

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		700,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	BL_RENDERER.Init(g_window);

	BL_RESOURCE.Init(dataPath);

	BL_TIMER.Init();

	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDL_SoundSystem>());
	BL_SOUND.SetDataPath(dataPath.c_str());
}

dae::BLinigin::~BLinigin()
{
	BL_RENDERER.Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	Mix_CloseAudio();
	SDL_Quit();
}

void dae::BLinigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = BL_RENDERER;
	auto& sceneManager = BL_SCENE_MANAGER;
	auto& input = BL_INPUT;
	auto& timer = BL_TIMER;
	auto& soundSys = BL_SOUND;

	// Init sound queue
	auto pSoundThread = std::unique_ptr<std::thread>(soundSys.InitQueue());

	bool doContinue = true;
	float lag{ };

	while (doContinue) {
		const float deltaTime{ std::clamp(timer.GetDeltaTime(), 0.0f, 1.0f) }; // clamp to max 1.0f in case lag spike
		const float fixedTimeStep{ timer.GetFixedTimeStep() };

		lag += deltaTime;

		doContinue = input.ProcessInput();
		
		while (lag >= fixedTimeStep) {
			lag -= fixedTimeStep;
		}

		timer.Update();
		
		sceneManager.Update();
		sceneManager.LateUpdate();

		renderer.Render();

		//const auto sleep_time = currentTime + std::chrono::milliseconds(16) - std::chrono::high_resolution_clock::now(); // 30fps : 33.3ms | 60fps : 16.6ms | 90fps : 11.1ms | 120fps : 8.3ms
		//std::this_thread::sleep_for(sleep_time);
	}

	soundSys.Quit();
	if (pSoundThread.get()) pSoundThread->join();
}
