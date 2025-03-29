#include "toolstrokecolor.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QColor>

ToolStrokeColor::ToolStrokeColor(QWidget* parent) {
    m_widget = new QWidget{parent};

    QHBoxLayout* layout {new QHBoxLayout{m_widget}};
    m_group = new QButtonGroup{m_widget};
    m_widget->setLayout(layout);

    QVector<Qt::GlobalColor> colors {Qt::black, Qt::red, Qt::blue, Qt::yellow, Qt::green};
    for (const Qt::GlobalColor color : colors) {
        QPushButton* btn {new QPushButton{"", m_widget}};
        btn->setCheckable(true);
        btn->setStyleSheet("background-color: " + QColor{color}.name());
        layout->addWidget(btn);
        m_group->addButton(btn, static_cast<int>(color));
    }

    m_widget->setStyleSheet(QString::asprintf("QPushButton {width: %dpx; height: %dpx};", 20, 20));
}

QString ToolStrokeColor::name() const {
    return "Color";
};

const QVariant ToolStrokeColor::value() const {
    return static_cast<int>(m_group->checkedId());
};
