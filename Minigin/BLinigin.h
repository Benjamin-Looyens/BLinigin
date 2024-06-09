#pragma once
#include <string>
#include <functional>

namespace dae
{
	class BLinigin final
	{
	public:
		explicit BLinigin(const std::string& dataPath);
		~BLinigin();
		void Run(const std::function<void()>& load);

		BLinigin(const BLinigin& other) = delete;
		BLinigin(BLinigin&& other) = delete;
		BLinigin& operator=(const BLinigin& other) = delete;
		BLinigin& operator=(BLinigin&& other) = delete;
	};
}