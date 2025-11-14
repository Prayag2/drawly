#include "ellipsefactory.h"

#include "../ellipse.h"

EllipseFactory::EllipseFactory() {
}

std::shared_ptr<Item> EllipseFactory::create() const {
    return std::make_shared<EllipseItem>();
}
