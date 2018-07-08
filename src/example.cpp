#include <memory>
#include <vector>
#include <iostream>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <QWindow>
#include <QImage>
#include <QTime>

#include "core/defs.hpp"
#include "core/raytracer_simple.hpp"

class ImageTarget : public rt::RenderTarget
{
private:
    std::shared_ptr<QImage> image;

public:
    ImageTarget(std::shared_ptr<QImage> image) : image(image) {}
    unsigned int getWidth() override { return this->image->width(); }
    unsigned int getHeight() override { return this->image->height(); }
    void setPixel(unsigned int w, unsigned int h, rt::floating r, rt::floating g, rt::floating b) override
    {
        this->image->setPixelColor(w, h, { (int)(255 * r), (int)(255 * g), (int)(255 * b) });
    }
};

void setup(std::shared_ptr<rt::Scene> scene)
{
    rt::Camera cam = rt::Camera();
    cam.position = rt::vec3(10.0, 3.0, 0.0);
    cam.direction = glm::normalize(rt::vec3(0.0, 0.0, 0.0) - cam.position); // Look at {0.0, 0.0, 0.0}
    cam.fov = 90;

    rt::AmbientLight ambient = rt::AmbientLight();
    ambient.color = rt::fColor(1.0f, 1.0f, 1.0f);
    ambient.intensity = 1.0;

    rt::Light light = rt::Light();
    light.position = rt::vec3(5.0, 8.0, -5.0);
    light.color = rt::fColor(1.0f, 1.0f, 1.0f);
    light.intensity = 1.0;

    scene->background = rt::fColor(0.5294117647, 0.80784313725, 0.92156862745);
    scene->camera = cam;
    scene->ambient = ambient;
    scene->light = light;

    std::shared_ptr<rt::Material> bronze = std::make_shared<rt::Material>();
    bronze->ambient = { 0.329412, 0.223529, 0.027451 };
    bronze->diffuse = { 0.780392, 0.568627, 0.113725 };
    bronze->specular = { 0.992157, 0.941176, 0.807843 };
    bronze->shininess = 27.8974;
    bronze->reflection_amount = 0.4;
    bronze->transparent = false;

    std::shared_ptr<rt::Material> glass = std::make_shared<rt::Material>();
    glass->ambient = { 0.1, 0.1, 0.1 };
    glass->diffuse = { 0.1, 0.1, 0.1 };
    glass->specular = { 0.2, 0.2, 0.2 };
    glass->shininess = 40.0;
    glass->reflection_amount = 0.3;
    glass->transparent = true;
    glass->refraction_amount = 0.7;
    glass->refraction_index = 1.5;

    std::shared_ptr<rt::Material> mirror = std::make_shared<rt::Material>();
    mirror->ambient = { 0.2, 0.2, 0.2 };
    mirror->diffuse = { 0.2, 0.2, 0.2 };
    mirror->specular = { 0.6, 0.6, 0.6 };
    mirror->shininess = 30.0;
    mirror->reflection_amount = 1.0;
    mirror->transparent = false;

    rt::Object box1 = rt::Object(rt::Mesh::getUnityCube(), bronze);
    box1.setPosition({0.0, 0.0, 4.0});
    box1.setScale({ 2.0, 2.0, 2.0 });
    box1.setRotation(45.0, { 0.0, 1.0, 0.0 });
    scene->objects.push_back(box1);

    rt::Object box2 = rt::Object(rt::Mesh::getUnityCube(), bronze);
    box2.setPosition({ 0.0, 0.0, -4.0 });
    box2.setScale({ 2.0, 2.0, 2.0 });
    box2.setRotation(-45.0, { 0.0, 1.0, 0.0 });
    //scene->objects.push_back(box2);

    rt::Object sphere1 = rt::Object(rt::Mesh::getUnitySphere(), bronze);
    sphere1.setPosition({ 0.0, 0.0, -4.0 });
    sphere1.setScale({ 2.0, 2.0, 2.0 });
    sphere1.setRotation(-20.0, { 0.0, 1.0, 0.0 });
    scene->objects.push_back(sphere1);

    rt::Object glass1 = rt::Object(rt::Mesh::getUnitySphere(), glass);
    glass1.setPosition({ 5.0, 2.0, 3.0 });
    glass1.setScale({2.0, 2.0, 2.0});
    //glass1.setRotation(0.0, { 0.0, 1.0, 0.0 });
    scene->objects.push_back(glass1);

    rt::Object glass2 = rt::Object(rt::Mesh::getUnityCube(), glass);
    glass2.setPosition({ 4.0, 0.0, 4.0 });
    glass2.setScale({ 0.2, 4.0, 4.0 });
    //glass1.setRotation(0.0, { 0.0, 1.0, 0.0 });
    //scene->objects.push_back(glass2);

    rt::Object mirror1 = rt::Object(rt::Mesh::getUnityCube(), mirror);
    mirror1.setPosition({ -5.0, 0.0, 4.0 });
    mirror1.setScale({ 0.1, 8.0, 8.0 });
    mirror1.setRotation(15.0, { 0.0, 1.0, 0.0 });
    //scene->objects.push_back(mirror1);

    rt::Object sphere2 = rt::Object(rt::Mesh::getUnitySphere(), mirror);
    sphere2.setPosition({ -8.0, -4.0, 0.0 });
    sphere2.setScale({ 4.0, 4.0, 4.0 });
    //scene->objects.push_back(sphere2);
}

int main(int argc, char** argv)
{
    rt::RaytracerSimple tracer = rt::RaytracerSimple();
    tracer.scene = std::make_shared<rt::Scene>(rt::Scene());
    setup(tracer.scene);
    tracer.scene->transform();

    std::shared_ptr<QImage> image = std::make_shared<QImage>(QImage(1920, 1080, QImage::Format::Format_RGB888));
    std::shared_ptr<ImageTarget> target = std::make_shared<ImageTarget>(ImageTarget(image));

    QTime timer = QTime();
    timer.start();

    tracer.render(target);

    unsigned long elapsed = timer.elapsed();

    image->save("example.png", "PNG");

    std::cout << elapsed << std::endl;
    system("PAUSE");
}
