#include "window.h"

#include "../canvas/canvas.h"
#include "../components/actionbar.h"
#include "../components/propertybar.h"
#include "../components/toolbar.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/uicontext.h"
#include "../controller/controller.h"
#include "boardlayout.h"
#include <QButtonGroup>
#include <QFile>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    this->m_applyCustomStyles();

    BoardLayout *layout{new BoardLayout(this)};
    Controller *controller{new Controller(this)};
    ApplicationContext *context{ApplicationContext::instance()};

    RenderingContext &renderingContext{context->renderingContext()};
    UIContext &uiContext{context->uiContext()};

    renderingContext.canvas().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setLayout(layout);

    layout->setMargins(10);
    layout->setLeftWidget(&uiContext.propertyBar());
    layout->setBottomWidget(&uiContext.toolBar());
    layout->setTopWidget(&uiContext.actionBar());
    layout->setCentralWidget(&renderingContext.canvas());

    QObject::connect(&renderingContext.canvas(),
                     &Canvas::mousePressed,
                     controller,
                     &Controller::mousePressed);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::mouseMoved,
                     controller,
                     &Controller::mouseMoved);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::mouseReleased,
                     controller,
                     &Controller::mouseReleased);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::keyPressed,
                     controller,
                     &Controller::keyPressed);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::keyReleased,
                     controller,
                     &Controller::keyReleased);
    QObject::connect(&renderingContext.canvas(), &Canvas::tablet, controller, &Controller::tablet);
    QObject::connect(&renderingContext.canvas(), &Canvas::wheel, controller, &Controller::wheel);
}

MainWindow::~MainWindow() {
}

void MainWindow::m_applyCustomStyles() {
    if (this->m_config_useSystemStyles)
        return;

    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString qss = stream.readAll();
        this->setStyleSheet(qss);
    } else {
        qWarning() << "Failed to load stylesheet.";
    }
}
