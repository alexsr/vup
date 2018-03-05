//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Simulation/particle_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/V_F_shader.h>
#include <vup/Shader/V_G_F_shader.h>
#include <vup/Geometry/Mesh_loader.h>
#include <vup/GPU_Storage/Instanced_VAO.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/Shader/Compute_shader.h>

static float cubic(float h, float r) {
    float res = 0.0;
    float q = r / h;
    if (q <= 1.0) {
        if (q <= 0.5) {
            float q2 = q * q;
            float q3 = q2 * q;
            res = 8.0 / (glm::pi<float>()* h * h * h) * (6.0*q3 - 6.0*q2 + 1.0);
        }
        else {
            res = 8.0 / (glm::pi<float>()* h * h * h) * (2.0*glm::pow(1.0 - q, 3));
        }
    }
    return res;
}

static glm::vec4 cubic_grad(float h, glm::vec4 v) {
    glm::vec4 res;
    float rl = glm::length(v);
    float q = rl / h;
    if (q <= 1.0) {
        if (rl > 1.0e-6) {
            glm::vec4 gradq = v * (1.0f / (rl*h));
            if (q <= 0.5) {
                res = 48.0f / (glm::pi<float>()* h * h * h)  * q*(3.0f*q - 2.0f)*gradq;
            }
            else {
                float factor = 1.0 - q;
                res = 48.0f / (glm::pi<float>()* h * h * h)  * (-factor * factor)*gradq;
            }
        }
    }
    else {
        res = glm::vec4(0);
    }
    return res;
}

float poly_six(float h, float r) {
    r = abs(r);
    if (0 <= r && r < h) {
        return pow(h * h - r * r, 3);
    }
    return 0.0f;
};

glm::vec4 spiky_gradient(float h, glm::vec4 v) {
    float r = abs(glm::length(v));
    if (0 <= r && r < h) {
        return v * (h - r) * (h - r) * (h - r) / r;
    }
    return glm::vec4(0.0f);
};

