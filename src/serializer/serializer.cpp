#include <QJsonDocument>
#include <QStandardPaths>
#include <QJsonArray>
#include <QFileDialog>
#include "serializer.h"
#include "../data-structures/quadtree.h"
#include "../context/spatialcontext.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../item/item.h"
#include "../item/polygon.h"
#include "../item/freeform.h"
#include "../item/text.h"
#include <memory>

Serializer::Serializer() {}

void Serializer::serialize(ApplicationContext *context) {
    QVector<std::shared_ptr<Item>> items{context->spatialContext().quadtree().getAllItems()};

    QJsonArray array{};
    for (auto& item : items) {
        array.push_back(toJson(item));
    }

    m_object["items"] = array;

    QPointF offsetPos{context->spatialContext().offsetPos()};
    m_object["offset_pos"] = toJson(offsetPos);

    qreal zoomFactor{context->renderingContext().zoomFactor()};
    m_object["zoom_factor"] = zoomFactor;
}

QJsonObject Serializer::toJson(std::shared_ptr<Item> item) {
    QJsonObject obj{};

    obj["type"] = QJsonValue(static_cast<int>(item->type()));
    obj["bounding_box"] = toJson(item->boundingBox());
    obj["bounding_box_padding"] = QJsonValue(item->boundingBoxPadding());
    obj["properties"] = toJson(item->properties());

    switch(item->type()) {
        case Item::Freeform: {
            std::shared_ptr<FreeformItem> freeform{std::dynamic_pointer_cast<FreeformItem>(item)};
            obj["points"] = toJson(freeform->points());
            obj["pressures"] = toJson(freeform->pressures());
            break;
        }
        case Item::Rectangle:
        case Item::Ellipse:
        case Item::Arrow:
        case Item::Line: {
            std::shared_ptr<PolygonItem> polygon{std::dynamic_pointer_cast<PolygonItem>(item)};
            obj["start"] = toJson(polygon->start());
            obj["end"] = toJson(polygon->end());
            break;
        }
        case Item::Text: {
            std::shared_ptr<TextItem> text{std::dynamic_pointer_cast<TextItem>(item)};
            obj["text"] = QJsonValue(text->text());
        }
    }

    return obj;
}

QJsonObject Serializer::toJson(const Property& property) {
    QJsonObject result{};

    result["type"] = property.type();
    result["value"] = QJsonValue::fromVariant(property.variant());

    return result;
}

QJsonObject Serializer::toJson(const QRectF& rect) {
    QJsonObject result{};
    result["x"] = QJsonValue(rect.x());
    result["y"] = QJsonValue(rect.y());
    result["width"] = QJsonValue(rect.width());
    result["height"] = QJsonValue(rect.height());

    return result;
}

QJsonObject Serializer::toJson(const QPointF& point) {
    QJsonObject result{};
    result["x"] = QJsonValue(point.x());
    result["y"] = QJsonValue(point.y());

    return result;
}

void Serializer::saveToFile() {
    QJsonDocument doc{m_object};

    qDebug() << "Saving...";

    QDir homeDir{QDir::home()};
    QString defaultFilePath = homeDir.filePath("Untitled.json");
    QString fileName{QFileDialog::getSaveFileName(nullptr, "Save File", defaultFilePath, "JSON (*.json)")};

    QFile file{fileName};
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Compact));
    file.close();

    qDebug() << "Saved to file: " << fileName;
}
