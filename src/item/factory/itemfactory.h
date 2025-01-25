#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

class Item;

class ItemFactory
{
public:
    ItemFactory();
    virtual Item* create() const = 0;
};

#endif // ITEMFACTORY_H
