//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/V_F_shader.h>
#include <vup/Shader/V_G_F_shader.h>
#include <vup/Geometry/Mesh_loader.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/Shader/Compute_shader.h>

struct Bounds {
    glm::vec4 min = glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, 1.0f);
    glm::vec4 max = glm::vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0f);
};

Bounds reduce_bounds(const std::vector<Bounds>& bounds) {
    Bounds result;
    for (const auto v : bounds) {
        if (v.min.x < result.min.x) {
            result.min.x = v.min.x;
        }
        if (v.min.y < result.min.y) {
            result.min.y = v.min.y;
        }
        if (v.min.z < result.min.z) {
            result.min.z = v.min.z;
        }
        if (v.max.x > result.max.x) {
            result.max.x = v.max.x;
        }
        if (v.max.y > result.max.y) {
            result.max.y = v.max.y;
        }
        if (v.max.z > result.max.z) {
            result.max.z = v.max.z;
        }
    }
    return result;
}

int main() {
    const int width = 800;
    const int height = 600;
    vup::Window window(width, height, "Calculate mesh bounding box", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    vup::Compute_shader calc_box1024("../../src/shader/bounding_box/calc_bounding_box.comp",
                                     vup::gl::introspection::basic, {{"N", "1024"}});
    vup::Compute_shader calc_box64("../../src/shader/bounding_box/reduce_aabb.comp",
                                   vup::gl::introspection::basic, {{"N", "64"}});
    vup::V_F_shader minimal("../../src/shader/rendering/mvp_ubo.vert", "../../src/shader/rendering/normal_rendering.frag",
                            vup::gl::introspection::ubos | vup::gl::introspection::ssbos);
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    glm::mat4 model(1.0f);
    vup::MVP mats{model, cam.get_view(), cam.get_projection()};
    bool allow_reset;
    const float delta = 0.001f;
    auto max_blocks = static_cast<int>(glm::ceil(bunny.get_count() / calc_box1024.get_workgroup_size_x()));
    minimal.update_ubo("mvp", mats);
    calc_box1024.update_uniform("max_index", bunny.get_count());
    calc_box1024.update_uniform("max_blocks", max_blocks);
    std::cout << "Max blocks " << max_blocks << "\n";
    calc_box64.update_uniform("max_index", max_blocks);
    const auto max_last_blocks = static_cast<int>(glm::ceil(max_blocks / calc_box64.get_workgroup_size_x()));
    calc_box64.update_uniform("max_blocks", max_last_blocks);
    std::vector<Bounds> intermediate_bounds(static_cast<unsigned long>(max_blocks));
    vup::SSBO bounds_ssbo(intermediate_bounds, 4, vup::gl::storage::read);
    bunny.get_vbo(0).bind_base(5);
    calc_box1024.run(bunny.get_count());
    calc_box64.run(max_blocks);
    intermediate_bounds.resize(static_cast<unsigned long>(max_last_blocks));
    bounds_ssbo.get_data(intermediate_bounds);
    const auto bounds = reduce_bounds(intermediate_bounds);
    glm::vec3 center((bounds.min.x + bounds.max.x) / 2.0f, (bounds.min.y + bounds.max.y) / 2.0f,
                     (bounds.min.z + bounds.max.z) / 2.0f);
    vup::VBO bounds_vbo(bounds, 4);
    vup::VAO bounds_vao(bounds_vbo);
    vup::V_G_F_shader bounds_renderer("../../src/shader/bounding_box/mvp_ubo_aabb.vert",
                                      "../../src/shader/bounding_box/aabb.geom",
                                      "../../src/shader/rendering/minimal.frag");
    auto elapsed = std::chrono::microseconds(0);
    glm::mat4 rotation(1.0f);
    vup::VAO vao(bunny);
    float accum = 0;
    const auto loop = [&](float dt) {
        accum += 1;
        cam.update(window, dt);
        rotation = glm::rotate(rotation, dt, glm::vec3(0, 1, 0));
        model = glm::translate(rotation, -center);
        mats.update(model, cam.get_view(), cam.get_projection());
        minimal.update_ubo("mvp", mats);
        auto start = std::chrono::system_clock::now();
        calc_box1024.run(bunny.get_count());
        calc_box64.run(max_blocks);
        bounds_ssbo.get_data(intermediate_bounds);
        elapsed += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
        bounds_vbo.update_data(reduce_bounds(intermediate_bounds));
        minimal.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        vao.render(GL_TRIANGLES);
        bounds_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_LINES);
        gl_debug_logger.retrieve_log(std::cout);
        if (accum > 1000) {
            std::cout << "Elapsed time for AABB calculation per frame: " << elapsed.count() / accum << "us\n";
            elapsed = std::chrono::microseconds(0);
            accum = 0;
        }
    };
    window.run_loop_fixed(delta, loop);
    glfwTerminate();
    return 0;
}
