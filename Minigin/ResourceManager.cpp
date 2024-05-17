#include "ResourceManager.h"
#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include <iostream>

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
	auto loadedTexture = m_LoadedTexturesMap[fullPath];

	if (loadedTexture) return loadedTexture;
	else {
		auto sdlTexture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

		if (sdlTexture == nullptr) throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		
		return LoadTexture(sdlTexture, file);
	}
}

dae::Texture2D* dae::ResourceManager::LoadTexture(SDL_Texture* pSDL_Texture, const std::string& file)
{
	const auto fullPath = m_DataPath + file;

	m_LoadedTextures.push_back(std::make_unique<Texture2D>(pSDL_Texture));
	auto rawTexture = m_LoadedTextures[m_LoadedTextures.size() - 1].get();
	m_LoadedTexturesMap[fullPath] = rawTexture;
	return rawTexture;
}

void dae::ResourceManager::UnLoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto loadedTexture = m_LoadedTexturesMap[fullPath];

	if (loadedTexture) {
		for (auto& texture : m_LoadedTextures) {
			if (texture.get() == loadedTexture) {
				texture.reset();
				break;
			}
		}

		m_LoadedTexturesMap.erase(fullPath);
	}
	else {
		std::cerr << "Loaded texture with file [" << fullPath << "] was not loaded before trying to unload!\n";
	}
}

bool dae::ResourceManager::IsTextureLoaded(const std::string& file)
{
	const auto fullPath = m_DataPath + file;
	auto loadedTexture = m_LoadedTexturesMap[fullPath];

	if (loadedTexture) return true;
	else return false;
}

dae::Font* dae::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	const auto fullPath = m_DataPath + file;
	auto loadedFont = m_LoadedFontsMap[{fullPath, size}];

	if (loadedFont) return loadedFont;
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
	auto loadedFont = m_LoadedFontsMap[{fullPath, size}];

	if (loadedFont) {
		for (auto& font : m_LoadedFonts) {
			if (font.get() == loadedFont) {
				font.reset();
				break;
			}
		}

		m_LoadedFontsMap.erase({ fullPath, size });
	}
	else {
		std::cerr << "Loaded font with file [" << fullPath << "] and size [" << size << "] was not loaded before trying to unload!\n";
	}
}

bool dae::ResourceManager::IsFontLoaded(const std::string& file, unsigned int size)
{
	const auto fullPath = m_DataPath + file;
	auto loadedFont = m_LoadedFontsMap[{fullPath, size}];

	return loadedFont;
}
