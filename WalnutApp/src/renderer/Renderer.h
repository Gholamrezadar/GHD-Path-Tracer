#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
#include "../primitives/camera.h"
#include "../utils/hittable_list.h"
#include "../utils/material.h"

class Renderer
{
public:
	Renderer();
	~Renderer();


	void OnResize(uint32_t width, uint32_t height);
	void SetScene(const hittable_list& scene, const camera& camera);
	void Render(int spp, int max_depth);
	void Reset();
	void PartialRender(float portion, int portion_id, bool finalPortion, int spp, int max_depth);
    // Returns a color for a given ray r
	color ray_color(const ray& r, const hittable_list& world, int depth);


	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;

	const hittable_list* m_ActiveScene = nullptr;
	const camera* m_ActiveCamera = nullptr;

	uint32_t* m_ImageData = nullptr;

};