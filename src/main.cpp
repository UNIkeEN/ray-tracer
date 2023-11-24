//
// Created by UNIkeEN on 23-11-24.
//

#include "camera.h"
#include "scene.h"
#include "vector3.h"
#include "utils.h"
#include "material.h"
#include "sphere.h"

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
    int image_height = 900;
    int image_width = 1200;

    // Configs of renderer
    int max_ray_bounces = 10;
    int spp = 100;

    // Configs of camera
    int vfov = 40;
    Vector3 lookfrom = Vector3(0, 0, -1);
    Vector3 lookat = Vector3(0, 0, 0);
    Vector3 background_color = Vector3(0 ,0, 0);
    
    // Create Scene
    Scene scene;

    auto* mat1 = new Lambertian(Vector3(0.8, 0.1, 0.1));

    auto* s1 = new Sphere(Vector3(0, 5, 20), 4.0, new BasicLight());
    auto* s2 = new Sphere(Vector3(0, -5, 20), 4.0, mat1);

    scene.add(s1);
    scene.add(s2);

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