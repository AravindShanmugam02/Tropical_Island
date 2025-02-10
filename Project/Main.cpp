#include "../nclgl/window.h"
#include "Renderer.h"
#include <chrono>

int main() {
	Window w("CW - Aravind Shanmugam - C1036775", 1900, 1030, false);
	if(!w.HasInitialised()) {
		return -1;
	}
	w.SetTitle("Status: Loading... | CW - Aravind Shanmugam - C1036775");
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	
	int frames = 0;
	double fps = 0.0f;
	double deltaTime = 0.0f;
	auto prevTime = std::chrono::high_resolution_clock::now(); //can also use steady_time instead of high_resolution_clock

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		renderer.RenderScene();
		renderer.SwapBuffers();

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}

		++frames;
		deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime)).count();
		
		if (deltaTime >= 500.0f)
		{
			fps = double(frames / ((deltaTime / 1000.0f) * 2));
			//Since it's coming in at 500ms, I have to * by 2 to convert it into a second or it will be 0.5s.
			//converting milliseconds to seconds. But can get deltaTime directly as seconds from above - Just change the dureaction_cast to cast to seconds.

			frames = 0;
			prevTime = std::chrono::high_resolution_clock::now();
		}

		w.SetTitle("Status: Running! | FPS: " + std::to_string(fps) + " | CW - Aravind Shanmugam - C1036775");
	}
	return 0;
}