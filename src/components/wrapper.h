#ifndef WRAPPER_H
#define WRAPPER_H

#include <QWidget>

class Wrapper : public QWidget
{
    Q_OBJECT
public:
    explicit Wrapper(QWidget *parent = nullptr);
};

#endif // WRAPPER_H
