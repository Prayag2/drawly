#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMouseEvent>
class ApplicationContext;

class Controller : public QObject {
    Q_OBJECT

  public:
    Controller(QObject *parent = nullptr);

    void setContext(ApplicationContext *context);

  public slots:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void wheel(QWheelEvent *event);

  private:
    ApplicationContext *m_context {};
};

#endif // CONTROLLER_H