float visc(float h, glm::vec4 v) {
    float r = abs(glm::length(v));
    if (0 <= r && r < h) {
        return h - r;
    }
    return 0.0f;
};

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
    vup::Window window(width, height, "IISPH demo", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, width, height);
    window.set_resize(resize_callback);
    bool allow_reset = true;
    float delta = 0.01f;
    vup::Cube bounds_cube(2.0f, 2.0f, 2.0f);
    vup::VAO bounds_vao(bounds_cube);
    vup::V_F_shader box_renderer("../../src/shader/mvp_ubo.vert",
        "../../src/shader/minimal.frag");
    glm::mat4 model(1.0f);
    glm::mat4 bb_model(1.0f);
    MVP mats{ glm::mat4(1.0f), cam.get_view(), cam.get_projection() };
    float radius = 0.05;
    vup::Sphere sphere(radius);
    vup::Instanced_VAO particle_spheres(sphere);
    vup::SPH_demo_constants demo_consts(radius, 4.0f * radius);
    auto p = vup::create_uniform_IISPH_particles(radius, -0.3f, 0.3f,
        1000.0f,
        radius*2.0f);
    vup::SSBO particles(p, 3);
    auto instances = static_cast<int>(p.size());
    std::vector<float> new_densities(instances);
    vup::SSBO densities(new_densities, 7, vup::gl::Storage::read);

    vup::V_F_shader particle_shader("../../src/shader/particles/iisph/instanced_iisph.vert", "../../src/shader/particles/particles.frag",
        vup::gl::Introspection::ubos, { {"N", std::to_string(instances)} });
    particle_shader.update_ubo("mvp", mats);

    //vup::Compute_shader iisph_init("../../src/shader/particles/iisph/iisph_iteration_init.comp",
    //    vup::gl::Introspection::ubos, { {"N", instances} });
    //iisph_init.update_ubo("demo_constants", demo_consts);
    //iisph_init.update_uniform("dt", delta);

    //vup::Compute_shader iisph_iteration("../../src/shader/particles/iisph/iisph_iteration.comp",
    //    vup::gl::Introspection::basic, { {"N", instances} });
    //iisph_iteration.update_uniform("dt", delta);

    //vup::Compute_shader iisph_integrate("../../src/shader/particles/iisph/integrate_iisph.comp",
    //    vup::gl::Introspection::basic, { {"N", instances} });
    //iisph_integrate.update_uniform("dt", delta);

    //vup::Compute_shader reduce_densities("../../src/shader/particles/iisph/reduce_densities.comp",
    //    vup::gl::Introspection::basic, { {"N", instances} });
    //int max_blocks = static_cast<int>(glm::ceil(instances / reduce_densities.get_workgroup_size_x()));
    //reduce_densities.update_uniform("max_index", instances);
    //reduce_densities.update_uniform("max_blocks", max_blocks);
    //new_densities.resize(max_blocks);

    vup::Compute_shader rotate_box("../../src/shader/compute/rotate_box.comp");
    bounds_vao.get_vbo(0).bind_base(12);
    bb_model = glm::rotate(bb_model, delta * 0.1f, glm::vec3(0.3, 0.3f, 0.3f));
    rotate_box.update_uniform("model", bb_model);

    float density_avg = 0.0f;
    float density_rest = 1000.0f;
    float eta = 10.0f;

    int neighbor_amount = 50;
    std::vector<int> neighbors(neighbor_amount * instances);
    std::vector<int> neighbor_counters(instances);

    using namespace glm;

    auto loop = [&](float dt) {
        rotate_box.run(bounds_cube.vertices.size());
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());

        for (int id = 0; id < instances; id++) {
            int neighbor_counter = 0;
            for (int index = 0; index < instances; index++) {
                if (index == id) {
                    continue;
                }
                if (distance(p[id].pos, p[index].pos) <= demo_consts.h) {
                    neighbors[id * neighbor_amount + neighbor_counter] = index;
                    neighbor_counter++;
                }
                if (neighbor_counter >= neighbor_amount - 1) {
                    break;
                }
            }
            neighbor_counters[id] = neighbor_counter;
        }
        /* predict advection */
        for (int id = 0; id < instances; id++) {
            float density_id = p[id].mass * cubic(demo_consts.h, 0.0);
            for (int i = 0; i < neighbor_counters[id]; i++) {
                int j = neighbors[id * neighbor_amount + i];
                density_id += p[j].mass * cubic(demo_consts.h, distance(p[id].pos, p[j].pos));
            }
            p[id].density = max(density_id, p[id].rest_density);
        }
        for (int id = 0; id < instances; id++) {
            p[id].dii = vec4(0.0);
            float visc_const = 0.0f;
            vec4 visc_force = vec4(0.0f);
            for (int i = 0; i < neighbor_counters[id]; i++) {
                int j = neighbors[id * neighbor_amount + i];
                vec4 pij = p[id].pos - p[j].pos;
                //visc_force += p[j].mass * (p[id].vel - p[j].vel) / p[j].density * visc(demo_consts.h, pij);
                p[id].dii -= p[j].mass / (p[j].density * p[j].density) * cubic_grad(demo_consts.h, pij);
            }
            vec4 extern_force = vec4(0.0f);
            extern_force.y = -9.81f * p[id].mass;
            p[id].vel_adv = p[id].vel + dt * (visc_force * visc_const * demo_consts.spiky + extern_force) / p[id].mass;
        }

        /* init pressure solver */
        for (int id = 0; id < instances; id++) {
            p[id].density_adv = p[id].density;
            p[id].aii = 0;
            p[id].pressure = 0.5f * p[id].last_pressure;
            for (int i = 0; i < neighbor_counters[id]; i++) {
                int j = neighbors[id * neighbor_amount + i];
                vec4 pij = p[id].pos - p[j].pos;
                vec4 vij = p[id].vel_adv - p[j].vel_adv;
                p[id].density_adv += dt * p[j].mass * dot(vij, cubic_grad(demo_consts.h, pij));
                p[id].aii += p[j].mass * dot(p[id].dii - p[id].mass / (p[id].density * p[id].density)
                    * cubic_grad(demo_consts.h, pij), cubic_grad(demo_consts.h, pij));
            }
        }
        int iteration = 0;
        while ((density_avg - density_rest > eta || iteration < 2) && iteration < 20) {
            density_avg = 0.0f;
            for (int id = 0; id < instances; id++) {
                p[id].dij_pj_sum = vec4(0.0f);
                for (int i = 0; i < neighbor_counters[id]; i++) {
                    int j = neighbors[id * neighbor_amount + i];
                    vec4 pij = p[id].pos - p[j].pos;
                    p[id].dij_pj_sum -= p[j].mass / (p[j].density * p[j].density) * p[j].last_pressure *  cubic_grad(demo_consts.h, pij);
                }
            }
            for (int id = 0; id < instances; id++) {
                p[id].aij_pj_sum = 0;
                for (int i = 0; i < neighbor_counters[id]; i++) {
                    int j = neighbors[id * neighbor_amount + i];
                    vec4 pij = p[id].pos - p[j].pos;
                    p[id].aij_pj_sum += p[j].mass * dot(p[id].dij_pj_sum
                        - p[j].dii * p[j].last_pressure - p[j].dij_pj_sum
                        + p[id].mass / (p[id].density * p[id].density)
                        * cubic_grad(demo_consts.h, pij) * p[id].last_pressure, cubic_grad(demo_consts.h, pij));
                }
                float w = 0.5f;
                p[id].pressure = (1.0f - w) * p[id].last_pressure + w / (dt * dt * p[id].aii) * (p[id].rest_density - p[id].density_adv - dt * dt * p[id].aij_pj_sum);
                new_densities[id] = dt * dt * (p[id].aii * p[id].pressure + p[id].aij_pj_sum) + p[id].density_adv;
            }
            for (int i = 0; i < new_densities.size(); i++) {
                if (isnan(new_densities[i])) {
                    new_densities[i] = 0.0f;
                }
                density_avg += new_densities[i];
                p[i].last_pressure = p[i].pressure;
            }
            density_avg /= instances;
            iteration++;
            //std::cout << "Density average: " << density_avg << "\n";
        }
        for (int id = 0; id < instances; id++) {
            p[id].pressure_force = vec4(0.0f);
            for (int i = 0; i < neighbor_counters[id]; i++) {
                int j = neighbors[id * neighbor_amount + i];
                vec4 pij = p[id].pos - p[j].pos;
                p[id].pressure_force -= p[j].mass * (p[id].pressure / (p[id].density * p[id].density) + p[j].pressure / (p[j].density * p[j].density)) * cubic_grad(demo_consts.h, pij);
            }
            vec4 a = p[id].pressure_force;
            p[id].vel = p[id].vel_adv + a * dt;
            p[id].pos += p[id].vel * dt;
        }
        for (int id = 0; id < instances; id++) {
            if (p[id].pos.y < -0.5f) {
                p[id].pos.y = -0.5f;
                p[id].vel.y *= -1.0f;
            }
        }
        particles.update_data(p);
        particle_shader.update_ubo("mvp", mats);
        particle_shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        particle_spheres.render(GL_TRIANGLES,
                                static_cast<unsigned int>(instances));
        box_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(delta, loop);
    glfwTerminate();
    return 0;
}