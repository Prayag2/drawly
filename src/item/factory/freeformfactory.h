#ifndef FREEFORMFACTORY_H
#define FREEFORMFACTORY_H

#include "itemfactory.h"

class FreeformFactory : public ItemFactory {
  public:
    FreeformFactory();
    std::shared_ptr<Item> create() const override;
};

#endif // FREEFORMFACTORY_H
