#include "ellipsefactory.h"
#include "../ellipse.h"

EllipseFactory::EllipseFactory() {}

Item* EllipseFactory::create() const {
    return new Ellipse();
}
