#ifndef CACHEGRID_H
#define CACHEGRID_H
#include <QHash>
#include <QPixmap>
#include <QPoint>

class CacheGrid;

// Based on LRU cache
class CacheCell {
public:
    static QSize cellSize();
    static int counter;

    CacheCell(const QPoint &point);
    ~CacheCell();

    QRect rect() const;
    const QPoint &point() const;
    bool dirty() const;
    void setDirty(bool dirty);
    QPixmap &image() const;
    QPainter &painter() const;

private:
    QPoint m_point{};
    std::unique_ptr<QPixmap> m_image{nullptr};
    std::unique_ptr<QPainter> m_painter{};
    std::weak_ptr<CacheCell> nextCell{};
    std::weak_ptr<CacheCell> prevCell{};
    bool m_dirty{};

    // CacheGrid can access private members
    friend CacheGrid;
};

class CacheGrid {
public:
    CacheGrid(int maxSize);
    ~CacheGrid();

    QVector<std::shared_ptr<CacheCell>> queryCells(const QRect &rect);
    std::shared_ptr<CacheCell> cell(const QPoint &point);
    void markDirty(const QRect &rect);
    void markAllDirty();
    void setSize(int newSize);
    int size() const;

private:
    QHash<QPoint, std::shared_ptr<CacheCell>> m_grid{};
    std::shared_ptr<CacheCell> m_headCell{std::make_shared<CacheCell>(QPoint{0, 0})};
    std::shared_ptr<CacheCell> m_tailCell{std::make_shared<CacheCell>(QPoint{0, 0})};

    QSize m_cellSize{};
    int m_curSize{0};
    int m_maxSize{0};
};

#endif  // CACHEGRID_H
