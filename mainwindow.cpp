#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = Manager();
    setUp();
    draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUp()
{
    connect(&manager, &Manager::changeUi, this, &MainWindow::changeDirectory);
    resizeTimer.setSingleShot( true );
    connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()) );
    connect( ui->backButton, &QPushButton::released, &manager, &Manager::turnBack);
    manager.getAllObjects();
}

void MainWindow::draw()
{
    clearUi(*ui->gridLayout);
    int maxCount = MainWindow::width()/130;
    ui->gridLayout->setSpacing(20);
    ui->gridLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    int i = 0;
    int j = 0;
    for(Directory* directory: manager.getDirectories())
    {
        QVBoxLayout* layout = new QVBoxLayout();
        directory->setScaledContents(true);
        directory->setStyleSheet("border-image: url(DirectoryIcon.png);");
        directory->setFixedSize(50, 50);
        layout->addWidget(directory, 0, Qt::AlignCenter);

        QLabel* text = new QLabel(directory->getName().c_str());
        layout->addWidget(text, 0, Qt::AlignCenter);

        ui->gridLayout->addLayout(layout, i, j);
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
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignCenter);
        file->setScaledContents(true);
        file->setStyleSheet("border-image: url(FileIcon.png);");
        file->setFixedSize(50, 50);
        layout->addWidget(file, 0, Qt::AlignCenter);

        QLabel* text = new QLabel(file->getName().c_str());
        layout->addWidget(text, 0, Qt::AlignCenter);
        ui->gridLayout->addLayout(layout, i, j);
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
        }
        else if (QLayout *pQLayout = pQLItem->layout())
        {
          clearUi(*pQLayout);
          delete pQLayout;
        }
    }
}

void MainWindow::changeDirectory()
{
    manager.getAllObjects();
    draw();
}

void MainWindow::resizeEvent( QResizeEvent *e )
{
  resizeTimer.start( 200 );
  QMainWindow::resizeEvent(e);
}

void MainWindow::resizeDone()
{
    ui->verticalScrollBar->move(MainWindow::width() - 20, 20);
    ui->gridLayoutWidget->setGeometry(QRect(QPoint(120, 20), QSize(MainWindow::width()- 149, MainWindow::height() - 99)));
}
