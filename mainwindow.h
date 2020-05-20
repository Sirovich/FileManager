#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include "manager.h"
#include <QRect>
#include "dialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
    Ui::MainWindow *ui;
    Dialog* dialogWindow;
    Manager manager;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
private:
    void createDirectory();
    void setUp();
    void draw();
    void clearUi(QLayout &qGrid);
private slots:
    void changeDirectory();
    void displayError(std::string errorMessage);
    void on_downloadsButton_clicked();
    void on_desktopButton_clicked();
    void on_homeButton_clicked();
};
#endif // MAINWINDOW_H
