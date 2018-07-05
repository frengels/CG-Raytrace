#pragma once

#include <vector>
#include <memory>

#include "defs.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "scene.hpp"

namespace rt
{
    class RenderTarget
    {
    public:
        virtual unsigned int getWidth() = 0;
        virtual unsigned int getHeight() = 0;
        virtual void setPixel(unsigned int w, unsigned int h, floating r, floating g, floating b) = 0;
    };

    class Raytracer
    {
    private:

    public:
        std::shared_ptr<Scene> scene;

        unsigned int maxDepth = 10;
        floating adaptiveDepth = 0.05;

        bool intersectTriangle(Ray& ray, Polygon& poly, floating& t, floating& u, floating& v, floating& w);       
        bool lightVisible(Ray& lightRay);
        fColor localLight(unsigned int polyIndex, Ray& viewRay, vec3& intersection, floating& u, floating& v, floating& w, vec3& normal);
        bool trace(Ray& ray, fColor& out_color, unsigned int depth, floating adpT);
        //bool intersectSphere(const ray& r, const sphere& sph);

        virtual void render(std::shared_ptr<rt::RenderTarget> target) = 0;
    };
}