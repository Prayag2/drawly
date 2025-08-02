#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QWidget>
class RenderingContext;
class SpatialContext;
class UIContext;
class SelectionContext;

class ApplicationContext : public QObject {
public:
    ApplicationContext(QWidget* parent = nullptr);
    ~ApplicationContext();

    void setContexts();

    QWidget* parentWidget() const;

    RenderingContext& renderingContext() const;
    SpatialContext& spatialContext() const;
    UIContext& uiContext() const;
    SelectionContext& selectionContext() const;

private:
    QWidget* m_parentWidget;

    RenderingContext* m_renderingContext{nullptr};
    SpatialContext* m_spatialContext{nullptr};
    UIContext* m_uiContext{nullptr};
    SelectionContext* m_selectionContext{nullptr};
};

#endif  // APPLICATIONCONTEXT_H
