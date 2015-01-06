/**
    Cucca Game Engine - Graphics - TransformationComponent.hpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.

    @version    0.1
    @author     Miika Lehtim�ki
    @date       2014-12-27
**/


#ifndef CUCCA_GRAPHICS_TRANSFORMATIONCOMPONENT_HPP
#define CUCCA_GRAPHICS_TRANSFORMATIONCOMPONENT_HPP


#include "../Core/Component.hpp"

#include <Eigen/Dense>


namespace Cucca {

    class TransformationComponent : public Component {
    public:


    private:
        Eigen::Matrix4f transformation_;
    };

} // namespace Cucca


#endif // CUCCA_GRAPHICS_TRANSFORMATIONCOMPONENT_HPP


