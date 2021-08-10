#pragma once
#include "DataStructures.h"
#include "ComputeEngine.h"
#include "Scene.h"

class Config {
public:
	SceneVertices m_buffer;

	std::vector<Model3D> m_models;
	std::vector<SceneObject> m_objects;
	std::vector<Texture> m_textures;

	std::string m_window_name = "Window";
	int m_window_width = 512;
	int m_window_height = 512;
	bool m_start_focused = true;

	std::string m_pixel_shader_path = "";
	std::string m_object_shader_path = "";

	int m_iterations = 5;
	int m_samples_per_frame = 1;
	int m_fov = 30;
	float m_camera_sensitivity = 10.0f;
	std::array<float, 3> m_camera_position = { 0, 0, 0 };
	std::array<float, 3> m_camera_rotation = { 0, 0, 0 };

	std::string m_texture_sky_path = "";
	std::string	m_texture_1_path = "";
	std::string	m_texture_2_path = "";
	std::string	m_texture_3_path = "";
	std::string	m_texture_4_path = "";
	std::string	m_texture_5_path = "";

	Texture* m_tex_sky = (Texture*)(nullptr);
	Texture* m_tex_1 = (Texture*)(nullptr);
	Texture* m_tex_2 = (Texture*)(nullptr);
	Texture* m_tex_3 = (Texture*)(nullptr);
	Texture* m_tex_4 = (Texture*)(nullptr);
	Texture* m_tex_5 = (Texture*)(nullptr);


	bool LoadConfigFile(std::string path);
	bool BindTextures(Program& program);
	std::vector<int>* GetIndexBuffer();
	std::vector<float>* GetTextureBuffer();
	std::vector<float>* GetVertexBuffer();
	std::vector<SceneObject>* GetObjects();
	int GetVertexCount();
	int GetFaceCount();
	int GetObjectCount();
	Config(std::string path);
	Config();
};