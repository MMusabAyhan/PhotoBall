#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imguiWindow.h"
#include "imfilebrowser.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

imguiWindow::imguiWindow(GLFWwindow* window) 
	: m_io(ImGuiIO()), m_GLFWwindow(window), m_SelectedFileName("")
{
	Init();
}

imguiWindow::~imguiWindow()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	m_FileDialog.Close();
}

void imguiWindow::OnUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void imguiWindow::OnRender()
{
	if (Application::Get().isInSimulation()) 
	{
		if (m_FileDialog.HasSelected()) 
		{
			m_SelectedFileName = "";
			m_FileDialog.ClearSelected();
		}

		return;
	}

	{
		ImGui::Begin("Select an Image to display!");  

		// open file dialog when user clicks this button
		if (ImGui::Button("Find Image"))
		{
			m_FileDialog.Open();
			m_EventSentAlready = false;
		}

		ImGui::Text("Selected File : ");

		if (m_FileDialog.HasSelected()) 
		{
			m_SelectedFileName = m_FileDialog.GetSelected().filename().string();

			if (!m_EventSentAlready)
			{
				m_EventSentAlready = true;
				SimulationSetEvent e(m_FileDialog.GetSelected().string());
				m_EventCallback(e);
			}
		}

		ImGui::Text(m_SelectedFileName.c_str());
		
		ImGui::End();
	}

	{
		ImGui::Begin("Select an Image to display!");
		ImGui::Text("\n");
	
		ImGui::Text("Simulation Type : ");
	
		const char* items[] = { "Waterfall", "Shower", "TwoReversedWaterFall", "Sprinkle", "MovingCloud" };
		static const char* current_item = items[0];
	
		if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(items[n], is_selected)) 
				{
					current_item = items[n];
					Application::Get().SetSimulationType((SimulationType) n);

					if (m_FileDialog.HasSelected())
					{
						m_SelectedFileName = m_FileDialog.GetSelected().filename().string();

						//m_EventSentAlready = true;
						SimulationSetEvent e(m_FileDialog.GetSelected().string());
						m_EventCallback(e);
					}
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
	
		ImGui::Text("\n");
		ImGui::End();
	}

	{
		ImGui::Begin("Select an Image to display!");

		if (ImGui::Button("Start Simulation") && m_FileDialog.HasSelected()) 
		{
			SimulationStartedEvent e;
			m_EventCallback(e);
		}

		ImGui::End();
	}

	// Example To look at
	
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;
	//
	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	//
	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);
	//
	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	//
	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);
	//
	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	//	ImGui::End();
	//}

	m_FileDialog.Display();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imguiWindow::Init()
{
	// Imgui

	const char* glsl_version = "#version 130";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_io = ImGui::GetIO(); (void)m_io;
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_GLFWwindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// (optional) set browser properties
	m_FileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
	m_FileDialog.SetTitle("Select An Image To Display");
	m_FileDialog.SetTypeFilters({ ".jpg", ".png" });

}
