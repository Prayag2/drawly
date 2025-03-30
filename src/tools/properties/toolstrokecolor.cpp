#include "toolstrokecolor.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QColor>

// TODO: Use a better widget and a better way to return colors instead of button id
ToolStrokeColor::ToolStrokeColor(QWidget* parent) {
    m_widget = new QWidget{parent};

    QHBoxLayout* layout {new QHBoxLayout{m_widget}};
    m_group = new QButtonGroup{m_widget};
    m_widget->setLayout(layout);

    QVector<QColor> colors {QColor{255,255,255}, QColor{255,53,71}, QColor{255,187,51}, QColor{51,181,229}, QColor{0,200,81}};

    for (const QColor& color : colors) {
        QPushButton* btn {new QPushButton{"", m_widget}};
        btn->setCheckable(true);
        btn->setStyleSheet("background-color: " + color.name());

        layout->addWidget(btn);
        m_group->addButton(btn, static_cast<int>(color.rgb()));
    }

    m_group->buttons()[0]->setChecked(true);
    m_widget->setStyleSheet(QString::asprintf("QPushButton {width: %dpx; height: %dpx};", 20, 20));
}

QString ToolStrokeColor::name() const {
    return "Color";
};

const QVariant ToolStrokeColor::value() const {
    return m_group->checkedId();
};
