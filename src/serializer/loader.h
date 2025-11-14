#ifndef LOADER_H
#define LOADER_H

#include <QJsonObject>
#include <memory>
class ApplicationContext;
class Item;
class Property;

class Loader {
public:
    void loadFromFile(ApplicationContext *context);

private:
    static std::shared_ptr<Item> createItem(const QJsonObject &obj);
    static Property createProperty(const QJsonObject &obj);

    static QJsonValue value(const QJsonObject &obj, const QString &key);
    static QJsonObject object(const QJsonValue &value);
    static QJsonArray array(const QJsonValue &value);

    static QPointF toPointF(const QJsonValue &val);
};

#endif
