#ifndef RENAME_H
#define RENAME_H

#include <QDialog>

namespace Ui {
class Rename;
}

class Rename : public QDialog
{
    Q_OBJECT

public:
    explicit Rename(QWidget *parent = nullptr);
    ~Rename();
    void setText(std::string text);
signals:
    void editingFinished(QString newName);
private slots:
    void textChanged(const QString& text);
    void on_pushButton_clicked();
private:
    Ui::Rename *ui;
};

#endif // RENAME_H
