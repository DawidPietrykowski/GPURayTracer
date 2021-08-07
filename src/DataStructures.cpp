#define _USE_MATH_DEFINES
#include <DataStructures.h>
#include <vector>
#include <array>
#include <math.h>
#include <cmath>
#include <string>
#include <iostream>

void CameraData::AccelerateCamera(float acc[3]) {
    for (int i = 0; i < 3; i++)
        acceleration[i] += acc[i] * acc_mul;
}

void CameraData::RotateCamera(float dx, float dy) {
    rotation[1] += dx * sensitivity * (500.0f/m_width);
    if (rotation[0] + dy * sensitivity * (500.0f / m_width) < -90)
        rotation[0] = -90;
    else if (rotation[0] + dy * sensitivity * (500.0f / m_width)  > 90)
        rotation[0] = 90;
    else
        rotation[0] += dy * sensitivity * (500.0f / m_width);


    rotation[1] = (float)fmod(rotation[1], 360);

    if((abs(dx) > 0.0001f) || (abs(dy) > 0.0001f))
        m_changed = true;
}

void CameraData::CopyData(ShaderData& shader_data) {
    shader_data._camera_position = { position[0], position[1], position[2], 0 };
    shader_data._camera_rotation = { rotation[0], rotation[1], rotation[2], 0 };
}

bool CameraData::HasChanged() {
    return m_changed;
}

void CameraData::SetChanged(bool ch) {
    m_changed = ch;
}

float DegToRad(float deg) {
    return (float)(deg * (M_PI / 180.0f));
}

void CameraData::UpdateCameraData(float time) {
    std::vector<std::vector<float>> move_vectors;

    std::vector<float> forward = { -sin(DegToRad(rotation[1])), sin(DegToRad(rotation[0])), cos(DegToRad(rotation[1])) };
    std::vector<float> up = { 0, 1.0f, 0 };
    std::vector<float> right = { cos(DegToRad(rotation[1])), 0, sin(DegToRad(rotation[1])) };

    move_vectors.push_back(right);
    move_vectors.push_back(up);
    move_vectors.push_back(forward);

    //update position
    for (int i = 0; i < 3; i++) {
        temp[0] = position[i];
        temp[1] = rotation[i];

        // calculate accelleration
        float accel = 0;
        for (int x = 0; x < 3; x++)
            accel += acceleration[x] * move_vectors[x][i];
        velocity[i] += accel * time;

        position[i] += velocity[i] * time;

        // apply friction
        float sum = abs(velocity[0]) + abs(velocity[1]) + abs(velocity[2]);
        float part_friction = abs((velocity[i] / sum)) * friction;
        if (sum != 0) {
            if (velocity[i] > 0) {
                if (part_friction * time > velocity[i])
                    velocity[i] = 0;
                else
                    velocity[i] -= part_friction * time;
            }
            else if (velocity[i] < 0) {
                if (part_friction * time < velocity[i])
                    velocity[i] = 0;
                else
                    velocity[i] += part_friction * time;
            }
            position[i] += velocity[i] * time;
        }

        if ((temp[0] != position[i]) || (temp[1] != rotation[i]))
            m_changed = true;
    }
    // reset acceleration
    for (int i = 0; i < 3; i++)
        acceleration[i] = 0;

    // scale velocity to max velocity
    float length = sqrt(pow(velocity[0], 2) + pow(velocity[1], 2) + pow(velocity[2], 2));
    float scaling = 1.0f;
    if (length > max_v) {
        scaling = ((length - max_v) / max_v) + 1.0f;
        for(int i = 0; i < 3; i++)
            velocity[i] /= scaling;
    }
}

void ShaderData::UpdateSeed() {
    _seed = GetRand();
}
void ShaderData::UpdateWindowSize(int width, int height) {
    _screen_size[0] = width;
    _screen_size[1] = height;
}
void ShaderData::GetWindowSize(int& width, int& height) {
    width = _screen_size[0];
    height = _screen_size[1];
}
int ShaderData::GetWidth() {
    return _screen_size[0];
}
int ShaderData::GetHeight() {
    return _screen_size[1];
}
void CameraData::UpdateWindowSize(int width, int height) {
    m_width = width;
    m_height = height;
}