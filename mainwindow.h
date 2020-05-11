#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "manager.h"
#include <QRect>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
    Ui::MainWindow *ui;
    Manager manager;
    QTimer resizeTimer;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    void setUp();
    void draw();
    void clearUi(QLayout &qGrid);
private slots:
    void changeDirectory();
    void resizeDone();
};
#endif // MAINWINDOW_H
