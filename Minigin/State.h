#pragma once

namespace dae {
	template<typename StateClass>
	class State
	{
	public:
		virtual ~State() {};

		virtual void Update(StateClass& stateObject) = 0;
		virtual void OnEnter(StateClass& stateObject) = 0;
		virtual void OnExit(StateClass& stateObject) = 0;
	};
}