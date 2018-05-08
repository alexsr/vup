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

int main() {
    const auto width = 800;
    const auto height = 600;
    vup::Window window(width, height, "Initial instanced rendering demo", true);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    vup::V_F_shader minimal("../../src/shader/rendering/mvp_ubo.vert",
                            "../../src/shader/rendering/normal_rendering.frag");
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    const vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    vup::VAO vao(bunny);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::MVP mats{glm::mat4(1.0f), cam.get_view(), cam.get_projection()};
    vup::UBO mvp(mats, 0);

    const auto loop = [&](float dt) {
        minimal.use();
        cam.update(window, dt);
        mats.view = cam.get_view();
        mats.projection = cam.get_projection();
        mvp.update_data(mats);
        vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };

    window.run_loop_fixed(0.01f, loop);
    return 0;
}
