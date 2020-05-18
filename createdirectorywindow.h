#ifndef CREATEDIRECTORYWINDOW_H
#define CREATEDIRECTORYWINDOW_H
#include <QDialog>
namespace Ui {
class CreateDirectoryWindow;
}

class CreateDirectoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDirectoryWindow(QWidget *parent = nullptr);
    ~CreateDirectoryWindow();
signals:
    void editingFinished(std::string newName);
private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateDirectoryWindow *ui;
};

#endif // CREATEDIRECTORYWINDOW_H
