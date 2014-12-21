/**
    Cucca Game Engine - Core - ShaderObject.hpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.

    @version    0.1
    @author     Miika Lehtim�ki
    @date       2014-12-21
**/


#ifndef CUCCA_GRAPHICS_SHADEROBJECT_HPP
#define CUCCA_GRAPHICS_SHADEROBJECT_HPP


#include "../../include/Core/Resource.hpp"

#include <GL/glew.h>


namespace Cucca {

    class ShaderObject : public Resource<ShaderObject, ResourceId> {
    public:
        //  Resource init and destroy member functions
        void init(const ResourceInitInfo<ShaderObject>& initInfo,
                  const std::vector<ResourceId>& initResources,
                  const std::vector<ResourceId>& depResources,
                  ResourceManager<ResourceId>* resourceManager);
        void destroy(void);

        GLuint getId(void) const;

    private:
        GLuint objectId_;
    };

    template<> struct ResourceInitInfo<ShaderObject> : public ResourceInitInfoBase {
        enum Source {
            SOURCE_CODE,       //  Loaded from plain GLSL code
            SOURCE_BINARY      //  Loaded from precompiled binary
        } source;

        GLenum type; //  Shader type
    };

} // namespace Cucca


#endif // CUCCA_GRAPHICS_SHADEROBJECT_HPP