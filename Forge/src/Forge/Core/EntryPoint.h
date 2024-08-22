#pragma once

#ifdef FG_PLATFORM_WINDOWS

extern Forge::Application* Forge::CreateApplication();

int main(int argc, char** argv) {
	Forge::Log::Init();

	FG_BEGIN_PROFILE_SESSION("Startup", "ForgeProfile-Startup.json");
	auto app = Forge::CreateApplication();
	FG_END_PROFILE_SESSION();

	FG_BEGIN_PROFILE_SESSION("Runtime", "ForgeProfile-Runtime.json");
	app->Run();
	FG_END_PROFILE_SESSION();

	FG_BEGIN_PROFILE_SESSION("Shutdown", "ForgeProfile-Shutdown.json");
	delete app;
	FG_END_PROFILE_SESSION();
}

#endif // FG_PLATFORM_WINDOWS
