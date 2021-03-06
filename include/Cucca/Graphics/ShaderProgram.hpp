/**
    Cucca Game Engine - Graphics - ShaderProgram.hpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.

    @version    0.1
    @author     Miika Lehtim�ki
    @date       2015-02-03
**/


#ifndef CUCCA_GRAPHICS_SHADERPROGRAM_HPP
#define CUCCA_GRAPHICS_SHADERPROGRAM_HPP


#include <Cucca/Core/Resource.hpp>

#include <GL/glew.h>


namespace Cucca {

    CUCCA_RESOURCE(ShaderProgram) {
    public:
        //  Resource init and destroy template member function declarations
        CUCCA_RESOURCE_INIT_DESTROY_DECL

        ShaderProgram(void);

        GLuint getId(void) const;

    private:
        GLuint programId_;
    };

} // namespace Cucca


#endif // CUCCA_GRAPHICS_SHADERPROGRAM_HPP

