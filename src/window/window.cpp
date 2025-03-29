#include "window.h"
#include <QFile>
#include <QButtonGroup>

#include "../controller/controller.h"
#include "../context/applicationcontext.h"
#include "../components/toolbar.h"
#include "../components/propertybar.h"
#include "../canvas/canvas.h"
#include "boardlayout.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    this->m_applyCustomStyles();

    BoardLayout *layout = new BoardLayout(this);
    Controller *controller = new Controller(this);
    ApplicationContext *context = new ApplicationContext(this);
    controller->setContext(context);

    context->canvas().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setLayout(layout);

    layout->setLeftWidget(&context->propertyBar());
    layout->setBottomWidget(&context->toolBar());
    layout->setCentralWidget(&context->canvas());

    QObject::connect(&context->canvas(), &Canvas::mousePressed, controller, &Controller::mousePressed);
    QObject::connect(&context->canvas(), &Canvas::mouseMoved, controller, &Controller::mouseMoved);
    QObject::connect(&context->canvas(), &Canvas::mouseReleased, controller, &Controller::mouseReleased);
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
