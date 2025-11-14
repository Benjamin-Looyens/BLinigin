#pragma once
#include <memory>

namespace dae {
	template<typename StateClass>
	class State
	{
	public:
		virtual ~State() {};

		virtual std::unique_ptr<State> Update(StateClass& stateObject) = 0;
		virtual void OnEnter(StateClass& stateObject) = 0;
		virtual void OnExit(StateClass& stateObject) = 0;
	};
}