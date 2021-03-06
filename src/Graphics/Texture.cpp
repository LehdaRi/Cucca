/**
    Cucca Game Engine - Graphics - Texture.cpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.

    @version    0.1
    @author     Miika Lehtim�ki
    @date       2015-05-29
**/


#include <Cucca/Graphics/Texture.hpp>


using namespace Cucca;


Texture::Texture(void) :
    textureId_(0)
{}

void Texture::bind(GLenum target) const {
    glBindTexture(target, textureId_);
}
