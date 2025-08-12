#ifndef ORDEREDLIST_H
#define ORDEREDLIST_H

#include <list>
#include <memory>
#include <unordered_map>
class Item;

// Keeps track of the z-index of every item
class OrderedList {
public:
    using ItemPtr = std::shared_ptr<Item>;

private:
    std::unordered_map<ItemPtr, std::list<ItemPtr>::iterator> m_itemIterMap;
    std::list<ItemPtr> m_itemList;
    std::unordered_map<ItemPtr, int> m_zIndex;

public:
    ~OrderedList();

    void insert(ItemPtr item);
    void remove(ItemPtr item);

    void bringForward(ItemPtr item);
    void sendBackward(ItemPtr item);
    void sendToBack(ItemPtr item);
    void bringToFront(ItemPtr item);
    bool hasItem(ItemPtr item) const;

    int zIndex(ItemPtr item) const;
};

#endif  // ORDEREDLIST_H
