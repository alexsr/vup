//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_TRACKBALL_CAMERA_H
#define VUP_TRACKBALL_CAMERA_H

#include <vup/Core/vup.h>
#include <vup/Core/Window.h>
#include <array>
#include <iostream>

namespace vup
{
    // Provides the functionality of a trackball camera.
    // The view matrix for the current camera view is calculated and
    // is accessable as well as the projection.
    // A lot of parameters of this camera can be specified.
    class Trackball_camera {
    public:
        // Creates an instance of a trackball camera
        // The default options proved to be reasonable
        // * int width - window width
        // * int height - window height
        // * float sens - mouse movement sensitivity
        // * float r - initial radius for camera
        // * float zoomsens - sensitivity for forward movement of the camera
        // * glm::vec3 center - center of the trackball camera
        // * float fov - field of view in degree
        // * float near - near value for projection
        // * float far - far value for projection
        Trackball_camera(int width, int height, float sens = 1.0f, float r = 2.0,
                         float zoom_sens = 1.0f, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f),
                         float fov = 60.0f, float near = 0.001f, float far = 1000.0f);

        ~Trackball_camera();

        glm::mat4 get_view() const;
        glm::mat4 get_projection() const;

        // Updates the camera view using mouse controls
        // * GLFWwindow* window - window to access mouse position and controls
        // * float dt - delta time between camera updates
        void update(Window window, float dt);
        void resize();

    private:
        glm::vec3 m_center;
        glm::mat4 m_view;
        glm::mat4 m_projection;
        float m_sens;
        float m_zoom_sens;
        float m_radius;
        float m_theta;
        float m_phi;
        double m_x;
        double m_y;
        float m_fov;
        float m_near;
        float m_far;
        int m_width;
        int m_height;

        void update_view();
        void move_camera(Window window, float dt);
    };
}

#endif //VUP_TRACKBALL_CAMERA_H
