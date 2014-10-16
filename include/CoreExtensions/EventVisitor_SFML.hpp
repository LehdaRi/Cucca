/**
    Cucca Game Engine - CoreExtensions - EventVisitor_SFML.hpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.

    EventVisitor_SFML is a Visitor designed to handle SFML events.

    @version    0.1
    @author     Miika Lehtim�ki
    @date       2014-10-16
**/


#ifndef CUCCA_CORE_EVENTVISITOR_SFML_HPP
#define CUCCA_CORE_EVENTVISITOR_SFML_HPP


#include "../Core/Visitor.hpp"
#include "../Core/EventComponent.hpp"

#include <SFML/Window/Event.hpp>


VISITOR(EventVisitor_SFML, EventComponent) {
public:
    void nodeEnter(Node* node, EventComponent* component);
    void nodeExit(Node* node, EventComponent* component) {}
};


#endif // CUCCA_CORE_EVENTVISITOR_SFML_HPP
