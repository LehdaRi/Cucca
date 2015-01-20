#include "../../include/UnitTests/UnitTest3.hpp"


#if CURRENT_TEST == 3


#include "../../include/Core/Device.hpp"
#include "../../include/Core/ThreadPool.hpp"
#include "../../include/Core/ResourceManager.hpp"
#include "../../include/CoreExtensions/Canvas_SFML.hpp"
#include "../../include/CoreExtensions/EventVisitor_SFML.hpp"
#include "../../include/Core/Binary.hpp"
#include "../../include/Graphics/ShaderObject.hpp"
#include "../../include/Graphics/ShaderProgram.hpp"
#include "../../include/Graphics/Texture.hpp"
#include "../../include/Graphics/Material.hpp"
#include "../../include/Graphics/VertexData.hpp"
#include "../../include/Graphics/Mesh.hpp"
#include "../../include/Graphics/TransformationComponent.hpp"
#include "../../include/Graphics/MeshComponent.hpp"
#include "../../include/Graphics/BasicCamera.hpp"

#include <random>


#define rndf ((float)rnd() / rnd.max())


using namespace Cucca;


int unitTest(void) {
    //  First things first
    std::default_random_engine rnd;
    auto device = DEVICE(Canvas_SFML);
    /*GLenum err = */glewInit();

    ThreadPool pool;
    pool.launchThreads(2);

    ResourceManager<ResourceId> manager(device->getGraphicsTaskQueue(), pool.getTaskQueue());

    Node* root = device->getRoot();

    //  Resources
    ResourceInitInfo<Binary> vertexShaderBinaryInitInfo1;
    vertexShaderBinaryInitInfo1.source = ResourceInitInfo<Binary>::SOURCE_FILE;
    vertexShaderBinaryInitInfo1.fileName = "res/shaders/VS_SingleTexture.glsl";
    manager.addResourceInfo<Binary>("BINARY_SHADER_VERTEX_1", vertexShaderBinaryInitInfo1);

    ResourceInitInfo<Binary> fragmentShaderBinaryInitInfo1;
    fragmentShaderBinaryInitInfo1.source = ResourceInitInfo<Binary>::SOURCE_FILE;
    fragmentShaderBinaryInitInfo1.fileName = "res/shaders/FS_SingleTexture.glsl";
    manager.addResourceInfo<Binary>("BINARY_SHADER_FRAGMENT_1", fragmentShaderBinaryInitInfo1);

    ResourceInitInfo<ShaderObject> vertexShaderInitInfo1;
    vertexShaderInitInfo1.source = ResourceInitInfo<ShaderObject>::SOURCE_CODE;
    vertexShaderInitInfo1.type = GL_VERTEX_SHADER;
    manager.addResourceInfo<ShaderObject>("SHADER_VERTEX_1",
                                          vertexShaderInitInfo1,
                                          std::vector<ResourceId>{ "BINARY_SHADER_VERTEX_1" },
                                          std::vector<ResourceId>(),
                                          true);

    ResourceInitInfo<ShaderObject> fragmentShaderInitInfo1;
    fragmentShaderInitInfo1.source = ResourceInitInfo<ShaderObject>::SOURCE_CODE;
    fragmentShaderInitInfo1.type = GL_FRAGMENT_SHADER;
    manager.addResourceInfo<ShaderObject>("SHADER_FRAGMENT_1",
                                          fragmentShaderInitInfo1,
                                          std::vector<ResourceId>{ "BINARY_SHADER_FRAGMENT_1" },
                                          std::vector<ResourceId>(),
                                          true);

    ResourceInitInfo<ShaderProgram> shaderProgramInitInfo1;
    manager.addResourceInfo<ShaderProgram>("SHADER_PROGRAM_1",
                                           shaderProgramInitInfo1,
                                           std::vector<ResourceId>{ "SHADER_VERTEX_1", "SHADER_FRAGMENT_1" },
                                           std::vector<ResourceId>(),
                                           true);

    ResourceInitInfo<Binary> textureBinaryInitInfo1;
    textureBinaryInitInfo1.source = ResourceInitInfo<Binary>::SOURCE_FILE;
    textureBinaryInitInfo1.fileName = "res/models/cube.png";
    manager.addResourceInfo<Binary>("BINARY_TEXTURE_1", textureBinaryInitInfo1);

    ResourceInitInfo<Texture> textureInitInfo1;
    textureInitInfo1.source = ResourceInitInfo<Texture>::SOURCE_BINARY_PNG;
    textureInitInfo1.wrapS = GL_REPEAT;
    textureInitInfo1.wrapT = GL_REPEAT;
    textureInitInfo1.minFiltering = GL_LINEAR_MIPMAP_LINEAR;
    textureInitInfo1.magFiltering = GL_LINEAR;
    manager.addResourceInfo<Texture>("TEXTURE_1",
                                     textureInitInfo1,
                                     std::vector<ResourceId>{ "BINARY_TEXTURE_1" },
                                     std::vector<ResourceId>(),
                                     true);

    ResourceInitInfo<Material> materialInitInfo1;
    manager.addResourceInfo<Material>("MATERIAL_1",
                                      materialInitInfo1,
                                      std::vector<ResourceId>(),
                                      std::vector<ResourceId>{ "SHADER_PROGRAM_1", "TEXTURE_1" },
                                      true);

    ResourceInitInfo<Binary> vertexDataBinaryInitInfo1;
    vertexDataBinaryInitInfo1.source = ResourceInitInfo<Binary>::SOURCE_FILE;
    vertexDataBinaryInitInfo1.fileName = "res/models/cube.obj";
    manager.addResourceInfo<Binary>("BINARY_VERTEX_DATA_1", vertexDataBinaryInitInfo1);

    ResourceInitInfo<VertexData> vertexDataInitInfo1;
    vertexDataInitInfo1.source = ResourceInitInfo<VertexData>::SOURCE_BINARY_OBJ;
    manager.addResourceInfo<VertexData>("VERTEX_DATA_1",
                                        vertexDataInitInfo1,
                                        std::vector<ResourceId>{ "BINARY_VERTEX_DATA_1" },
                                        std::vector<ResourceId>());

    ResourceInitInfo<Mesh> meshInitInfo1;
    manager.addResourceInfo<Mesh>("MESH_1",
                                  meshInitInfo1,
                                  std::vector<ResourceId>{ "VERTEX_DATA_1" },
                                  std::vector<ResourceId>{ "MATERIAL_1" },
                                  true);

    auto mesh1 = manager.getResource<Mesh>("MESH_1");

    //  Nodes
    Node eventNode;
    eventNode.addComponent(EventComponent());
    device->subscribeEvents(eventNode.getComponents<EventComponent>().back(), EventBase::getEventTypeId<sf::Event>());
    root->addChild(std::move(eventNode));

    for (auto i=0u; i<100; ++i) {
        Node graphicsNode;
        graphicsNode.addComponent(TransformationComponent());
        graphicsNode.getComponents<TransformationComponent>().back()->translate(Vector3Glf{ 25.0f - 50.0f*rndf, 5.0f - 10.0f*rndf, 25.0f - 50.0f*rndf }, true);
        graphicsNode.addComponent(MeshComponent(mesh1));
        root->addChild(std::move(graphicsNode));
    }

    //  Visitors
    BasicCamera camera;
    camera.lookAt(Vector3Glf{ 0.0f, 0.0f, 1.0f },
                  Vector3Glf{ 0.0f, 0.0f, 0.0f },
                  Vector3Glf{ 0.0f, -1.0f, 0.0f });
    camera.projection(1.5708f, 4.0f/3.0f, 0.05f, 100.0f);

    EventVisitor_SFML sfmlEventVisitor;

    //  Some variables
    float t(0.0f);

    //  Run it
    while (device->status() == Device<Canvas_SFML>::STATUS_RUNNING) {
        device->handleEvents();
        device->getRoot()->accept(sfmlEventVisitor);
        device->getRoot()->accept(camera);
        device->render();

        t += 0.001f;
        camera.lookAt(Vector3Glf{ 10.0f*cosf(t*4.0f), 1.5f + 2.5f*sinf(t*1.276f), 10.0f*sinf(t*4.0f) },
                      Vector3Glf{ 0.0f, 0.0f, 0.0f },
                      Vector3Glf{ 0.0f, 1.0f, 0.0f });
    }

    return 0;
}


#endif // CURRENT_TEST
