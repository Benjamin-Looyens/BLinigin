#pragma once
#include "GameObject.h"
#include <vector>

namespace dae {
	class PlayerLocator 
	{
	public:
		static std::vector<GameObject*> GetPlayers()
		{
			return s_pPlayers;
		}

		static void RegisterPlayer(GameObject* pAddPlayer)
		{
			s_pPlayers.push_back(pAddPlayer);
		}

		static std::vector<GameObject*> s_pPlayers;
	};
}