#include "arrowfactory.h"
#include "../arrow.h"

ArrowFactory::ArrowFactory() {}

std::shared_ptr<Item> ArrowFactory::create() const {
    return std::make_shared<Arrow>();
}
