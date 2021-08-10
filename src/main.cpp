#include <array>
#include <string>
#include <vector>
#include <iostream>

#include <ComputeEngine.h>

#include "Utils.h"
#include "DataStructures.h"
#include "Config.h"
#include "Scene.h"

#define UPDATE_OBJECT_BUFFERS 0

void UpdateKeys(ComputeEngine& renderer, CameraData& camera, std::vector<SceneObject>& objects, ShaderData& shader_data, Texture& tex_output) {
    if (renderer.IsKeyClicked(GLFW_KEY_P)) {
        renderer.SwitchInput();
    }
    if (renderer.GetInput()) {
        if (renderer.IsKeyPressed(GLFW_KEY_W))
            camera.AccelerateCamera(new float[3]{ 0, 0, 1.0f });
        if (renderer.IsKeyPressed(GLFW_KEY_A))
            camera.AccelerateCamera(new float[3]{ -1.0f, 0, 0 });
        if (renderer.IsKeyPressed(GLFW_KEY_S))
            camera.AccelerateCamera(new float[3]{ 0, 0, -1.0f });
        if (renderer.IsKeyPressed(GLFW_KEY_D))
            camera.AccelerateCamera(new float[3]{ 1.0f, 0, 0 });
        if (renderer.IsKeyPressed(GLFW_KEY_SPACE))
            camera.AccelerateCamera(new float[3]{ 0, 1.0f, 0 });
        if (renderer.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
            camera.AccelerateCamera(new float[3]{ 0, -1.0f, 0 });
        if (renderer.IsKeyPressed(GLFW_KEY_E)) {
        }
        if (renderer.IsKeyPressed(GLFW_KEY_Q)) {
        }
        if (renderer.IsKeyClicked(GLFW_KEY_F11)) {
            renderer.SwitchFullScreen();
        }
        if (renderer.IsKeyClicked(GLFW_KEY_ESCAPE))
            renderer.CloseWindow();
        if (renderer.IsKeyClicked(GLFW_KEY_L)) {
            renderer.SaveScreen("screenshot");
        }
    }
    int width, height;
    shader_data.GetWindowSize(width, height);
    renderer.GetWindowSize(shader_data._screen_size[0], shader_data._screen_size[1]);
    if (width != shader_data.GetWidth() || height != shader_data.GetHeight())
        tex_output.Resize(shader_data.GetWidth(), shader_data.GetHeight());
}

void UpdateInput(ComputeEngine& renderer, CameraData& camera, std::vector<SceneObject>& objects, ShaderData& shader_data, Texture& tex_output) {
    renderer.PollEvents();
    UpdateKeys(renderer, camera, objects, shader_data, tex_output);

    double dx, dy;
    renderer.GetMouseDelta(dx, dy);
    dx *= (renderer.GetFrametime() / 1000.0);
    dy *= (renderer.GetFrametime() / 1000.0);

    double xp, yp;
    int w, h;
    renderer.GetWindowSize(w, h);
    renderer.GetMousePos(xp, yp);

    bool in_window = xp > 0 && yp > 0 && xp < w && yp < h;

    if (renderer.IsMouseButtonClicked(GLFW_MOUSE_BUTTON_1) && !renderer.GetInput())
    {
        renderer.SetInput(true);
    }

    camera.RotateCamera((float)dx, (float)dy);
    camera.UpdateCameraData((float)(renderer.GetFrametime() / 1000.0));
}

int main(int argc, char** argv)
{
    srand((unsigned)time(0));

    std::string config_filename = "configs/config.txt";

    if (argc > 1)
        config_filename = argv[1];

    if (!FileExists(config_filename)) {
        LOG_ERROR(config_filename + " does not exist");
        return -1;
    }

    std::cout << "config file name: " << config_filename << std::endl;

    Config config(config_filename);

    int width = config.m_window_width;
    int height = config.m_window_height;

    // init renderer
    ComputeEngine renderer(width, height, config.m_window_name, config.m_start_focused, true);

    Shader pixel_compute_shader(config.m_pixel_shader_path);
    if (!pixel_compute_shader.Compiled()) {
        LOG_ERROR("shader compilation fail");
        return -1;
    }

    Program pixel_compute_program(pixel_compute_shader);

    // init data for shader
    ShaderData shader_data = {};
    shader_data._screen_size = { width, height };
    shader_data._iterations = config.m_iterations;
    shader_data._samples = config.m_samples_per_frame;
    shader_data._fov = config.m_fov;

    // init textures
    Texture tex_output("img_output", width, height);

    // bind textures
    pixel_compute_program.BindTextureImage(tex_output, 0);
    config.BindTextures(pixel_compute_program);

    // init buffers
    UBO data_buffer(2);
    UBO objects_buffer(3);
    SSBO index_buffer(4);
    SSBO vert_buffer(5);
    SSBO texture_vert_buffer(6);

    // fill buffers with data
    data_buffer.Set(&shader_data);
    objects_buffer.Set(config.GetObjects());
    index_buffer.Set(config.GetIndexBuffer());
    vert_buffer.Set(config.GetVertexBuffer());
    texture_vert_buffer.Set(config.GetTextureBuffer());

    // init camera
    CameraData camera;
    camera.position = config.m_camera_position;
    camera.rotation = config.m_camera_rotation;
    camera.sensitivity = config.m_camera_sensitivity;
    camera.m_width = width;
    camera.m_height = height;
    camera.CopyData(shader_data);

    printf("faces: %d\nvertices: %d\nobjects: %d\n", config.GetFaceCount(), config.GetVertexCount(), config.GetObjectCount());

    while (!renderer.ShouldClose()) {
        // update input
        camera.SetChanged(false);
        UpdateInput(renderer, camera, *config.GetObjects(), shader_data, tex_output);

        // update data for shader
        if (camera.HasChanged()) {
            shader_data._sample = 1;
            shader_data._pixel_offset = { 0.5f, 0.5f };
        }
        else {
            shader_data._sample++;
            shader_data._pixel_offset = { (GetRand() - 0.5f), (GetRand() - 0.5f) };
        }
        shader_data.UpdateSeed();
        shader_data._objectcount = config.GetObjectCount();
        camera.UpdateWindowSize(shader_data.GetWidth(), shader_data.GetHeight());
        camera.CopyData(shader_data);

        // update buffers
        data_buffer.Update(&shader_data);

#if UPDATE_OBJECT_BUFFERS
        objects_buffer.Update(config.GetObjects());
        index_buffer.Update(config.GetIndexBuffer());
        vert_buffer.Update(config.GetVertexBuffer());
        texture_vert_buffer.Update(config.GetTextureBuffer());
#endif

        // dispatch compute shader
        pixel_compute_program.DispatchCompute(shader_data.GetWidth(), shader_data.GetHeight());
        
        renderer.UpdateFrametime();

        // draw rendered image
        renderer.DrawTexture(tex_output);

        // glFinish call
        renderer.Finish();

        //print frametime every 50 frames
        if(renderer.GetFramecount()%50 == 0)
            printf("f:%.2fms w:%d h:%d samples:%d\n", renderer.GetFrametime(), renderer.GetWidth(), renderer.GetHeight(), shader_data._sample * shader_data._samples);
    }

    //print average frametime
    printf("average frametime: %.3fms", renderer.GetAverageFrametime());

    return 0;
}