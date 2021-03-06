/**
    Cucca Game Engine - Graphics - VertexData_Init_Binary.hpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.


    This file is a resource initialization file.
    It specifies VertexDataInitInfo_Binary struct and VertexData
    init and destroy member function template specializations for
    initializing VertexData resources from a Binary resource.

    Initialization resources:
        0: Binary
    Dependency resources:
        none


    @version    0.1
    @author     Miika Lehtim�ki
    @date       2015-02-04
**/


#ifndef CUCCA_GRAPHICS_VERTEXDATA_INIT_BINARY_HPP
#define CUCCA_GRAPHICS_VERTEXDATA_INIT_BINARY_HPP


#include <Cucca/Core/ResourceManager.hpp>
#include <Cucca/Graphics/VertexData.hpp>
#include <Cucca/Core/Binary.hpp>

#include <map>
#include <cstring>


namespace Cucca {

    /// Initialization info struct
    struct VertexDataInitInfo_Binary : public ResourceInitInfoBase {
        enum Source {
            SOURCE_BINARY_OBJ,      //  from .obj file
        } source;
    };


    /// Resource init and destroy template member function specializations
    CUCCA_RESOURCE_INIT(VertexData, VertexDataInitInfo_Binary) {
        switch (initInfo.source) {
        case VertexDataInitInfo_Binary::SOURCE_BINARY_OBJ:
            {
                if (initResources.size() == 0)
                    return; // TODO_EXCEPTION: maybe throw an exception instead?

                positions_.clear();
                texCoords_.clear();
                normals_.clear();
                indices_.clear();

                char* buffer = resourceManager->getResource<Binary>(initResources[0])->getBufferPtr();

                char lineHeader[32];
                std::vector<std::array<float, 4>> positions;
                std::vector<std::array<float, 3>> texCoords;
                std::vector<std::array<float, 3>> normals;
                std::vector<std::array<unsigned, 9>> indices;

                while(*buffer) {
                    if (sscanf(buffer, "%s", lineHeader) == 0)
                        return;

                    buffer += strlen(lineHeader)+1;

                    if (strcmp(lineHeader, "v") == 0) {
                        std::array<float, 4> position = {0.0f, 0.0f, 0.0f, 1.0f};
                        if (sscanf(buffer, "%f %f %f %f", &position[0], &position[1], &position[2], &position[3]) < 3)
                            throw "VertexData: invalid file!"; // TODO_EXCEPTION
                        positions.push_back(std::move(position));
                    }
                    else if (strcmp(lineHeader, "vt") == 0) {
                        std::array<float, 3> texCoord = {0.0f, 0.0f, 0.0f};
                        if (sscanf(buffer, "%f %f %f", &texCoord[0], &texCoord[1], &texCoord[2]) < 2)
                            throw "VertexData: invalid file!"; // TODO_EXCEPTION
                        texCoords.push_back(std::move(texCoord));
                    }
                    else if (strcmp(lineHeader, "vn") == 0) {
                        std::array<float, 3> normal = {0.0f, 0.0f, 0.0f};
                        if (sscanf(buffer, "%f %f %f", &normal[0], &normal[1], &normal[2]) < 3)
                            throw "VertexData: invalid file!"; // TODO_EXCEPTION
                        normals.push_back(std::move(normal));
                    }
                    else if (strcmp(lineHeader, "f") == 0) {
                        std::array<unsigned, 9> index = {0, 0, 0,  0, 0, 0,  0, 0, 0};
                        if (sscanf(buffer, "%u %u %u", &index[0], &index[1], &index[2]) == 3 ||
                            sscanf(buffer, "%u//%u %u//%u %u//%u", &index[0], &index[6], &index[1], &index[7], &index[2], &index[8]) == 6 ||
                            sscanf(buffer, "%u/%u %u/%u %u/%u", &index[0], &index[3], &index[1], &index[4], &index[2], &index[5]) == 6 ||
                            sscanf(buffer, "%u/%u/%u %u/%u/%u %u/%u/%u", &index[0], &index[3], &index[6], &index[1], &index[4], &index[7], &index[2], &index[5], &index[8]) == 9)
                            indices.push_back(std::move(index));
                        else
                            throw "VertexData: invalid file!"; // TODO_EXCEPTION
                    }

                    while (*buffer++ != 10)
                        if (*buffer == 0)
                            break;
                }

                std::map<std::array<unsigned, 3>, unsigned> createdVertices;

                usingTexCoords_ = texCoords.size() > 0;
                usingNormals_ = normals.size() > 0;
                usingIndexing_ = true;

                for (auto& indexArray : indices) {
                    if (usingTexCoords_ && (indexArray[3] == 0 || indexArray[4] == 0 || indexArray[5] == 0))
                        throw "VertexData: invalid index data (texture coordinates)";

                    if (usingNormals_ && (indexArray[6] == 0 || indexArray[7] == 0 || indexArray[8] == 0))
                        throw "VertexData: invalid index data (normals)";

                    std::array<unsigned, 3> v1 = { indexArray[0], indexArray[3], indexArray[6] };
                    std::array<unsigned, 3> v2 = { indexArray[1], indexArray[4], indexArray[7] };
                    std::array<unsigned, 3> v3 = { indexArray[2], indexArray[5], indexArray[8] };

                    if (createdVertices[v1] == 0) {
                        positions_.push_back(positions.at(indexArray[0]-1));
                        if (usingTexCoords_)
                            texCoords_.push_back(texCoords.at(indexArray[3]-1));
                        if (usingNormals_)
                            normals_.push_back(normals.at(indexArray[6]-1));

                        createdVertices[v1] = positions_.size()-1;
                    }
                    indices_.push_back(createdVertices[v1]);

                    if (createdVertices[v2] == 0) {
                        positions_.push_back(positions.at(indexArray[1]-1));
                        if (usingTexCoords_)
                            texCoords_.push_back(texCoords.at(indexArray[4]-1));
                        if (usingNormals_)
                            normals_.push_back(normals.at(indexArray[7]-1));

                        createdVertices[v2] = positions_.size()-1;
                    }
                    indices_.push_back(createdVertices[v2]);

                    if (createdVertices[v3] == 0) {
                        positions_.push_back(positions.at(indexArray[2]-1));
                        if (usingTexCoords_)
                            texCoords_.push_back(texCoords.at(indexArray[5]-1));
                        if (usingNormals_)
                            normals_.push_back(normals.at(indexArray[8]-1));

                        createdVertices[v3] = positions_.size()-1;
                    }
                    indices_.push_back(createdVertices[v3]);
                }
            }
        break;
        }
    }

    CUCCA_RESOURCE_DESTROY(VertexData, VertexDataInitInfo_Binary) {}

} // namespace Cucca


#endif // CUCCA_GRAPHICS_VERTEXDATA_INIT_BINARY_HPP
