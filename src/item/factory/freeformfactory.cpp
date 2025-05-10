#include "freeformfactory.h"

#include "../freeform.h"

FreeformFactory::FreeformFactory() {
}
std::shared_ptr<Item> FreeformFactory::create() const {
    return std::make_shared<Freeform>();
}
