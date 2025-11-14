#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QWidget>
class RenderingContext;
class SpatialContext;
class UIContext;
class SelectionContext;

/**
 * @note: This class was made a Singleton recently, so in many places it is still accessed by
 * passing its pointer instead of using the `instance` method.
 */
class ApplicationContext : public QObject {
public:
    ~ApplicationContext();

    static ApplicationContext *instance(QWidget *parent = nullptr) {
        if (!m_instance) {
            m_instance = new ApplicationContext(parent);
        }

        return m_instance;
    }

    void setContexts();

    QWidget *parentWidget() const;

    RenderingContext &renderingContext() const;
    SpatialContext &spatialContext() const;
    UIContext &uiContext() const;
    SelectionContext &selectionContext() const;

    void reset();  // resets the canvas to a blank state

private:
    ApplicationContext(QWidget *parent = nullptr);

    ApplicationContext(const ApplicationContext &) = delete;
    ApplicationContext(ApplicationContext *) = delete;

    QWidget *m_parentWidget;

    RenderingContext *m_renderingContext{nullptr};
    SpatialContext *m_spatialContext{nullptr};
    UIContext *m_uiContext{nullptr};
    SelectionContext *m_selectionContext{nullptr};

    static ApplicationContext *m_instance;
};

#endif  // APPLICATIONCONTEXT_H
