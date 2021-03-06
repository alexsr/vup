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
#include <vup/Voxelization/bounding_box_utils.h>

int main() {
    const int width = 800;
    const int height = 600;
    vup::Window window(width, height, "Calculate mesh bounding box", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    vup::Compute_shader calc_box1024("../../src/shader/bounding_box/calc_aabb_world.comp", {{"N", "1024"}});
    vup::Compute_shader calc_box64("../../src/shader/bounding_box/reduce_aabb.comp", {{"N", "64"}});
    vup::Compute_shader transform_vertices("../../src/shader/compute/transform_vertices.comp");
    vup::V_F_shader minimal("../../src/shader/rendering/mvp_ubo.vert",
                            "../../src/shader/rendering/normal_rendering.frag");
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    vup::MVP mats{glm::mat4(1.0f), cam.get_view(), cam.get_projection()};
    vup::UBO mvp_ubo(mats, 8);
    mvp_ubo.update_data(mats);
    const float delta = 0.001f;
    auto max_blocks = static_cast<int>(glm::ceil(bunny.get_count() / calc_box1024.get_workgroup_size_x()));
    calc_box1024.update_uniform("max_index", bunny.get_count());
    calc_box1024.update_uniform("max_blocks", max_blocks);
    std::cout << "Max blocks " << max_blocks << "\n";
    const auto max_last_blocks = static_cast<int>(glm::ceil(max_blocks / calc_box64.get_workgroup_size_x()));
    calc_box64.update_uniform("max_index", max_blocks);
    calc_box64.update_uniform("max_blocks", max_last_blocks);
    std::vector<vup::vox::Bounds> intermediate_bounds(static_cast<unsigned long>(max_blocks));
    vup::SSBO bounds_ssbo(intermediate_bounds, 4, vup::gl::storage::read);
    bunny.get_vbo(0)->bind_base(5);
    calc_box1024.run(bunny.get_count());
    calc_box64.run(max_blocks);
    intermediate_bounds.resize(static_cast<unsigned long>(max_last_blocks));
    bounds_ssbo.get_data(intermediate_bounds);
    const auto bounds = vup::vox::reduce_bounds(intermediate_bounds);
    glm::vec3 center((bounds.min.x + bounds.max.x) / 2.0f, (bounds.min.y + bounds.max.y) / 2.0f,
                     (bounds.min.z + bounds.max.z) / 2.0f);
    transform_vertices.update_uniform("model", glm::translate(glm::mat4(1.0f), -center));
    transform_vertices.run(bunny.get_count());
    auto bounds_vbo = std::make_shared<vup::VBO>(bounds);
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
        mats.update(rotation, cam.get_view(), cam.get_projection());
        mvp_ubo.update_data(mats);
        auto start = std::chrono::system_clock::now();
        calc_box1024.run(bunny.get_count());
        calc_box64.run(max_blocks);
        bounds_ssbo.get_data(intermediate_bounds);
        elapsed += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
        bounds_vbo->update_data(vup::vox::reduce_bounds(intermediate_bounds));
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
