#include "linefactory.h"

#include "../line.h"

LineFactory::LineFactory() {
}

std::shared_ptr<Item> LineFactory::create() const {
    return std::make_shared<LineItem>();
}
