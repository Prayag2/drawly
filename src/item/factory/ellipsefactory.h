#ifndef ELLIPSEFACTORY_H
#define ELLIPSEFACTORY_H

#include "itemfactory.h"

class EllipseFactory : public ItemFactory
{
public:
    EllipseFactory();
    std::shared_ptr<Item> create() const override;
};

#endif // ELLIPSEFACTORY_H
