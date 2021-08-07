#include <Config.h>

Config::Config(std::string path) {
    LoadConfigFile(path);
}

std::vector<SceneObject>* Config::GetObjects()
{
    return &m_objects;
}

int Config::GetVertexCount()
{
    return m_buffer.GetVertexCount();
}

int Config::GetFaceCount()
{
    return m_buffer.GetFaceCount();
}
int Config::GetObjectCount()
{
    return (int)m_objects.size();
}

Config::Config() {
    
}

bool Config::LoadConfigFile(std::string path)
{

    std::fstream file(path);

    std::string line;
    std::stringstream str;

    std::string prefix;

    int vertex_id = 0;

    float x, y, z;
    int ix, iy, iz;
    std::string data;

    bool reading_object = false;
    while (std::getline(file, line)) {
        str.clear();
        str.str(line);
        str >> prefix;

        if (line.length() == 0)
            continue;
        if (line[0] == '#')
            continue;

        if (prefix == "window_name") {
            m_window_name = line.substr(prefix.length() + 1, line.length() - prefix.length());
        }
        else if (prefix == "window_size") {
            str >> ix >> iy;
            m_window_width = ix;
            m_window_height = iy;
        }
        else if (prefix == "start_focused") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            if (data == "true" || data == "1")
                m_start_focused = true;
            else
                m_start_focused = false;
        }
        else if (prefix == "pixel_shader_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_pixel_shader_path = data;
        }
        else if (prefix == "object_shader_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_object_shader_path = data;
        }
        else if (prefix == "texture_sky_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_texture_sky_path = data;
        }
        else if (prefix == "texture_1_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_texture_1_path = data;
        }
        else if (prefix == "texture_2_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_texture_2_path = data;
        }
        else if (prefix == "texture_3_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_texture_3_path = data;
        }
        else if (prefix == "texture_4_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_texture_4_path = data;
        }
        else if (prefix == "texture_5_path") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_texture_5_path = data;
        }
        else if (prefix == "reflections") {
            str >> m_iterations;
        }
        else if (prefix == "samples") {
            str >> m_samples_per_frame;
        }
        else if (prefix == "fov") {
            str >> m_fov;
        }
        else if (prefix == "sensitivity") {
            str >> m_camera_sensitivity;
        }
        else if (prefix == "camera_position") {
            str >> x >> y >> z;
            m_camera_position = { x, y, z };
        }
        else if (prefix == "camera_rotation") {
            str >> x >> y >> z;
            m_camera_rotation = { x, y, z };
        }
        else if (prefix == "model") {
            reading_object = true;
            m_objects.push_back(SceneObject());
        }
        else if (prefix.find("object_") != -1) {
            data = prefix.substr(7, prefix.length() - 6 - 1);
            int id = stoi(data);
            m_models.push_back(Model3D());
            m_models.back().SetId(id);
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            m_models.back().Load(data);
        }
        else if (prefix == "model") {
            reading_object = true;
            m_objects.push_back(SceneObject());
        }
        else if (prefix == "obj_id") {
            str >> ix;
            m_objects.back().type = 2;
            m_objects.back().obj_id = ix;
        }
        else if (prefix == "texture_albedo") {
            str >> ix;
            m_objects.back().texture_id[0] = ix;
        }
        else if (prefix == "texture_specular") {
            str >> ix;
            m_objects.back().texture_id[1] = ix;
        }
        else if (prefix == "texture_emission") {
            str >> ix;
            m_objects.back().texture_id[3] = ix;
        }
        else if (prefix == "position") {
            str >> x >> y >> z;
            m_objects.back().position = { x, y, z };
        }
        else if (prefix == "rotation") {
            str >> x >> y >> z;
            m_objects.back().orientation = { x, y, z };
        }
        else if (prefix == "size") {
            str >> x >> y >> z;
            m_objects.back().size = { x, y, z };
        }
        else if (prefix == "albedo") {
            str >> x >> y >> z;
            m_objects.back().albedo = { x, y, z };
        }
        else if (prefix == "specular") {
            str >> x >> y >> z;
            m_objects.back().specular = { x, y, z };
        }
        else if (prefix == "emission") {
            str >> x >> y >> z;
            m_objects.back().emission = { x, y, z };
        }
        else if (prefix == "smoothness") {
            str >> x;
            m_objects.back().smoothness = x;
        }
        else if (prefix == "refractive_index") {
            str >> x;
            m_objects.back().refractive_index = x;
        }
        else if (prefix == "transparency") {
            str >> x;
            m_objects.back().transparency = x;
        }
        else if (prefix == "type") {
            data = line.substr(prefix.length() + 1, line.length() - prefix.length() - 1);
            if (data == "sphere")
                m_objects.back().type = 0;
            else if (data == "plane")
                m_objects.back().type = 1;
        }
    }

    for (int i = 0; i < m_models.size(); i++)
        m_buffer.AddModel(&m_models[i]);
    m_buffer.FillBuffers();

    for (int i = 0; i < m_objects.size(); i++) {
        if (m_objects[i].type == 2) {
            // find object
            for (int j = 0; j < m_models.size(); j++)
                if (m_models[j].GetId() == m_objects[i].obj_id) {
                    // set offsets
                    m_objects[i].offset = m_models[j].GetIndexOffset();
                    m_objects[i].vert_num = m_models[j].GetFaceCount();
                }
        }
    }

	return true;
}

bool Config::BindTextures(Program& program)
{
    if (m_texture_sky_path != "") {
        m_tex_sky = new Texture("u_skybox", m_texture_sky_path);
    }
    if (m_texture_1_path != "") {
        m_tex_1 = new Texture("u_tex1", m_texture_1_path);
    }
    if (m_texture_2_path != "") {
        m_tex_2 = new Texture("u_tex2", m_texture_2_path);
    }
    if (m_texture_3_path != "") {
        m_tex_3 = new Texture("u_tex3", m_texture_3_path);
    }
    if (m_texture_4_path != "") {
        m_tex_4 = new Texture("u_tex4", m_texture_4_path);
    }
    if (m_texture_5_path != "") {
        m_tex_5 = new Texture("u_tex5", m_texture_5_path);
    }
    if (m_texture_sky_path != "") {
        program.BindTexture(m_tex_sky, 0);
    }
    if (m_texture_1_path != "") {
        program.BindTexture(m_tex_1, 1);
    }
    if (m_texture_2_path != "") {
        program.BindTexture(m_tex_2, 2);
    }
    if (m_texture_3_path != "") {
        program.BindTexture(m_tex_3, 3);
    }
    if (m_texture_4_path != "") {
        program.BindTexture(m_tex_4, 4);
    }
    if (m_texture_5_path != "") {
        program.BindTexture(m_tex_5, 5);
    }
    return true;
}

std::vector<int>* Config::GetIndexBuffer() {
    return m_buffer.GetIndexBuffer();
}
std::vector<float>* Config::GetTextureBuffer() {
    return m_buffer.GetTextureBuffer();
}
std::vector<float>* Config::GetVertexBuffer() {
    return m_buffer.GetVertexBuffer();
}
