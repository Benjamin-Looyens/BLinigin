#pragma once

namespace dae {
	namespace Timer {
		void Update();

		float DeltaTime();
		float FixedTimeStop() { return 1.0f / 60.0f; };
		
		void SetTimeScale(float newTimeScale = 1.0f);
	}
}