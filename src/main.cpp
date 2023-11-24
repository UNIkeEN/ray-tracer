//
// Created by UNIkeEN on 23-11-24.
//

#include "camera.h"
#include "scene.h"
#include "vector3.h"
#include "utils.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"

void write_image(const int& height, const int& width, std::vector<Vector3>& buffer) {
    
    FILE* fp = fopen("binary.ppm", "wb");
    fprintf(fp, "P3\n%d %d\n255\n", width, height);

    for (int i=0; i < height * width; i++){
        Vector3 pixel_color = buffer[i];
        double r = sqrt(pixel_color.x); //sqrt makes linear to gamma
        double g = sqrt(pixel_color.y);
        double b = sqrt(pixel_color.z);
        fprintf(fp, "%d %d %d\n", 
            static_cast<int>(256 * clamp(r, 0, 1-EPSILON)),
            static_cast<int>(256 * clamp(g, 0, 1-EPSILON)),
            static_cast<int>(256 * clamp(b, 0, 1-EPSILON))
        );
    }
    fclose(fp);
}

int main(){

    // Configs of image
    int image_height = 784;
    int image_width = 784;

    // Configs of renderer
    int max_ray_bounces = 50;
    int spp = 1000;

    // Configs of camera
    int vfov = 40;
    Vector3 lookfrom = Vector3(278, 278, -300);
    Vector3 lookat = Vector3(278, 278, 0);
    Vector3 background_color = Vector3(0 ,0, 0);
    
    // Create Scene
    Scene scene;

    //cornell box
    auto* mat_red = new Lambertian(Vector3(0.65f, 0.05f, 0.05f));
    auto* mat_green = new Lambertian(Vector3(0.12f, 0.45f, 0.15f));
    auto* mat_white = new Lambertian(Vector3(0.73f, 0.73f, 0.73f));

    MeshTriangle floor("../models/cornellbox/floor.obj", mat_white);
    MeshTriangle shortbox("../models/cornellbox/shortbox.obj", mat_white) ;
    MeshTriangle tallbox("../models/cornellbox/tallbox.obj", mat_white);
    MeshTriangle left("../models/cornellbox/left.obj", mat_red);
    MeshTriangle right("../models/cornellbox/right.obj", mat_green);
    MeshTriangle toplight("../models/cornellbox/light.obj", new BasicLight());

    Sphere test(Vector3(278, 278, 200), 100, mat_red);

    scene.add(&floor);
    scene.add(&shortbox);
    scene.add(&tallbox);
    scene.add(&left);
    scene.add(&right);
    scene.add(&toplight);
//    scene.add(&test);

    // Create Camera
    Camera camera(image_height, image_width);
    camera.vfov = vfov;
    camera.max_depth = max_ray_bounces;
    camera.spp = spp;
    camera.lookfrom = lookfrom;
    camera.lookat = lookat;
    camera.background_color = background_color;

    // Render and output
    std::vector<Vector3> rendered_image = camera.render(scene);
    write_image(image_height, image_width, rendered_image); 

    return 0;
}