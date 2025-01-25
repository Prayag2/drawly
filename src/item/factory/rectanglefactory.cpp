#include "rectanglefactory.h"
#include "../rectangle.h"

RectangleFactory::RectangleFactory() {}

Item* RectangleFactory::create() const {
    return new Rectangle();
}
