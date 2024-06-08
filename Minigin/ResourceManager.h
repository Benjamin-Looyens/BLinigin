#pragma once
#include <string>
#include <memory>
#include "Singleton.h"
#include <map>
#include <utility>
#include <vector>
#include "Texture2D.h"
#include "Font.h"
#include <json.hpp>

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
		bool IsTextureLoaded(const std::string& file);

		dae::Font* LoadFont(const std::string& file, unsigned int size);
		void UnLoadFont(const std::string& file, unsigned int size);
		bool IsFontLoaded(const std::string& file, unsigned int size);

		nlohmann::json* LoadJSON(const std::string& fileName);
		bool IsJSONLoaded(const std::string& fileName);
		void UnLoadJSON(const std::string& fileName);
		void WriteToJSON(const std::string& fileName, const nlohmann::json& jsonToWrite);

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::map<std::string, dae::Texture2D*> m_LoadedTexturesMap;
		std::map<std::pair<std::string, unsigned int>, dae::Font*> m_LoadedFontsMap;
		std::map<std::string, nlohmann::json*> m_LoadedJSONMap;

		std::vector<std::unique_ptr<dae::Texture2D>> m_LoadedTextures;
		std::vector<std::unique_ptr<Font>> m_LoadedFonts;
		std::vector<std::unique_ptr<nlohmann::json>> m_LoadedJSON;
	};
}

#define BL_RESOURCE dae::ResourceManager::GetInstance()