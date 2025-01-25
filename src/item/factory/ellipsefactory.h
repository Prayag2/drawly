#ifndef ELLIPSEFACTORY_H
#define ELLIPSEFACTORY_H

#include "itemfactory.h"

class EllipseFactory : public ItemFactory
{
public:
    EllipseFactory();
    Item* create() const override;
};

#endif // ELLIPSEFACTORY_H
