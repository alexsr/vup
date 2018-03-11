//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_IMGUI_UTILS_H
#define VUP_IMGUI_UTILS_H

#include "vup.h"
#include <imgui/imgui.h>

namespace vup
{
    namespace gui
    {
        // GLFW data
        static GLFWwindow* g_window = NULL;
        static double g_time = 0.0f;
        static bool g_mouse_just_pressed[3] = { false, false, false };
        static GLFWcursor* g_mouse_cursors[ImGuiMouseCursor_COUNT] = { nullptr };

        // OpenGL460 data
        static char g_GlslVersion[32] = "#version 460\n";
        static GLuint g_font_texture = 0;
        static int g_shader_handle = 0, g_VertHandle = 0, g_frag_handle = 0;
        static int g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
        static int g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
        static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

        // OpenGL3 Render function.
        // (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
        // Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so. 
        inline void render_draw_data() {

            ImGui::Render();
            auto draw_data = ImGui::GetDrawData();
            // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
            auto& io = ImGui::GetIO();
            const auto fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
            const auto fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
            if (fb_width == 0 || fb_height == 0)
                return;
            draw_data->ScaleClipRects(io.DisplayFramebufferScale);

            // Backup GL state
            GLenum last_active_texture;
            glGetIntegerv(GL_ACTIVE_TEXTURE, reinterpret_cast<GLint*>(&last_active_texture));
            glActiveTexture(GL_TEXTURE0);
            GLint last_program;
            glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            GLint last_texture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            GLint last_sampler;
            glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
            GLint last_array_buffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            GLint last_element_array_buffer;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
            GLint last_vertex_array;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
            GLint last_polygon_mode[2];
            glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
            GLint last_viewport[4];
            glGetIntegerv(GL_VIEWPORT, last_viewport);
            GLint last_scissor_box[4];
            glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
            GLenum last_blend_src_rgb;
            glGetIntegerv(GL_BLEND_SRC_RGB, reinterpret_cast<GLint*>(&last_blend_src_rgb));
            GLenum last_blend_dst_rgb;
            glGetIntegerv(GL_BLEND_DST_RGB, reinterpret_cast<GLint*>(&last_blend_dst_rgb));
            GLenum last_blend_src_alpha;
            glGetIntegerv(GL_BLEND_SRC_ALPHA, reinterpret_cast<GLint*>(&last_blend_src_alpha));
            GLenum last_blend_dst_alpha;
            glGetIntegerv(GL_BLEND_DST_ALPHA, reinterpret_cast<GLint*>(&last_blend_dst_alpha));
            GLenum last_blend_equation_rgb;
            glGetIntegerv(GL_BLEND_EQUATION_RGB, reinterpret_cast<GLint*>(&last_blend_equation_rgb));
            GLenum last_blend_equation_alpha;
            glGetIntegerv(GL_BLEND_EQUATION_ALPHA, reinterpret_cast<GLint*>(&last_blend_equation_alpha));
            const auto last_enable_blend = glIsEnabled(GL_BLEND);
            const auto last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
            const auto last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
            const auto last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

            // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Setup viewport, orthographic projection matrix
            glViewport(0, 0, static_cast<GLsizei>(fb_width), static_cast<GLsizei>(fb_height));
            const float ortho_projection[4][4] =
            {
                {2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f},
                {0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f},
                {0.0f, 0.0f, -1.0f, 0.0f},
                {-1.0f, 1.0f, 0.0f, 1.0f},
            };
            glUseProgram(g_shader_handle);
            glUniform1i(g_AttribLocationTex, 0);
            glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
            glBindSampler(0, 0); // Rely on combined texture/sampler state.

            // Recreate the VAO every time 
            // (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
            GLuint vao_handle = 0;
            glGenVertexArrays(1, &vao_handle);
            glBindVertexArray(vao_handle);
            glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
            glEnableVertexAttribArray(g_AttribLocationPosition);
            glEnableVertexAttribArray(g_AttribLocationUV);
            glEnableVertexAttribArray(g_AttribLocationColor);
            glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                                  reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, pos)));
            glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                                  reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, uv)));
            glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert),
                                  reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, col)));

            // Draw
            for (int n = 0; n < draw_data->CmdListsCount; n++) {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                const ImDrawIdx* idx_buffer_offset = nullptr;

                glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
                glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert),
                             static_cast<const GLvoid*>(cmd_list->VtxBuffer.Data), GL_STREAM_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx),
                             static_cast<const GLvoid*>(cmd_list->IdxBuffer.Data), GL_STREAM_DRAW);

                for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
                    auto pcmd = &cmd_list->CmdBuffer[cmd_i];
                    if (pcmd->UserCallback) {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                    else {
                        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(reinterpret_cast<intptr_t>(pcmd->TextureId)));
                        glScissor(static_cast<int>(pcmd->ClipRect.x), static_cast<int>(fb_height - pcmd->ClipRect.w),
                                  static_cast<int>(pcmd->ClipRect.z - pcmd->ClipRect.x),
                                  static_cast<int>(pcmd->ClipRect.w - pcmd->ClipRect.y));
                        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(pcmd->ElemCount),
                                       sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
                    }
                    idx_buffer_offset += pcmd->ElemCount;
                }
            }
            glDeleteVertexArrays(1, &vao_handle);

            // Restore modified GL state
            glUseProgram(last_program);
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glBindSampler(0, last_sampler);
            glActiveTexture(last_active_texture);
            glBindVertexArray(last_vertex_array);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
            glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
            glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
            if (last_enable_blend) glEnable(GL_BLEND);
            else glDisable(GL_BLEND);
            if (last_enable_cull_face) glEnable(GL_CULL_FACE);
            else glDisable(GL_CULL_FACE);
            if (last_enable_depth_test) glEnable(GL_DEPTH_TEST);
            else glDisable(GL_DEPTH_TEST);
            if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST);
            else glDisable(GL_SCISSOR_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(last_polygon_mode[0]));
            glViewport(last_viewport[0], last_viewport[1], static_cast<GLsizei>(last_viewport[2]), static_cast<GLsizei>(last_viewport[3]));
            glScissor(last_scissor_box[0], last_scissor_box[1], static_cast<GLsizei>(last_scissor_box[2]),
                      static_cast<GLsizei>(last_scissor_box[3]));
        }

        static const char* get_clipboard_text(void* user_data) {
            return glfwGetClipboardString(static_cast<GLFWwindow*>(user_data));
        }

        static void set_clipboard_text(void* user_data, const char* text) {
            glfwSetClipboardString(static_cast<GLFWwindow*>(user_data), text);
        }

        inline void mouse_button_callback(GLFWwindow*, const int button, const int action, int /*mods*/) {
            if (action == GLFW_PRESS && button >= 0 && button < 3)
                g_mouse_just_pressed[button] = true;
        }

        inline void scroll_callback(GLFWwindow*, const double xoffset, const double yoffset) {
            auto& io = ImGui::GetIO();
            io.MouseWheelH += static_cast<float>(xoffset);
            io.MouseWheel += static_cast<float>(yoffset);
        }

        inline void key_callback(GLFWwindow*, const int key, int, const int action, int mods) {
            auto& io = ImGui::GetIO();
            if (action == GLFW_PRESS) {
                io.KeysDown[key] = true;
            }
            if (action == GLFW_RELEASE) {
                io.KeysDown[key] = false;
            }

            (void) mods; // Modifiers are not reliable across systems
            io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
            io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
            io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
            io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        }

        inline void char_callback(GLFWwindow*, const unsigned int c) {
            auto& io = ImGui::GetIO();
            if (c > 0 && c < 0x10000)
                io.AddInputCharacter(static_cast<unsigned short>(c));
        }

        inline bool create_fonts_texture() {
            // Build texture atlas
            auto& io = ImGui::GetIO();
            unsigned char* pixels;
            int width, height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
            // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

            // Upload texture to graphics system
            GLint last_texture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGenTextures(1, &g_font_texture);
            glBindTexture(GL_TEXTURE_2D, g_font_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            // Store our identifier
            io.Fonts->TexID = reinterpret_cast<void *>(static_cast<intptr_t>(g_font_texture));

            // Restore state
            glBindTexture(GL_TEXTURE_2D, last_texture);

            return true;
        }

        inline bool create_device_objects() {
            // Backup GL state
            GLint last_texture, last_array_buffer, last_vertex_array;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

            const auto vertex_shader =
                "uniform mat4 ProjMtx;\n"
                "in vec2 Position;\n"
                "in vec2 UV;\n"
                "in vec4 Color;\n"
                "out vec2 Frag_UV;\n"
                "out vec4 Frag_Color;\n"
                "void main()\n"
                "{\n"
                "	Frag_UV = UV;\n"
                "	Frag_Color = Color;\n"
                "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                "}\n";

            const auto fragment_shader =
                "uniform sampler2D Texture;\n"
                "in vec2 Frag_UV;\n"
                "in vec4 Frag_Color;\n"
                "out vec4 Out_Color;\n"
                "void main()\n"
                "{\n"
                "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
                "}\n";

            const GLchar* vertex_shader_with_version[2] = {g_GlslVersion, vertex_shader};
            const GLchar* fragment_shader_with_version[2] = {g_GlslVersion, fragment_shader};

            g_shader_handle = glCreateProgram();
            g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
            g_frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(g_VertHandle, 2, vertex_shader_with_version, nullptr);
            glShaderSource(g_frag_handle, 2, fragment_shader_with_version, nullptr);
            glCompileShader(g_VertHandle);
            glCompileShader(g_frag_handle);
            glAttachShader(g_shader_handle, g_VertHandle);
            glAttachShader(g_shader_handle, g_frag_handle);
            glLinkProgram(g_shader_handle);

            g_AttribLocationTex = glGetUniformLocation(g_shader_handle, "Texture");
            g_AttribLocationProjMtx = glGetUniformLocation(g_shader_handle, "ProjMtx");
            g_AttribLocationPosition = glGetAttribLocation(g_shader_handle, "Position");
            g_AttribLocationUV = glGetAttribLocation(g_shader_handle, "UV");
            g_AttribLocationColor = glGetAttribLocation(g_shader_handle, "Color");

            glGenBuffers(1, &g_VboHandle);
            glGenBuffers(1, &g_ElementsHandle);

            create_fonts_texture();

            // Restore modified GL state
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindVertexArray(last_vertex_array);

            return true;
        }

        inline void invalidate_device_objects() {
            if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
            if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
            g_VboHandle = g_ElementsHandle = 0;

            if (g_shader_handle && g_VertHandle) glDetachShader(g_shader_handle, g_VertHandle);
            if (g_VertHandle) glDeleteShader(g_VertHandle);
            g_VertHandle = 0;

            if (g_shader_handle && g_frag_handle) glDetachShader(g_shader_handle, g_frag_handle);
            if (g_frag_handle) glDeleteShader(g_frag_handle);
            g_frag_handle = 0;

            if (g_shader_handle) glDeleteProgram(g_shader_handle);
            g_shader_handle = 0;

            if (g_font_texture) {
                glDeleteTextures(1, &g_font_texture);
                ImGui::GetIO().Fonts->TexID = 0;
                g_font_texture = 0;
            }
        }

        static void install_callbacks(GLFWwindow* window) {
            glfwSetMouseButtonCallback(window, mouse_button_callback);
            glfwSetScrollCallback(window, scroll_callback);
            glfwSetKeyCallback(window, key_callback);
            glfwSetCharCallback(window, char_callback);
        }

        inline ImGuiContext* init_imgui(GLFWwindow* window, const bool install) {
            auto context = ImGui::CreateContext();
            auto& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
            g_window = window;
            
            io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
            io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
            io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
            io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
            io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
            io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
            io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
            io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
            io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
            io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
            io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
            io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
            io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
            io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
            io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
            io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
            io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

            io.SetClipboardTextFn = set_clipboard_text;
            io.GetClipboardTextFn = get_clipboard_text;
            io.ClipboardUserData = g_window;
#ifdef _WIN32
            io.ImeWindowHandle = g_window;
#endif

            // Load cursors
            // FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW, ResizeNWSE. We revert to arrow cursor for those.
            g_mouse_cursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            g_mouse_cursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
            g_mouse_cursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            g_mouse_cursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
            g_mouse_cursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
            g_mouse_cursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            g_mouse_cursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

            if (install) {
                install_callbacks(window);
            }
            return context;
        }

        inline void shutdown_imgui(ImGuiContext* context) {
            // Destroy GLFW mouse cursors
            for (auto& g_mouse_cursor : g_mouse_cursors) {
                glfwDestroyCursor(g_mouse_cursor);
            }
            memset(g_mouse_cursors, 0, sizeof(g_mouse_cursors));

            // Destroy OpenGL objects
            invalidate_device_objects();

            ImGui::DestroyContext(context);
        }

        inline void start_new_frame() {
            if (!g_font_texture)
                create_device_objects();

            auto& io = ImGui::GetIO();

            // Setup display size (every frame to accommodate for window resizing)
            int w, h;
            int display_w, display_h;
            glfwGetWindowSize(g_window, &w, &h);
            glfwGetFramebufferSize(g_window, &display_w, &display_h);
            io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));
            io.DisplayFramebufferScale = ImVec2(w > 0 ? (static_cast<float>(display_w) / w) : 0,
                                                h > 0 ? (static_cast<float>(display_h) / h) : 0);

            // Setup time step
            const auto current_time = glfwGetTime();
            io.DeltaTime = g_time > 0.0 ? static_cast<float>(current_time - g_time) : static_cast<float>(1.0f / 60.0f);
            g_time = current_time;

            // Setup inputs
            // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
            if (glfwGetWindowAttrib(g_window, GLFW_FOCUSED)) {
                if (io.WantMoveMouse) {
                    glfwSetCursorPos(g_window, static_cast<double>(io.MousePos.x), static_cast<double>(io.MousePos.y));
                    // Set mouse position if requested by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
                }
                else {
                    double mouse_x, mouse_y;
                    glfwGetCursorPos(g_window, &mouse_x, &mouse_y);
                    io.MousePos = ImVec2(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
                }
            }
            else {
                io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
            }

            for (int i = 0; i < 3; i++) {
                // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
                io.MouseDown[i] = g_mouse_just_pressed[i] || glfwGetMouseButton(g_window, i) != 0;
                g_mouse_just_pressed[i] = false;
            }

            // Update OS/hardware mouse cursor if imgui isn't drawing a software cursor
            ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
            if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
                glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else {
                glfwSetCursor(
                    g_window, g_mouse_cursors[cursor] ? g_mouse_cursors[cursor] : g_mouse_cursors[ImGuiMouseCursor_Arrow]);
                glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }

            // Gamepad navigation mapping [BETA]
            memset(io.NavInputs, 0, sizeof(io.NavInputs));
            if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) {
                // Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO) { if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS) io.NavInputs[NAV_NO] = 1.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); if (v > 1.0f) v = 1.0f; if (io.NavInputs[NAV_NO] < v) io.NavInputs[NAV_NO] = v; }
                int axes_count = 0, buttons_count = 0;
                const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
                const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
                MAP_BUTTON(ImGuiNavInput_Activate, 0); // Cross / A
                MAP_BUTTON(ImGuiNavInput_Cancel, 1); // Circle / B
                MAP_BUTTON(ImGuiNavInput_Menu, 2); // Square / X
                MAP_BUTTON(ImGuiNavInput_Input, 3); // Triangle / Y
                MAP_BUTTON(ImGuiNavInput_DpadLeft, 13); // D-Pad Left
                MAP_BUTTON(ImGuiNavInput_DpadRight, 11); // D-Pad Right
                MAP_BUTTON(ImGuiNavInput_DpadUp, 10); // D-Pad Up
                MAP_BUTTON(ImGuiNavInput_DpadDown, 12); // D-Pad Down
                MAP_BUTTON(ImGuiNavInput_FocusPrev, 4); // L1 / LB
                MAP_BUTTON(ImGuiNavInput_FocusNext, 5); // R1 / RB
                MAP_BUTTON(ImGuiNavInput_TweakSlow, 4); // L1 / LB
                MAP_BUTTON(ImGuiNavInput_TweakFast, 5); // R1 / RB
                MAP_ANALOG(ImGuiNavInput_LStickLeft, 0, -0.3f, -0.9f);
                MAP_ANALOG(ImGuiNavInput_LStickRight, 0, +0.3f, +0.9f);
                MAP_ANALOG(ImGuiNavInput_LStickUp, 1, +0.3f, +0.9f);
                MAP_ANALOG(ImGuiNavInput_LStickDown, 1, -0.3f, -0.9f);
#undef MAP_BUTTON
#undef MAP_ANALOG
            }

            // Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
            ImGui::NewFrame();
        }
    }
}

#endif //VUP_IMGUI_UTILS_H
