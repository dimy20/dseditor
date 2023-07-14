#include "transform.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

std::ostream& operator << (std::ostream& out, const glm::vec3& v){
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}
std::ostream& operator << (std::ostream& out, const glm::mat4& mat){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            out << mat[i][j] << " ";
        }
        std::cout << "\n";
    }
    return out;
}

glm::mat4 Transform::world() const{
    glm::mat4 world(1.0);
    world = glm::translate(world, translation);
    world = glm::rotate(world, rotation.angle, rotation.axis);
    world = glm::scale(world, scale);
    return world;
}
