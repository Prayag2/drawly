/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "loader.h"

#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/arrow.h"
#include "../item/ellipse.h"
#include "../item/freeform.h"
#include "../item/item.h"
#include "../item/line.h"
#include "../item/rectangle.h"
#include "../item/text.h"
#include <QDir>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <memory>

void Loader::loadFromFile(ApplicationContext *context) {
    QDir homeDir{QDir::home()};
    QString fileName{
        QFileDialog::getOpenFileName(nullptr, "Open File", homeDir.path(), "JSON (*.json)")};
    QFile file{fileName};

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return;
    }

    QByteArray byteArray;
    QByteArray line;
    QTextStream in(&file);

    while (!in.atEnd()) {
        line = in.readLine().toUtf8();
        byteArray.append(line);
        byteArray.append('\n');
    }

    QJsonDocument doc = QJsonDocument::fromJson(byteArray);
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject docObj = doc.object();

        context->reset();
        QuadTree &quadtree{context->spatialContext().quadtree()};

        QJsonArray itemsArray = array(value(docObj, "items"));
        for (const QJsonValue &value : itemsArray) {
            QJsonObject itemObj = object(value);

            std::shared_ptr<Item> item = createItem(itemObj);
            quadtree.insertItem(item);
        }

        qreal zoomFactor = value(docObj, "zoom_factor").toDouble();
        context->renderingContext().setZoomFactor(zoomFactor);

        QPointF offsetPos = toPointF(value(docObj, "offset_pos"));
        context->spatialContext().setOffsetPos(offsetPos);
    } else {
        qWarning() << "JSON document is not an object or is invalid";
    }

    context->spatialContext().cacheGrid().markAllDirty();
    context->renderingContext().markForRender();
    context->renderingContext().markForUpdate();
}

std::shared_ptr<Item> Loader::createItem(const QJsonObject &obj) {
    Item::Type type{static_cast<Item::Type>(value(obj, "type").toInt())};

    std::shared_ptr<Item> item;
    switch (type) {
        case Item::Freeform: {
            std::shared_ptr<FreeformItem> cur{std::make_shared<FreeformItem>()};
            QJsonArray points = array(value(obj, "points"));
            QJsonArray pressures = array(value(obj, "pressures"));

            qsizetype len{points.size()};
            for (qsizetype pos{0}; pos < len; pos++) {
                QPointF point{toPointF(points[pos])};

                cur->addPoint(point, pressures[pos].toDouble());
            }

            item = cur;
            break;
        }
        case Item::Rectangle: {
            std::shared_ptr<RectangleItem> cur{std::make_shared<RectangleItem>()};
            cur->setStart(toPointF(value(obj, "start")));
            cur->setEnd(toPointF(value(obj, "end")));

            item = cur;
            break;
        }
        case Item::Line: {
            std::shared_ptr<LineItem> cur{std::make_shared<LineItem>()};
            cur->setStart(toPointF(value(obj, "start")));
            cur->setEnd(toPointF(value(obj, "end")));

            item = cur;
            break;
        }
        case Item::Arrow: {
            std::shared_ptr<ArrowItem> cur{std::make_shared<ArrowItem>()};
            cur->setStart(toPointF(value(obj, "start")));
            cur->setEnd(toPointF(value(obj, "end")));

            item = cur;
            break;
        }
        case Item::Ellipse: {
            std::shared_ptr<EllipseItem> cur{std::make_shared<EllipseItem>()};
            cur->setStart(toPointF(value(obj, "start")));
            cur->setEnd(toPointF(value(obj, "end")));

            item = cur;
            break;
        }
        case Item::Text: {
            std::shared_ptr<TextItem> cur{std::make_shared<TextItem>()};
            QPointF topLeft = toPointF(value(obj, "bounding_box"));

            cur->createTextBox(topLeft);
            cur->insertText(value(obj, "text").toString());

            item = cur;
            break;
        }
    }

    QJsonArray properties = array(value(obj, "properties"));
    for (const QJsonValue &propertyValue : properties) {
        Property prop{createProperty(object(propertyValue))};
        item->setProperty(prop.type(), prop);
    }

    item->setBoundingBoxPadding(value(obj, "bounding_box_padding").toInt());

    return item;
}

Property Loader::createProperty(const QJsonObject &obj) {
    Property::Type type{static_cast<Property::Type>(value(obj, "type").toInt())};
    QVariant val{value(obj, "value").toVariant()};

    return Property{val, type};
}

QJsonValue Loader::value(const QJsonObject &obj, const QString &key) {
    if (!obj.contains(key)) {
        qWarning() << "Object does not contain key: " << key;
        return {};
    }

    return obj.value(key);
}

QJsonObject Loader::object(const QJsonValue &value) {
    if (value.isUndefined() || !value.isObject()) {
        qWarning() << "Value is not an object";
        return {};
    }

    return value.toObject();
}

QJsonArray Loader::array(const QJsonValue &value) {
    if (value.isUndefined() || !value.isArray()) {
        qWarning() << "Value is not an array";
        return {};
    }

    return value.toArray();
}

QPointF Loader::toPointF(const QJsonValue &val) {
    QJsonObject obj = object(val);

    if (obj.contains("x") && obj.contains("y")) {
        return QPointF{obj["x"].toDouble(), obj["y"].toDouble()};
    } else {
        qWarning() << "Given point does not contain x and y properties";
        return {};
    }

    qWarning() << "Given point value is not an object";
    return {};
}
