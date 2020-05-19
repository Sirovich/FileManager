#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = Manager();
    dialogWindow = new Dialog(this);
    setUp();
    draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
        manager.copySelected();
    }

    if (event->matches(QKeySequence::Paste))
    {
        manager.insertFiles();
    }

    if (event->matches(QKeySequence::Cut))
    {
        manager.cutFiles();
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* menu = new QMenu();
    QAction* createAction = new QAction("Create directory", this);
    QAction* copyAction = new QAction("Copy selected", this);
    QAction* cutAction = new QAction("Cut", this);
    QAction* insertAction = new QAction("Insert", this);

    connect(insertAction, &QAction::triggered, &manager, &Manager::insertFiles);
    connect(copyAction, &QAction::triggered, &manager, &Manager::copySelected);
    connect(cutAction, &QAction::triggered, &manager, &Manager::cutFiles);
    connect(createAction, &QAction::triggered, this, &MainWindow::createDirectory);
    menu->addAction(createAction);
    menu->addAction(copyAction);
    menu->addAction(cutAction);
    menu->addAction(insertAction);
    menu->exec(QCursor::pos());
}

void MainWindow::createDirectory()
{
    manager.createDirectory();
}

void MainWindow::setUp()
{
    connect(&manager, &Manager::displayError, this, &MainWindow::displayError);
    connect(&manager, &Manager::changeUi, this, &MainWindow::changeDirectory);
    connect( ui->backButton, &QPushButton::released, &manager, &Manager::turnBack);
    manager.getAllObjects();
}

void MainWindow::draw()
{
    clearUi(*ui->gridLayout);
    int maxCount = ui->scrollArea->width()/150;
    int i = 0;
    int j = 0;
    for(Directory* directory: manager.getDirectories())
    {
        QWidget* widget = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        directory->setScaledContents(true);
        QPixmap pix ("DirectoryIcon.png");
        directory->setScaledContents(true);
        directory->setPixmap(pix);
        directory->setFixedSize(50, 50);
        layout->addWidget(directory, 0, Qt::AlignCenter | Qt::AlignBottom);

        QLabel* text = new QLabel(directory->getName().c_str());
        text->setAlignment(Qt::AlignCenter);
        text->setWordWrap(true);
        layout->addWidget(text, 0, Qt::AlignTop);
        widget->setLayout(layout);
        ((QGridLayout*)ui->scrollAreaWidgetContents->layout())->addWidget(widget, i, j);
        if (j > maxCount)
        {
            j = 0;
            i++;
        }
        else
        {
            j++;
        }
    }

    for(File* file: manager.getFiles())
    {
        QWidget* widget = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignCenter);
        QPixmap pix ("FileIcon.png");
        file->setScaledContents(true);
        file->setPixmap(pix);
        file->setFixedSize(50, 50);
        layout->addWidget(file, 0, Qt::AlignCenter | Qt::AlignBottom);

        QLabel* text = new QLabel(file->getName().c_str());
        text->setAlignment(Qt::AlignCenter);
        text->setWordWrap(true);
        layout->addWidget(text, 0, Qt::AlignTop | Qt::AlignCenter);
        widget->setLayout(layout);
        ((QGridLayout*)ui->scrollAreaWidgetContents->layout())->addWidget(widget, i, j);
        if (j > maxCount)
        {
            j = 0;
            i++;
        }
        else
        {
            j++;
        }
    }
}

void MainWindow::clearUi(QLayout &qGrid)
{
    for (int i = qGrid.count(); i--;)
    {
        QLayoutItem *pQLItem = qGrid.takeAt(i);
        if (QWidget *pQWidget = pQLItem->widget())
        {
          delete pQWidget;
          delete pQLItem;
          pQWidget = nullptr;
          pQLItem = nullptr;
        }
        else if (QLayout *pQLayout = pQLItem->layout())
        {
          clearUi(*pQLayout);
          delete pQLayout;
          pQLayout = nullptr;
        }
    }
}

void MainWindow::changeDirectory()
{
    manager.getAllObjects();
    draw();
}

void MainWindow::displayError(std::string errorMessage)
{
    dialogWindow->setText(errorMessage);
    dialogWindow->show();
}
