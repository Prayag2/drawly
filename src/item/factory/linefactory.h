#ifndef LINEFACTORY_H
#define LINEFACTORY_H

#include "itemfactory.h"

class LineFactory : public ItemFactory
{
public:
    LineFactory();
    std::shared_ptr<Item> create() const override;
};

#endif // LINEFACTORY_H
