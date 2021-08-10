#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ComputeEngine.h"

class IndexArray {
public:
    void AddVertex(int x, int y, int z, int tx, int ty, int tz);
    void AddVertex(int x, int y, int z);
    int GetSize();
    int GetByteSize();
    std::vector<int>* GetPointer();
    std::vector<int> GetVector();
    void MoveIndices(int offset);
    void MoveTextureIndices(int offset);
private:
    std::vector<int> m_array;
};

class VertexArray {
public:
    void AddVertex(float x, float y, float z);
    int GetSize();
    int GetByteSize();
    std::vector<float>* GetPointer();
    std::vector<float> GetVector();
private:
    std::vector<float> m_array;
};

class TextureVertexArray {
public:
    void AddVertex(float u, float v);
    int GetSize();
    int GetByteSize();
    std::vector<float>* GetPointer();
    std::vector<float> GetVector();
private:
    std::vector<float> m_array;
};

class Model3D {
public:
    void Load(std::string path);
    void SetId(int id);
    void SetIndexOffset(int offset);
    int GetIndexOffset();
    int GetFaceCount();
    float GetLowestPoint();
    int GetId();
    VertexArray GetVertexArray();
    IndexArray GetIndexArray();
    TextureVertexArray GetTextureArray();
    VertexArray* GetVertexArrayPtr();
    IndexArray* GetIndexArrayPtr();
    TextureVertexArray* GetTextureArrayPtr();
private:
    VertexArray m_vertex_arr;
    IndexArray m_index_arr;
    TextureVertexArray m_texture_arr;

    int m_id = -1;
    int m_index_offset = 0;

    float m_lowest_point = 0;
};

class SceneVertices {
public:
    void AddModel(Model3D* model);
    void FillBuffers();
    std::vector<float>* GetVertexBuffer();
    std::vector<float>* GetTextureBuffer();
    std::vector<int>* GetIndexBuffer();
    int GetFaceCount();
    int GetTextureVertsCount();
    int GetVertexCount();
private:
    std::vector<Model3D*> m_models;
    int m_index_count = 0;
    int m_vertex_count = 0;
    int m_texture_vertex_count = 0;
    std::vector<int> m_offsets;

    std::vector<float> m_vertex_buffer;
    std::vector<float> m_texture_buffer;
    std::vector<int> m_index_buffer;
};