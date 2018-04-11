#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
/*

*/

class GameObject
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 eulerRotation;
    };

    //generate the modelToWorld matrix;
    glm::mat4 modelToWorld(const Transform t)
    {
        auto m = glm::mat4(1);      //origin
        m = glm::scale(m, scale);   //scale


        m = glm::rotate(m, eulerRotation.x, glm::vec3{ 1,0,0 });
        m = glm::rotate(m, eulerRotation.y, glm::vec3{ 0,1,0 });
        m = glm::rotate(m, eulerRotation.z, glm::vec3{ 0,0,1 });
        m = glm::translate(m, position);
        return m;
    }


    /*
    1,      0,       0,      0,
    0,      cos(r), -sin(r), 0,
    0,      sin(r),  cos(r), 0,
    0,      0,       0,      1


    cos(r), 0,     -sin(r), 0,
    0,      1,      0,      0,
    sin(r), 0,      cos(r), 0,
    0,      0,      0,      1

    (cos(y+z)+cos(y-z))/2,                                                      (-sin(y+z)+sin(y-z))/2,                                                         -sin(y),                    0,
    (cos(x+y+z)-cos(x-y+z)+cos(x+y-z)-cos(x-y-z)+2*sin(x+z)-2*sin(x-z))/4,      (2*cos(x+z)+2*cos(x-z)-sin(x+y+z)+sin(x-y+z)+sin(x+y-z)-sin(x-y-z))/4,          (-sin(x+y)-sin(x-y))/2,     0,
    (-2*cos(x+z)+2*cos(x-z)+sin(x+y+z)-sin(x-y+z)+sin(x+y-z)-sin(x-y-z))/4,     (cos(x+y+z)-cos(x-y+z)-cos(x+y-z)+cos(x-y-z)+2*sin(x+z)+2*sin(x-z))/4,          (cos(x+y)+cos(x-y))/2,      0,
    0                                                                           0,                                                                              0,                          1

    */

private:

public:
}