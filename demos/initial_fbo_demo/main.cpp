//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/Shader.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Rendering/FBO.h>
#include <vup/Utility/OpenGL_debug_logger.h>

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
    int width = 800;
    int height = 600;
    vup::Window window(width, height, "Initial instanced rendering demo", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    vup::V_F_shader minimal("../../src/shader/rendering/gbuffer.vert", "../../src/shader/rendering/gbuffer.frag");
    vup::V_F_shader screenfilling_quad("../../src/shader/rendering/screenfilling_quad.vert",
                                       "../../src/shader/rendering/screenfilling_quad.frag");
    vup::Rectangle r(2.0f, 2.0f);
    vup::Cube q(1.0f);
    vup::VAO vao(q);
    vup::VAO r_vao(r);
    vup::Texture_definition t;
    vup::Texture_definition d{GL_TEXTURE_2D, vup::gl::Tex_format::depth, GL_R32F, vup::gl::Tex_type::f, true};
    vup::FBO fbo(width, height, {t, t, t, d});
    glm::mat4 model(1.0f);
    MVP mats{model, cam.get_view(), cam.get_projection()};
    vup::UBO mvp_ubo(mats, 9);
    fbo.get_texture(0).activate(0);
    screenfilling_quad.update_uniform("image", 0);
    auto loop = [&](float dt) {
        fbo.bind();
        vup::gl::clear_buffers();
        minimal.use();
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        mvp_ubo.update_data(mats);
        vao.render(GL_TRIANGLES);
        fbo.unbind();
        screenfilling_quad.use();
        r_vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(0.01f, loop);
    glfwTerminate();
    return 0;
}
