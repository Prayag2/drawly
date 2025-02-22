#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H
#include <memory>

class Item;

class ItemFactory
{
public:
    ItemFactory();
    virtual std::shared_ptr<Item> create() const = 0;
};

#endif // ITEMFACTORY_H
