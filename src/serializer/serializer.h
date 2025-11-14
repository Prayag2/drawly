#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QJsonArray>
#include <QJsonObject>
class Item;
class Property;
class ApplicationContext;

class Serializer {
public:
    Serializer();

    void serialize(ApplicationContext *context);
    void saveToFile();

private:
    static QJsonObject toJson(const QRectF &rect);
    static QJsonObject toJson(const QPointF &point);
    static QJsonObject toJson(std::shared_ptr<Item> item);
    static QJsonObject toJson(const Property &property);

    template <typename T>
    static QJsonValue toJson(const T &val) {
        return QJsonValue(val);
    };

    template <typename T>
    static QJsonArray toJson(const QVector<T> &vector) {
        QJsonArray result{};

        for (const T &el : vector) {
            result.push_back(toJson(el));
        }

        return result;
    }

private:
    // properties
    QJsonObject m_object;
};

#endif
