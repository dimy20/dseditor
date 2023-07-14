
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glad/glad.h>

#include "input.h"
#include "utils.h"
#include "shader.h"
#include "texture.h"
#include "plane.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"
#include "RenderContext.h"

#include <glm/matrix.hpp>
#include <iostream>

Camera camera(glm::vec3(0.0, 1.0, 5.0),
              glm::vec3(0.0, 0.0, -1.0),
              glm::vec3(0.0, 1.0, 0.0));

void update(){
    camera.update();
};

int main(){
    RobyRender::init();
    Plane plane;
    Model cube(Transform(glm::vec3(0.0), glm::vec3(0.5), Rotation(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0))));

    if(!load_model(&cube, "./assets/cube.obj")){
        fprintf(stderr, "Error: failed to load cube model\n");
        exit(1);
    }
    if(!plane.is_loaded()){
        fprintf(stderr, "Error: failed to load plane\n");
        exit(1);
    }

    while(RobyInput::is_running){
        RobyRender::clear();
        RobyInput::process();

        update();

        //draw
        plane.draw(camera.view());
        cube.draw(camera.view());

        RobyRender::swap_buffers();
    };

    RobyRender::quit();
}
