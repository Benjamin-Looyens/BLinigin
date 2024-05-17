#pragma once
#include <string>
#include <memory>
#include "Singleton.h"
#include <map>
#include <utility>
#include <vector>
#include "Texture2D.h"
#include "Font.h"

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		dae::Texture2D* LoadTexture(const std::string& file);
		dae::Texture2D* LoadTexture(SDL_Texture* pSDL_Texture, const std::string& file);
		void UnLoadTexture(const std::string& file);

		dae::Font* LoadFont(const std::string& file, unsigned int size);
		void UnLoadFont(const std::string& file, unsigned int size);

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::map<std::string, Texture2D*> m_LoadedTexturesMap;
		std::map<std::pair<std::string, unsigned int>, Font*> m_LoadedFontsMap;

		std::vector<std::unique_ptr<Texture2D>> m_LoadedTextures;
		std::vector<std::unique_ptr<Font>> m_LoadedFonts;
	};
}
