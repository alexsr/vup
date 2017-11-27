//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Rendering/V_F_shader_program.h>
#include <vup/Rendering/V_G_F_shader_program.h>
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
    auto calc_box_shader1024(std::make_shared<vup::Compute_shader>("../../src/shader/compute/calc_bounding_box.comp"));
    vup::Compute_shader_program calc_box1024(calc_box_shader1024);
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/aabb_move_bunny.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/normal_rendering.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment,
                                    vup::gl::Introspection::ubos | vup::gl::Introspection::ssbos);
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    std::vector<glm::vec4> bunny_verts = bunny_loader.get_mesh_data(0).vertices;
    float min_x = FLT_MAX;
    float min_y = FLT_MAX;
    float min_z = FLT_MAX;
    float max_x = -FLT_MAX;
    float max_y = -FLT_MAX;
    float max_z = -FLT_MAX;
    for (auto v : bunny_verts) {
        if (v.x < min_x) {
            min_x = v.x;
        }
        if (v.y < min_y) {
            min_y = v.y;
        }
        if (v.z < min_z) {
            min_z = v.z;
        }
        if (v.x > max_x) {
            max_x = v.x;
        }
        if (v.y > max_y) {
            max_y = v.y;
        }
        if (v.z > max_z) {
            max_z = v.z;
        }
    }
    std::cout << "Lower bounds: " << min_x << ", " << min_y << ", " << min_z << "\n";
    std::cout << "Upper bounds: " << max_x << ", " << max_y << ", " << max_z << "\n";
    vup::VAO vao(bunny);
    vup::SSBO current_bunny(bunny_verts, 5);
    glm::mat4 model(1.0f);
    MVP mats{model, cam.get_view(), cam.get_projection()};
    bool allow_reset;
    float delta = 0.001f;
    int max_blocks = static_cast<int>(glm::ceil(bunny.get_count() / calc_box1024.get_workgroup_size_x()));
    calc_box1024.update_uniform("dt", 0.00001f);
    calc_box1024.update_uniform("max_index", bunny.get_count());
    calc_box1024.update_uniform("max_blocks", max_blocks);
    std::cout << "Max blocks " << max_blocks << "\n";
    struct Bounds {
        float min_x;
        float max_x;
        float min_y;
        float max_y;
        float min_z;
        float max_z;
    };
    std::vector<Bounds> bounds(static_cast<unsigned long>(max_blocks));
    vup::SSBO bounds_ssbo(bounds, 4, vup::gl::Storage::read);
    auto start = std::chrono::system_clock::now();
    calc_box1024.run(bunny.get_count());
    glFinish();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
    std::cout << "Duration for workgroup size " << calc_box1024.get_workgroup_size_x() << ": " << elapsed.count() << "\xE6s\n";
    bounds_ssbo.get_data(bounds);
    min_x = FLT_MAX;
    min_y = FLT_MAX;
    min_z = FLT_MAX;
    max_x = -FLT_MAX;
    max_y = -FLT_MAX;
    max_z = -FLT_MAX;
    for (auto v : bounds) {
        if (v.min_x < min_x) {
            min_x = v.min_x;
        }
        if (v.min_y < min_y) {
            min_y = v.min_y;
        }
        if (v.min_z < min_z) {
            min_z = v.min_z;
        }
        if (v.max_x > max_x) {
            max_x = v.max_x;
        }
        if (v.max_y > max_y) {
            max_y = v.max_y;
        }
        if (v.max_z > max_z) {
            max_z = v.max_z;
        }
    }
    std::cout << "Lower bounds: " << min_x << ", " << min_y << ", " << min_z << "\n";
    std::cout << "Upper bounds: " << max_x << ", " << max_y << ", " << max_z << "\n";
    std::vector<float> bounds_points{min_x, min_y, min_z, 1.0f, max_x, max_y, max_z, 1.0f};
    glm::vec3 center((min_x + max_x) / 2.0f, (min_y + max_y) / 2.0f, (min_z + max_z) / 2.0f);
    model = glm::translate(glm::mat4(1.0f), -center);
    vup::VBO bounds_vbo(bounds_points, 4);
    vup::VAO bounds_vao(bounds_vbo);
    auto bounds_vert = std::make_shared<vup::Vertex_shader>("../../src/shader/mvp_ubo_aabb.vert");
    auto bounds_geom = std::make_shared<vup::Geometry_shader>("../../src/shader/geometry/aabb.geom");
    auto bounds_frag = std::make_shared<vup::Fragment_shader>("../../src/shader/minimal.frag");
    vup::V_G_F_shader_program bounds_renderer(bounds_vert, bounds_geom, bounds_frag, vup::gl::Introspection::ubos);
    elapsed = std::chrono::microseconds(0);
    float accum = 0;
    auto loop = [&](float dt) {
        accum += 1;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        minimal.use();
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        bounds_renderer.update_ubo("mvp", mats);
        minimal.update_ubo("mvp", mats);
        vao.render(GL_TRIANGLES);
        glFinish();
        start = std::chrono::system_clock::now();
        calc_box1024.run(bunny.get_count());
        glFinish();
        bounds_ssbo.get_data(bounds);
        min_x = FLT_MAX;
        min_y = FLT_MAX;
        min_z = FLT_MAX;
        max_x = -FLT_MAX;
        max_y = -FLT_MAX;
        max_z = -FLT_MAX;
        for (auto v : bounds) {
            if (v.min_x < min_x) {
                min_x = v.min_x;
            }
            if (v.min_y < min_y) {
                min_y = v.min_y;
            }
            if (v.min_z < min_z) {
                min_z = v.min_z;
            }
            if (v.max_x > max_x) {
                max_x = v.max_x;
            }
            if (v.max_y > max_y) {
                max_y = v.max_y;
            }
            if (v.max_z > max_z) {
                max_z = v.max_z;
            }
        }
        elapsed += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
        bounds_points = {min_x, min_y, min_z, 1.0f, max_x, max_y, max_z, 1.0f};
        bounds_vbo.update_data(bounds_points);
        bounds_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_LINES);
        gl_debug_logger.retrieve_log(std::cout);
        if (accum > 1000) {
            std::cout << "Elapsed time for AABB calculation per frame: " << elapsed.count() / accum << "\xE6s\n";
            elapsed = std::chrono::microseconds(0);
            accum = 0;
        }
    };
    window.run_loop_fixed(delta, loop);
    glfwTerminate();
    return 0;
}