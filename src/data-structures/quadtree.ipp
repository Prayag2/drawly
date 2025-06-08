#include "../common/utils.h"
#include "../item/item.h"
#include "orderedlist.h"
#include <cstdlib>
#include <memory>
#include <unordered_map>

template <typename Shape>
QVector<std::shared_ptr<Item>> QuadTree::queryItems(const Shape& shape,
                                                    bool onlyBoundingBox) const {
    QVector<std::shared_ptr<Item>> curItems{};
    std::unordered_map<std::shared_ptr<Item>, bool> itemAlreadyPushed{};

    // look for matches and store the result in curItems
    query(shape, onlyBoundingBox, curItems, itemAlreadyPushed);

    // sort based on z-index
    std::sort(curItems.begin(), curItems.end(), [&](auto& firstItem, auto& secondItem) {
        return m_orderedList->zIndex(firstItem) < m_orderedList->zIndex(secondItem);
    });

    return curItems;
};

template <typename Shape>
void QuadTree::query(const Shape& shape, bool onlyBoundingBox, QVector<std::shared_ptr<Item>>& out,
                     std::unordered_map<std::shared_ptr<Item>, bool>& itemAlreadyPushed) const {
    if (!Common::intersects(m_boundingBox, shape)) {
        return;
    }

    for (const std::shared_ptr<Item>& item : m_items) {
        if (Common::intersects(item->boundingBox(), shape)) {
            if (onlyBoundingBox || item->intersects(shape)) {
                // using the hash map because multiple nodes may have a pointer to the same item
                if (!itemAlreadyPushed[item]) {
                    out.push_back(item);
                    itemAlreadyPushed[item] = true;
                }
            }
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->query(shape, onlyBoundingBox, out, itemAlreadyPushed);
        m_topRight->query(shape, onlyBoundingBox, out, itemAlreadyPushed);
        m_bottomRight->query(shape, onlyBoundingBox, out, itemAlreadyPushed);
        m_bottomLeft->query(shape, onlyBoundingBox, out, itemAlreadyPushed);
    }
}
