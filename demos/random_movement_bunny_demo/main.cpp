//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/Shader.h>
#include <vup/Geometry/Mesh_loader.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/Shader/Compute_shader.h>

int main() {
    int width = 800;
    int height = 600;
    vup::Window window(width, height, "Randomly move bunny vertices", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    vup::Compute_shader move_verts("../../src/shader/compute/randomly_move_verts.comp");
    vup::V_F_shader minimal("../../src/shader/rendering/mvp_ubo.vert",
                            "../../src/shader/rendering/normal_rendering.frag");
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    vup::VAO vao(bunny);
    bunny.get_vbo(0).bind_base(0);
    vup::SSBO random_pos(vup::generate_random_float_data(bunny.get_count() * 4, 0, 1.0f), 4);
    glm::mat4 model(1.0f);
    vup::MVP mats{model, cam.get_view(), cam.get_projection()};
    vup::UBO mvp_ubo(mats, 8);
    bool allow_reset;
    move_verts.update_uniform("dt", 0.0001f);
    auto loop = [&](float dt) {
        move_verts.run(bunny.get_count());
        minimal.use();
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        mvp_ubo.update_data(mats);
        vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_PRESS && allow_reset) {
            move_verts.reload();
            move_verts.update_uniform("dt", 0.0001f);
            random_pos.update_data(vup::generate_random_float_data(bunny.get_count() * 4, 0, 1.0f));
            allow_reset = false;
        }
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_RELEASE) {
            allow_reset = true;
        }
    };
    window.run_loop_fixed(0.01f, loop);
    glfwTerminate();
    return 0;
}
