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
#include <vup/Compute/Compute_shader_program.h>

struct MVP {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    void update(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) {
        model = m;
        view = v;
        projection = p;
    }
};

int main() {
    vup::init_GLFW();
    int width = 800;
    int height = 600;
    vup::Window window(width, height, "Calculate mesh bounding box", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    auto mesh_random_movement(std::make_shared<vup::Compute_shader>("../../src/shader/compute/calc_bounding_box.comp"));
    vup::Compute_shader_program move_verts(mesh_random_movement);
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/mvp_ubo.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/normal_rendering.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment,
                                    vup::gl::Introspection::ubos | vup::gl::Introspection::ssbos);
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    vup::VAO vao(bunny);
    bunny.get_VBO(0).bind_base(0);
    glm::mat4 model(1.0f);
    MVP mats{model, cam.get_view(), cam.get_projection()};
    bool allow_reset;
    float delta = 0.001f;
    move_verts.update_uniform("dt", 0.00001f);
    move_verts.run(bunny.get_count());
    auto loop = [&](float dt) {
        minimal.use();
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        minimal.update_ubo("mvp", mats);
        vao.render(GL_POINTS);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(delta, loop);
    glfwTerminate();
    return 0;
}