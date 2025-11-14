#include "ResourceManager.h"
#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include <iostream>
#include <json.hpp>
#include <fstream>

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

dae::Texture2D* dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto textureIt = m_LoadedTexturesMap.find(file);
	if (textureIt != end(m_LoadedTexturesMap)) return std::get<dae::Texture2D*>(*textureIt);
	else {
		auto sdlTexture = IMG_LoadTexture(BL_RENDERER.GetSDLRenderer(), fullPath.c_str());

		if (sdlTexture == nullptr) throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		
		return LoadTexture(sdlTexture, file);
	}
}

dae::Texture2D* dae::ResourceManager::LoadTexture(SDL_Texture* pSDL_Texture, const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto textureIt = m_LoadedTexturesMap.find(file);
	if (textureIt != end(m_LoadedTexturesMap)) return std::get<dae::Texture2D*>(*textureIt);

	m_LoadedTextures.push_back(std::make_unique<Texture2D>(pSDL_Texture));
	auto rawTexture = m_LoadedTextures[m_LoadedTextures.size() - 1].get();
	m_LoadedTexturesMap[fullPath] = rawTexture;
	return rawTexture;
}

void dae::ResourceManager::UnLoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto textureIt = m_LoadedTexturesMap.find(file);

	if (textureIt != end(m_LoadedTexturesMap)) {
		auto pRaw = std::get<dae::Texture2D*>(*textureIt);

		auto uniqueIt = std::find_if(m_LoadedTextures.begin(), m_LoadedTextures.end(), [pRaw](const std::unique_ptr<dae::Texture2D>& pUnique)
			{ return pUnique.get() == pRaw; });

		if (uniqueIt != end(m_LoadedTextures)) {
			m_LoadedTextures.erase(uniqueIt);
		}

		m_LoadedTexturesMap.erase(fullPath);
#ifndef NDEBUG
		std::cout << "Loaded texture with file [" << fullPath << "] was successfully unloaded!\n";
#endif
	}
	else {
#ifndef NDEBUG
		std::cerr << "Loaded texture with file [" << fullPath << "] was not loaded before trying to unload!\n";
#endif
	}
}

bool dae::ResourceManager::IsTextureLoaded(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto textureIt = m_LoadedTexturesMap.find(file);
	return textureIt != end(m_LoadedTexturesMap);
}

dae::Font* dae::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	const auto fullPath = m_DataPath + file;
	auto fontIt = m_LoadedFontsMap.find({ fullPath, size });

	if (fontIt != end(m_LoadedFontsMap)) return std::get<dae::Font*>(*fontIt);
	else {
		m_LoadedFonts.push_back(std::make_unique<Font>(fullPath, size));
		auto rawFont = m_LoadedFonts[m_LoadedFonts.size() - 1].get();
		m_LoadedFontsMap[{fullPath, size}] = rawFont;
		return rawFont;
	}
}

void dae::ResourceManager::UnLoadFont(const std::string& file, unsigned int size)
{
	const auto fullPath = m_DataPath + file;
	auto fontIt = m_LoadedFontsMap.find({ fullPath, size });

	if (fontIt != end(m_LoadedFontsMap)) {
		auto pRaw = std::get<dae::Font*>(*fontIt);

		auto uniqueIt = std::find_if(m_LoadedFonts.begin(), m_LoadedFonts.end(), [pRaw](const std::unique_ptr<dae::Font>& pUnique)
			{ return pUnique.get() == pRaw; });

		if (uniqueIt != end(m_LoadedFonts)) {
			m_LoadedFonts.erase(uniqueIt);
		}

		m_LoadedFontsMap.erase(fontIt);
#ifndef NDEBUG
		std::cout << "Loaded font with file [" << fullPath << "] and size [" << size << "] was succesfully unloaded!\n";
#endif
	}
	else {
#ifndef NDEBUG
		std::cerr << "Loaded font with file [" << fullPath << "] and size [" << size << "] was not loaded before trying to unload!\n";
#endif
	}
}

bool dae::ResourceManager::IsFontLoaded(const std::string& file, unsigned int size)
{
	const auto fullPath = m_DataPath + file;
	auto fontIt = m_LoadedFontsMap.find({ fullPath, size });
	return fontIt != end(m_LoadedFontsMap);
}

nlohmann::json* dae::ResourceManager::LoadJSON(const std::string& fileName)
{
	const auto fullPath = m_DataPath + fileName;
	auto jsonIt = m_LoadedJSONMap.find(fullPath);
	if (jsonIt != end(m_LoadedJSONMap)) return std::get<nlohmann::json*>(*jsonIt);

	std::ifstream jsonFile{ fullPath };

	if (!jsonFile.is_open()) {
#ifndef NDEBUG
		std::cerr << "JSON file " << fileName << " could not be opened!\n";
#endif
		return nullptr;
	}

	nlohmann::json jsonData;
	jsonFile >> jsonData;

	jsonFile.close();

	m_LoadedJSON.push_back(std::make_unique<nlohmann::json>(jsonData));

	auto pRawJSON = m_LoadedJSON[m_LoadedJSON.size() - 1].get();
	if (pRawJSON) {
		m_LoadedJSONMap[fullPath] = pRawJSON;
	}


	return pRawJSON;
}

bool dae::ResourceManager::IsJSONLoaded(const std::string& fileName)
{
	const auto fullPath = m_DataPath + fileName;
	auto jsonIt = m_LoadedJSONMap.find(fullPath);
	return jsonIt != end(m_LoadedJSONMap);
}

void dae::ResourceManager::UnLoadJSON(const std::string& fileName)
{
	const auto fullPath = m_DataPath + fileName;
	auto jsonIt = m_LoadedJSONMap.find(fullPath);

	if (jsonIt != end(m_LoadedJSONMap)) {
		auto pRaw = std::get<nlohmann::json*>(*jsonIt);

		auto uniqueIt = std::find_if(m_LoadedJSON.begin(), m_LoadedJSON.end(), [pRaw](const std::unique_ptr<nlohmann::json>& pUnique)
			{ return pUnique.get() == pRaw; });
		
		if (uniqueIt != end(m_LoadedJSON)) {
			m_LoadedJSON.erase(uniqueIt);
		}

		m_LoadedJSONMap.erase(jsonIt);

#ifndef NDEBUG
		std::cout << "Loaded json with file [" << fullPath << "] was successfully unloaded!\n";
#endif
	}
	else {
#ifndef NDEBUG
		std::cerr << "Loaded json with file [" << fullPath << "] was not loaded before trying to unload!\n";
#endif
	}
}

void dae::ResourceManager::WriteToJSON(const std::string& fileName, const nlohmann::json& jsonToWrite)
{
	const auto fullPath = m_DataPath + fileName;
	//auto pLoadedJSON = m_LoadedJSONMap[fullPath];

	auto jsonIt = m_LoadedJSONMap.find(fullPath);
	if (jsonIt != end(m_LoadedJSONMap)) {
		UnLoadJSON(fullPath);
	}

	std::ofstream jsonFile{ fullPath };

	if (jsonFile.is_open()) {
		jsonFile << jsonToWrite.dump(4);
		jsonFile.close();

#ifndef NDEBUG
		std::cout << jsonToWrite.dump(4) << "\n";
#endif
	}
	else {
		// TODO : handle error
	}
}
