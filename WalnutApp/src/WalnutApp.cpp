#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include <Walnut/Timer.h>
#include "primitives/camera.h"
#include "utils/hittable_list.h"
#include "renderer/Renderer.h"
#include "utils/scenes.h"
#include "theme.h"

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() {
		// set random seed
		srand(69);

		// World
		hittable_list scene = Scenes::three_spheres_motion_blur_scene();
		m_Scene = scene;

		// Camera
		point3 m_lookfrom(13.0, 2.0, 3.0);
		point3 m_lookat(0.0, 0.0, 0.0);
		vec3 m_vup(0.0, 1.0, 0.0);
		double m_dist_to_focus = 12.0;
		double m_aperture = 0.1;
		camera camera(m_lookfrom, m_lookat, m_vup, 19, m_aspect_ratio, m_aperture, m_dist_to_focus, 0.0, 1.0);
		m_Camera = camera;

		m_Renderer.SetScene(m_Scene, m_Camera);
	}

	virtual void OnUIRender() override
	{
		embraceTheDarkness();

		ImGui::Begin("Render Settings");
		if (ImGui::Button("Render")) {
			std::cout << "Render button clicked";
			m_rendering = true;
			
			//Render();
		}

		if (ImGui::Button("Reset")) {
			std::cout << "Reset button clicked";
			Reset();
		}

		// Partial Rendering
		//srand(time(NULL));
		srand(42);

		//m_Renderer.SetScene(m_Scene, m_Camera);
		if (m_rendering) {
			if (m_PortionId < m_Portions-1) {
				PartialRender(1.0 / m_Portions, m_PortionId, false);
				m_PortionId++;
			}
			else {
				PartialRender(1.0 / m_Portions, m_PortionId, true);
				m_PortionId = 0;
				m_rendering = false;
			}
		}


		ImGui::DragInt("Samples/Pixel", &m_samples_per_pixel, 1.0f, 1, 100);
		ImGui::DragInt("Max Depth", &m_max_depth, 1.0f, 1, 100);
		ImGui::DragInt("Portions", &m_Portions, 1.0f, 1, 100);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Statusbar");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		ImGui::Text("Portion: %d / %d", m_PortionId+1, m_Portions);
		ImGui::End();

		//ImGui::ShowDemoWindow();
	}

	void Render()
	{
		Walnut::Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		// m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_samples_per_pixel, m_max_depth);

		m_LastRenderTime = timer.ElapsedMillis();
	}

	void Reset()
	{
		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		// m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Reset();
	}

	void PartialRender(float portion, int portion_id, bool finalPortion) {
		Walnut::Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		// m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.PartialRender(portion, portion_id, finalPortion, m_samples_per_pixel, m_max_depth);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	int m_image_width = 500;
	int m_image_height = static_cast<int>(m_image_width / 4 * 3);
	double m_aspect_ratio = (double)m_image_width / (double)m_image_height;
	int m_samples_per_pixel = 2;
	int m_max_depth = 2;
	
	uint32_t m_ViewportWidth = m_image_width, m_ViewportHeight = m_image_height;
	Renderer m_Renderer;

	int m_Portions = 3;
	int m_PortionId = 0;
	bool m_rendering = false;

	float m_LastRenderTime = 0.0f;

	hittable_list m_Scene;
	camera m_Camera;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "GHD Path Tracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}