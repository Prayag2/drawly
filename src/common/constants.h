#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QMargins>
#include <QColor>
#include <QSize>

namespace Drawly {
    constexpr QMargins cleanupMargin{10, 10, 10, 10};
    constexpr int defaultEraserSize{30};

    constexpr QColor eraserBorderColor{255, 0, 0};
    constexpr QColor eraserBackgroundColor{255, 0, 0, 50};
    constexpr int eraserBorderWidth{2};

    constexpr unsigned int erasedItemColor{0x6E6E6E96};

    constexpr QColor lightBackgroundColor{248, 249, 250};
    constexpr QColor darkBackgroundColor{18, 18, 18};

    constexpr QColor lightForegroundColor{30, 30, 30};
    constexpr QColor darkForegroundColor{211, 211, 211};
};

#endif // CONSTANTS_H
