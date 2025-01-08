#include "window.h"
#include "../components/cbutton.h"
#include "../components/canvas.h"
#include "../components/ctoolbar.h"
#include <QFile>
#include <QButtonGroup>

#include "../shapes/shape.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->m_applyCustomStyles();

    CToolBar *toolbar = new CToolBar(this);
    QButtonGroup* group = new QButtonGroup(this);

    CButton *pen = new CButton("󰏫", this);
    CButton *line = new CButton("—", this);
    CButton *rectangle = new CButton("󰹟", this);
    CButton *ellipse = new CButton("󰺡", this);
    CButton *arrow = new CButton("→", this);
    CButton *eraser = new CButton("", this);

    pen->setCheckable(true);
    line->setCheckable(true);
    rectangle->setCheckable(true);
    ellipse->setCheckable(true);
    arrow->setCheckable(true);
    eraser->setCheckable(true);

    group->addButton(pen);
    group->addButton(line);
    group->addButton(rectangle);
    group->addButton(ellipse);
    group->addButton(arrow);
    group->addButton(eraser);

    group->setId(pen, static_cast<int>(ShapeType::Stroke));
    group->setId(line, static_cast<int>(ShapeType::Line));
    group->setId(rectangle, static_cast<int>(ShapeType::Rectangle));
    group->setId(ellipse, static_cast<int>(ShapeType::Ellipse));
    group->setId(arrow, static_cast<int>(ShapeType::Arrow));
    group->setId(eraser, static_cast<int>(ShapeType::Eraser));

    toolbar->addTool(pen);
    toolbar->addTool(line);
    toolbar->addTool(rectangle);
    toolbar->addTool(ellipse);
    toolbar->addTool(arrow);
    toolbar->addTool(eraser);
    toolbar->show();

    Canvas* canvas = new Canvas (this);
    QObject::connect(group, &QButtonGroup::idClicked, canvas, [=](int id){
        canvas->setShape(static_cast<ShapeType>(id));
    });
}

MainWindow::~MainWindow() {}

void MainWindow::m_applyCustomStyles() {
    if (this->m_config_useSystemStyles) return;

    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString qss = stream.readAll();
        this->setStyleSheet(qss);
    } else {
        qWarning() << "Failed to load stylesheet.";
    }
}
