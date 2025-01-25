#ifndef RECTANGLEFACTORY_H
#define RECTANGLEFACTORY_H

#include "itemfactory.h"

class Item;

class RectangleFactory : public ItemFactory
{
public:
    RectangleFactory();
    Item* create() const override;
};

#endif // RECTANGLEFACTORY_H
