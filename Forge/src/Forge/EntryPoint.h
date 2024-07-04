#pragma once

#ifdef FG_PLATFORM_WINDOWS

extern Forge::Application* Forge::CreateApplication();

int main(int argc, char** argv) {
	std::cout << "Welcome to Forge Engine" << std::endl;
	auto app = Forge::CreateApplication();
	app->Run();
	delete app;

}

#endif // FG_PLATFORM_WINDOWS
