#include "Renderer.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

Renderer::Renderer()
{
}

// Returns a color for a given ray r
color Renderer::ray_color(const ray& r, const hittable_list& world, int depth)
{
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::SetScene(const hittable_list& scene, const camera& camera) {
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;
}

void Renderer::Render(int spp, int max_depth)
{
	//// Empty the m_imageData
	//for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	//{
	//	for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
	//	{
	//		m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(0.1, 0.1, 0.2, 1.0));
	//	}
	//}
	//m_FinalImage->SetData(m_ImageData);

	// Render the scene
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			// render
			color pixel_color(0, 0, 0);
			for (int s = 0; s < spp; ++s)
			{
				// Screen UV coordinates
				auto u = (x + random_double()) / (m_FinalImage->GetWidth() - 1);
				auto v = (y + random_double()) / (m_FinalImage->GetHeight() - 1);
				ray r = m_ActiveCamera->get_ray(u, v);

				// Add the color of every sample to current pixels color
				pixel_color += ray_color(r, *m_ActiveScene, max_depth);
			}
			vec3 averaged_color = pixel_color / spp;
			glm::vec4 color(averaged_color.x(), averaged_color.y(), averaged_color.z(), 1.0);

			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
		m_FinalImage->SetData(m_ImageData);
	}

	m_FinalImage->SetData(m_ImageData);
}

void Renderer::Reset()
{
	// Empty the m_imageData
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(0.1, 0.1, 0.2, 1.0));
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

void Renderer::PartialRender(float portion, int portion_id, bool finalPortion, int spp, int max_depth)
{
	
	// Calculate the portion of the image that is going to be rendered in this step
	int startRow = portion_id * static_cast<int>((m_FinalImage->GetHeight()*portion));
	int endRow = (portion_id+1) * static_cast<int>((m_FinalImage->GetHeight() * portion));
	if (finalPortion) {
		endRow = m_FinalImage->GetHeight();
	}
	
	// Render the scene for that portion
	for (uint32_t y = startRow; y < endRow; y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			// render
			color pixel_color(0, 0, 0);
			for (int s = 0; s < spp; ++s)
			{
				// Screen UV coordinates
				auto u = (x + random_double()) / (m_FinalImage->GetWidth() - 1);
				auto v = (y + random_double()) / (m_FinalImage->GetHeight() - 1);
				ray r = m_ActiveCamera->get_ray(u, v);

				// Add the color of every sample to current pixels color
				pixel_color += ray_color(r, *m_ActiveScene, max_depth);
			}
			vec3 averaged_color = pixel_color / spp;
			glm::vec4 color(averaged_color.x(), averaged_color.y(), averaged_color.z(), 1.0);

			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	// Update the finalImage after every portion
	m_FinalImage->SetData(m_ImageData);
}

Renderer::~Renderer()
{
}
