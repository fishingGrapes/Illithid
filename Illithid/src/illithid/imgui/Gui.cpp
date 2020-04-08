#include "ilpch.h"
#include "Gui.h"

#include "imgui.h"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "illithid/core/Window.h"

namespace itd
{
	void Gui::Initialize( std::unique_ptr<Window>& window )
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( ); (void) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark( );

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle( );
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ ImGuiCol_WindowBg ].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL( static_cast<GLFWwindow*>( window->GetRawWindow( ) ), true );
		ImGui_ImplOpenGL3_Init( "#version 410" );

	}

	void Gui::Destroy( )
	{
		ImGui_ImplOpenGL3_Shutdown( );
		ImGui_ImplGlfw_Shutdown( );
		ImGui::DestroyContext( );
	}

	void Gui::Begin( )
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );
	}

	void Gui::End( std::unique_ptr<Window>& window )
	{
		ImGuiIO& io = ImGui::GetIO( );
		io.DisplaySize = ImVec2( static_cast<float_t>( window->Width( ) ), static_cast<float_t>( window->Height( ) ) );

		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext( );
			ImGui::UpdatePlatformWindows( );
			ImGui::RenderPlatformWindowsDefault( );
			glfwMakeContextCurrent( backup_current_context );
		}
	}
}
