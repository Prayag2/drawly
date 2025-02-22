#ifndef ARROWFACTORY_H
#define ARROWFACTORY_H

#include "itemfactory.h"

class ArrowFactory : public ItemFactory {
  public:
    ArrowFactory();
    std::shared_ptr<Item> create() const override;
};

#endif // ARROWFACTORY_H
