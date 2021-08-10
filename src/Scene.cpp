#include "Scene.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void IndexArray::AddVertex(int x, int y, int z) {
    m_array.push_back(x - 1);
    m_array.push_back(y - 1);
    m_array.push_back(z - 1);
    m_array.push_back(0);
    m_array.push_back(0);
    m_array.push_back(0);
}

void IndexArray::AddVertex(int x, int y, int z, int tx, int ty, int tz) {
    m_array.push_back(x - 1);
    m_array.push_back(y - 1);
    m_array.push_back(z - 1);
    m_array.push_back(tx - 1);
    m_array.push_back(ty - 1);
    m_array.push_back(tz - 1);
}

int IndexArray::GetSize() {
    return (int)m_array.size();
}

int IndexArray::GetByteSize() {
    return (int)m_array.size() * (int)sizeof(int);
}

std::vector<int>* IndexArray::GetPointer() {
    return &m_array;
}

std::vector<int> IndexArray::GetVector() {
    return m_array;
}

void IndexArray::MoveIndices(int offset) {
    int size = (int)m_array.size();
    const int block_size = 6;
    for (int i = 0; i < size / block_size; i++) {
        m_array[block_size *i] += offset;
        m_array[block_size *i + 1] += offset;
        m_array[block_size *i + 2] += offset;
    }
}

void IndexArray::MoveTextureIndices(int offset){
    int size = (int)m_array.size();
    const int block_size = 6;
    for (int i = 0; i < size / block_size; i++) {
        m_array[block_size * i + 3] += offset;
        m_array[block_size * i + 4] += offset;
        m_array[block_size * i + 5] += offset;
    }
}

void VertexArray::AddVertex(float x, float y, float z) {
    m_array.push_back(x);
    m_array.push_back(y);
    m_array.push_back(z);
}

int VertexArray::GetSize() {
    return (int)m_array.size();
}

int VertexArray::GetByteSize() {
    return (int)m_array.size() * (int)sizeof(float);
}

std::vector<float>* VertexArray::GetPointer() {
    return &m_array;
}

std::vector<float> VertexArray::GetVector() {
    return m_array;
}

void TextureVertexArray::AddVertex(float u, float v) {
    m_array.push_back(u);
    m_array.push_back(v);
}

int TextureVertexArray::GetSize() {
    return (int)m_array.size();
}

int TextureVertexArray::GetByteSize() {
    return (int)m_array.size() * (int)sizeof(float);
}

std::vector<float>* TextureVertexArray::GetPointer() {
    return &m_array;
}

std::vector<float> TextureVertexArray::GetVector() {
    return m_array;
}

void SceneVertices::AddModel(Model3D* model) {
    int size = model->GetIndexArrayPtr()->GetSize() / 6;
    m_offsets.push_back(size + m_index_count);

    m_models.push_back(model);
}

void SceneVertices::FillBuffers() {
    int size = (int)m_models.size();
    for (int i = 0; i < size; i++) {
        m_models[i]->GetIndexArrayPtr()->MoveIndices(m_vertex_count);
        m_models[i]->GetIndexArrayPtr()->MoveTextureIndices(m_texture_vertex_count);
        m_models[i]->SetIndexOffset(m_index_count);

        m_vertex_buffer.insert(m_vertex_buffer.end(),
            m_models[i]->GetVertexArrayPtr()->GetPointer()->begin(),
            m_models[i]->GetVertexArrayPtr()->GetPointer()->end());
        
        m_texture_buffer.insert(m_texture_buffer.end(),
            m_models[i]->GetTextureArrayPtr()->GetPointer()->begin(),
            m_models[i]->GetTextureArrayPtr()->GetPointer()->end());

        m_index_buffer.insert(m_index_buffer.end(),
            m_models[i]->GetIndexArrayPtr()->GetPointer()->begin(),
            m_models[i]->GetIndexArrayPtr()->GetPointer()->end());

        m_vertex_count += m_models[i]->GetVertexArrayPtr()->GetSize() / 3;
        m_texture_vertex_count += m_models[i]->GetTextureArrayPtr()->GetSize() / 2;
        m_index_count += m_models[i]->GetIndexArrayPtr()->GetSize() / 6;
    }
}

