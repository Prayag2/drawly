#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainWindow : public QWidget {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  private:
    bool m_config_useSystemStyles {true};
    void m_applyCustomStyles();
};
#endif // MAINWINDOW_H
