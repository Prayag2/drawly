#include "textfactory.h"

#include "../text.h"

TextFactory::TextFactory() {
}

std::shared_ptr<Item> TextFactory::create() const {
    return std::make_shared<TextItem>();
}
