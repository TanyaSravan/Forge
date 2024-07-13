#include "FGpch.h"
#include "Forge/ImGui/ImguiLayer.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Forge/Application.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Forge {

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGui Layer") {}

	ImGuiLayer:: ~ImGuiLayer() {}

	void ImGuiLayer::OnAttach(){
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//Temp should eventually use FORGE keycodes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {

	}

	void ImGuiLayer::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

    bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;
        return false;
    }

    bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetXPos(), e.GetYPos());
        return false;
    }

    bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.GetYOffset();
        io.MouseWheelH += e.GetXOffset();
        return false;
    }

    bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];



        return false;
    }

    bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;
        return false;
    }

    bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        int keyCode = e.GetKeyCode();
        if (keyCode > 0 && keyCode < 0x10000)
            io.AddInputCharacter((unsigned short)keyCode);
        return false;
    }

    bool ImGuiLayer::OnWindowResised(WindowResizeEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)e.GetWindowWidth(), (float)e.GetWindowHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWindowWidth(), e.GetWindowHeight());
        return true;
    }

	void ImGuiLayer::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased));
        dispatcher.Dispatch<MouseScrolledEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolled));
        dispatcher.Dispatch<MouseMovedEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnMouseMoved));
        dispatcher.Dispatch<KeyPressedEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnKeyReleased));
        dispatcher.Dispatch<KeyTypedEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnKeyTyped));
        dispatcher.Dispatch<WindowResizeEvent>(FG_BIND_EVENT_FN(ImGuiLayer::OnWindowResised));
	}
}