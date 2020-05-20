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
    manager.showCreateDirectoryWindow();
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
    std::string displayedPath;
    if (manager.getCurrentDirectory().size() > MainWindow::width()/10)
    {
        displayedPath = manager.getCurrentDirectory().substr(0, MainWindow::width()/10) + "...";
    }
    else
    {
        displayedPath = manager.getCurrentDirectory();
    }

    ui->path->setText(displayedPath.c_str());
    for(Directory* directory: manager.getDirectories())
    {
        QWidget* widget = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
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

void MainWindow::on_downloadsButton_clicked()
{
    char username[256];
    size_t size = 256;
    getlogin_r(username, size);
    std::string fullPath = "/home/";
    fullPath += username;
    fullPath += "/Downloads/";
    manager.setDirectory(fullPath);
}

void MainWindow::on_desktopButton_clicked()
{
    char username[256];
    size_t size = 256;
    getlogin_r(username, size);
    std::string fullPath = "/home/";
    fullPath += username;
    fullPath += "/Desktop/";
    manager.setDirectory(fullPath);
}

void MainWindow::on_homeButton_clicked()
{
    char username[256];
    size_t size = 256;
    getlogin_r(username, size);
    std::string fullPath = "/home/";
    fullPath += username;
    fullPath += "/";
    manager.setDirectory(fullPath);
}
