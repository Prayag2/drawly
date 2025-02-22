#include "rectanglefactory.h"
#include "../rectangle.h"

RectangleFactory::RectangleFactory() {}

std::shared_ptr<Item> RectangleFactory::create() const {
    return std::make_shared<Rectangle>();
}
