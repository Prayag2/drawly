#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDateTime>
#include <QMouseEvent>
#include <QObject>
class ApplicationContext;

class Controller : public QObject {
    Q_OBJECT

public:
    Controller(QObject *parent = nullptr);
    ~Controller();

public slots:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseDoubleClick(QMouseEvent *event);
    void mouseTripleClick(QMouseEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void inputMethodInvoked(QInputMethodEvent *event);
    void tablet(QTabletEvent *event);
    void wheel(QWheelEvent *event);

private:
    ApplicationContext *m_context{};
    qint64 m_lastTime{};
    qint64 m_lastClickTime{};
    int m_clickCount{}; // for double/triple clicks

    bool m_mouseMoved{false};
};

#endif  // CONTROLLER_H
