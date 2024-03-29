#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vm(vm::MainWindowVM::GetInstance()),
    _centralView(CentralView::GetInstance())
{
  ui->setupUi(this);
  
  this->setCentralWidget(_centralView);

  this->setMainWindowConnects();
  this->setMenuConnects();
}

MainWindow::~MainWindow()
{
  this->clearCentralView();
  delete ui;
}

void MainWindow::setMenuConnects()
{
  connect(ui->actionOpen_file, SIGNAL(triggered()), _centralView, SLOT(OpenFile()));
  connect(ui->actionOpen_device, SIGNAL(triggered()), _centralView, SLOT(OpenCameraDevice()));
  connect(ui->actionPlaylist, SIGNAL(triggered()), _centralView, SLOT(OpenPlayList()));
}

void MainWindow::setMainWindowConnects()
{
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(appExit()));
}

void MainWindow::appExit()
{
  this->clearCentralView();
  qApp->quit();
}

CentralView* MainWindow::createCentralView()
{
  if (_centralView == 0)
    {
      _centralView = CentralView::GetInstance();
  }
  return _centralView;
}

void MainWindow::clearCentralView()
{
  if (_centralView != 0)
  {
    delete _centralView;
    _centralView = 0;
  }
}

