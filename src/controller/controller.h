#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDateTime>
#include <QMouseEvent>
#include <QObject>
class ApplicationContext;

class Controller : public QObject {
    Q_OBJECT

public:
    Controller(QObject* parent = nullptr);
    ~Controller();

    void setContext(ApplicationContext* context);

public slots:
    void mousePressed(QMouseEvent* event);
    void mouseMoved(QMouseEvent* event);
    void mouseReleased(QMouseEvent* event);
    void keyPressed(QKeyEvent* event);
    void keyReleased(QKeyEvent* event);
    void inputMethodInvoked(QInputMethodEvent* event);
    void tablet(QTabletEvent* event);
    void wheel(QWheelEvent* event);

private:
    ApplicationContext* m_context{};
    qint64 m_lastTime{};
};

#endif  // CONTROLLER_H
