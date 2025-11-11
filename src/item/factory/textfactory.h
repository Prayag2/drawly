#ifndef TEXTFACTORY_H
#define TEXTFACTORY_H

#include "itemfactory.h"

class TextFactory : public ItemFactory {
public:
    TextFactory();
    std::shared_ptr<Item> create() const override;
};

#endif  // TEXTFACTORY_H
