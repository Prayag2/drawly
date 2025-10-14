#ifndef RENDERINGCONTEXT_H
#define RENDERINGCONTEXT_H

#include <QTimer>
#include <QWidget>
class Canvas;
class ApplicationContext;

class RenderingContext : public QObject {
    Q_OBJECT

public:
    RenderingContext(ApplicationContext *context);
    ~RenderingContext();

    void setRenderingContext();

    Canvas &canvas() const;
    QPainter &canvasPainter() const;
    QPainter &overlayPainter() const;

    void markForRender();
    void markForUpdate();
    void markForUpdate(const QRect &region);

    qreal zoomFactor() const;
    void setZoomFactor(int diff);

    const int fps() const;

private slots:
    void beginPainters();
    void endPainters();
    void canvasResized();

private:
    Canvas *m_canvas{nullptr};
    QPainter *m_canvasPainter{};
    QPainter *m_overlayPainter{};

    QTimer m_frameTimer;

    bool m_needsReRender{false};
    bool m_needsUpdate{false};
    QRect m_updateRegion{};

    qreal m_zoomFactor{1};

    ApplicationContext *m_applicationContext;
};

#endif  // RENDERINGCONTEXT_H
