#pragma once
#include <array>
#include <string>
#include <Utils.h>

struct ShaderData
{
public:
    std::array<float, 4> _camera_position = { 0, 0, 0, 0 };
    std::array<float, 4> _camera_rotation = { 0, 0, 0, 0 };
    std::array<float, 2> _pixel_offset = { 0.5f, 0.5f };
    std::array<int, 2> _screen_size = { 0, 0 };
    int _iterations = 5;
    float _seed = GetRand();
    int _objectcount = 0;
    int _sample = 1;
    int _samples = 1;
    int _fov = 30;
    std::array<float, 2> _padding = { 0, 0};
    void UpdateSeed();
    void UpdateWindowSize(int width, int height);
    void GetWindowSize(int& width, int& height);
    int GetWidth();
    int GetHeight();
};
struct CameraData {
    std::array<float, 3> velocity = { 0, 0, 0 };
    std::array<float, 3> position = { 0, 0, 0 };
    std::array<float, 3> rotation = { 0, 0, 0 };
    std::array<float, 3> acceleration = { 0, 0, 0 };
    float max_v = 30.0f;
    float friction = 30.0f;
    float acc_mul = 50.0f;
    bool m_changed = false;
    bool breakk = false;
    float sensitivity = 3.0f;
    int m_width = 0, m_height = 0;
    void AccelerateCamera(float acc[3]);
    void RotateCamera(float dx, float dy);
    void CopyData(ShaderData& shader_data);

    bool HasChanged();

    void SetChanged(bool ch);

    float temp[2] = {0, 0};

    void UpdateCameraData(float time);
    void UpdateWindowSize(int width, int height);
};

struct SceneObject {
    std::array<float, 4> position = { 0, 0, 0, 0 };
    std::array<float, 4> size = { 1, 1, 1, 1 };
    std::array<float, 4> orientation = { 0, 0, 0, 0 };
    std::array<float, 4> albedo = { 1, 1, 1, 1 };
    std::array<float, 4> specular = { 0, 0, 0, 0 };
    std::array<float, 4> emission = { 0, 0, 0, 0 };
    //0 - diffuse
    //1 - specular
    //2 - normal
    //3 - emission
    std::array<int, 4> texture_id = { 0, 0, 0, 0 };
    float refractive_index = 1;
    float transparency = 0;
    float smoothness = 0;
    int type = 0;
    int offset = 0;
    int vert_num = 0;
    int obj_id = 0;
    int padding = 0;
};