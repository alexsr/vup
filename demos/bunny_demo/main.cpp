//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Rendering/V_F_shader_program.h>
#include <vup/Geometry/Mesh_loader.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/GPU_Storage/Storage_buffer.h>

struct MVP {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    void update(glm::mat4 m, glm::mat4 v, glm::mat4 p) {
        model = m;
        view = v;
        projection = p;
    }
};

int main() {
    vup::init_GLFW();
    vup::Window window(800, 600, "Initial instanced rendering demo", true);
    vup::Trackball_camera cam(800, 600);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/mvp_ubo.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/normal_rendering.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment);
    vup::Mesh_loader bunny("../../resources/meshes/bunny.obj");
    vup::VAO vao(bunny.get_mesh(0));
    vup::OpenGL_debug_logger gl_debug_logger;
    glm::mat4 model(1.0f);
    MVP mats{model, cam.get_view(), cam.get_projection()};
    vup::UBO mvp(mats, 0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    auto loop = [&](float dt) {
        minimal.use();
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        mvp.update_data(mats);
        vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(0.01f, loop);
    glfwTerminate();
    return 0;
}