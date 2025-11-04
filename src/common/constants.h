#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QColor>
#include <QMargins>
#include <QSize>
#include <array>

namespace Common {
constexpr QMargins cleanupMargin{10, 10, 10, 10};
constexpr int defaultEraserSize{30};

constexpr QColor eraserBorderColor{255, 0, 0};
constexpr QColor eraserBackgroundColor{255, 0, 0, 50};
constexpr int eraserBorderWidth{2};

constexpr QColor selectionBorderColor{67, 135, 244, 255};
constexpr QColor selectionBackgroundColor{67, 135, 244, 50};

constexpr unsigned int erasedItemColor{0x6E6E6E96};

constexpr QColor lightBackgroundColor{248, 249, 250};
constexpr QColor darkBackgroundColor{18, 18, 18};

constexpr QColor lightForegroundColor{30, 30, 30};
constexpr QColor darkForegroundColor{211, 211, 211};

constexpr int maxItemOpacity{255};
constexpr int eraseItemOpacity{50};

constexpr double defaultTextBoxWidth{5}; // in pixels
constexpr std::array<QChar, 38> wordSeparators{
    '\t', '\n', '\v', '\f', '\r', ' ',   // whitespace
    '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(', ')', '*',
    '+',  ',',  '-',  '.',  '/',  ':',  ';', '<', '=', '>',
    '?',  '@',  '[',  '\\', ']', '^',  '_',  '`', '{', '|',
    '}',  '~'
};

constexpr int doubleClickInterval{300}; // milliseconds

};  // namespace Common

#endif  // CONSTANTS_H
