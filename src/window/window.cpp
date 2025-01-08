#include "window.h"
#include "../components/cbutton.h"
#include "../components/ctoolbar.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->m_applyCustomStyles();

    CToolBar *toolbar = new CToolBar(this);
    CButton *pen = new CButton("Pen", this);
    CButton *eraser = new CButton("Eraser", this);

    toolbar->addTool(pen);
    toolbar->addTool(eraser);
    toolbar->setPosition(Qt::BottomEdge);
    toolbar->show();
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
