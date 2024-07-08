#pragma once

#ifdef FG_PLATFORM_WINDOWS

extern Forge::Application* Forge::CreateApplication();

int main(int argc, char** argv) {
	Forge::Log::Init();
	FG_CORE_WARN("Initialised Logger");
	int a = 5;
	FG_INFO("Hello World! Var = {0}",a); 
	auto app = Forge::CreateApplication();
	app->Run();
	delete app;

}

#endif // FG_PLATFORM_WINDOWS
