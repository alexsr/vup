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
    auto calc_box_shader(std::make_shared<vup::Compute_shader>("../../src/shader/compute/calc_bounding_box.comp"));
    vup::Compute_shader_program calc_box(calc_box_shader);
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/mvp_ubo.vert"));
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
    bunny.get_VBO(0).bind_base(0);
    glm::mat4 model(1.0f);
    MVP mats{model, cam.get_view(), cam.get_projection()};
    bool allow_reset;
    float delta = 0.001f;
    int max_blocks = static_cast<int>(glm::ceil(bunny.get_count() / calc_box.get_workgroup_size_x()));
    calc_box.update_uniform("dt", 0.00001f);
    calc_box.update_uniform("max_index", bunny.get_count());
    calc_box.update_uniform("max_blocks", max_blocks);
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
    glPointSize(5);
    calc_box.run(bunny.get_count());
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