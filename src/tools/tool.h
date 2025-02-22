#ifndef TOOL_H
#define TOOL_H

#include <QCursor>
class ApplicationContext;

// INTERFACE
class Tool {
  public:
    Tool();
    virtual ~Tool() = default;

    const QCursor& cursor() const;
    virtual QString iconAlt() const = 0;
    virtual void mousePressed(ApplicationContext *context) = 0;
    virtual void mouseMoved(ApplicationContext *context) = 0;
    virtual void mouseReleased(ApplicationContext *context) = 0;

  protected:
    QCursor m_cursor {};
};

#endif // TOOL_H
