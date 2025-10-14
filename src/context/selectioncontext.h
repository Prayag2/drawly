#ifndef SELECTIONCONTEXT_H
#define SELECTIONCONTEXT_H

#include <QWidget>
#include <unordered_set>
class Tool;
class Item;
class ApplicationContext;

class SelectionContext : public QObject {
public:
    SelectionContext(ApplicationContext *context);
    ~SelectionContext();

    std::unordered_set<std::shared_ptr<Item>> &selectedItems();
    QRectF selectionBox() const;

private:
    std::unordered_set<std::shared_ptr<Item>> m_selectedItems{};

    ApplicationContext *m_applicationContext;
};

#endif  // SELECTIONCONTEXT_H