std::vector<float>* SceneVertices::GetVertexBuffer() {
    return &m_vertex_buffer;
}

std::vector<float>* SceneVertices::GetTextureBuffer() {
    return &m_texture_buffer;
}

std::vector<int>* SceneVertices::GetIndexBuffer() {
    return &m_index_buffer;
}

int SceneVertices::GetFaceCount() {
    return m_index_count;
}

int SceneVertices::GetTextureVertsCount() {
    return m_texture_vertex_count;
}

int SceneVertices::GetVertexCount() {
    return m_vertex_count;
}

void Model3D::Load(std::string path) {
    std::fstream file(path);

    std::string line;
    std::stringstream str;

    std::string prefix;

    int vertex_id = 0;

    float x, y, z;
    while (std::getline(file, line)) {
        str.clear();
        str.str(line);
        str >> prefix;
        if (prefix == "v") {
            str >> x >> y >> z;

            if (m_vertex_arr.GetSize() == 0)
                m_lowest_point = y;
            else
                m_lowest_point = std::min(m_lowest_point, y);

            m_vertex_arr.AddVertex(x, y, z);
        }
        else if (prefix == "vt") {
            str >> x >> y;

            m_texture_arr.AddVertex(x, y);
        }
        else if (prefix == "f") {
            int f;
            int v1 = 0;
            int v2 = 0;
            int v3 = 0;
            int v4 = 0;

            int vt1 = 0;
            int vt2 = 0;
            int vt3 = 0;
            int vt4 = 0;

            int type_counter = 0;
            int id_counter = 0;

            while (str >> f) {


                if (type_counter == 0) {
                    if (id_counter == 0) {
                        v1 = f;
                    }
                    else if (id_counter == 1) {
                        v2 = f;
                    }
                    else if (id_counter == 2) {
                        v3 = f;
                    }
                    else if (id_counter == 3) {
                        v4 = f;
                    }
                }
                else if (type_counter == 1) {
                    if (id_counter == 0) {
                        vt1 = f;
                    }
                    else if (id_counter == 1) {
                        vt2 = f;
                    }
                    else if (id_counter == 2) {
                        vt3 = f;
                    }
                    else if (id_counter == 3) {
                        vt4 = f;
                    }
                }

                while (str.peek() == '/') {
                    type_counter++;
                    type_counter %= 3;
                    str.ignore(1, '/');
                }
                while (str.peek() == ' ') {
                    id_counter++;
                    type_counter = 0;
                    str.ignore(1, ' ');
                }
            }

            if (id_counter == 2)
                m_index_arr.AddVertex(v1, v2, v3, vt1, vt2, vt3);
            else {
                m_index_arr.AddVertex(v1, v2, v3, vt1, vt2, vt3);
                m_index_arr.AddVertex(v1, v3, v4, vt1, vt3, vt4);
            }
        }
    }
    // shift vertices to level the vertices to ground
    const int block_size = 3;
    for (int i = 0; i < m_vertex_arr.GetSize()/ block_size; i++)
        (*m_vertex_arr.GetPointer())[block_size * i + 1] -= (m_lowest_point - 0.001f);
}

void Model3D::SetId(int id) {
    m_id = id;
}

void Model3D::SetIndexOffset(int offset) {
    m_index_offset = offset;
}

int Model3D::GetIndexOffset() {
    return m_index_offset;
}

int Model3D::GetFaceCount() {
    return m_index_arr.GetSize() / 6;
}

int Model3D::GetId() {
    return m_id;
}

float Model3D::GetLowestPoint() {
    return m_lowest_point;
}

VertexArray Model3D::GetVertexArray() {
    return m_vertex_arr;
}

IndexArray Model3D::GetIndexArray() {
    return m_index_arr;
}

TextureVertexArray Model3D::GetTextureArray() {
    return m_texture_arr;
}

VertexArray* Model3D::GetVertexArrayPtr() {
    return &m_vertex_arr;
}

IndexArray* Model3D::GetIndexArrayPtr() {
    return &m_index_arr;
}

TextureVertexArray* Model3D::GetTextureArrayPtr() {
    return &m_texture_arr;
}
