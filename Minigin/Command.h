#pragma once
namespace dae {
	class Command 
	{
	public:
		virtual void Execute() = 0;

		Command() = default;
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;
	};
}
