#ifndef CAMERAVIEW_HPP
#define CAMERAVIEW_HPP

#include <set>
#include <QWidget>
#include <QImage>
#include <QDialog>
#include <QComboBox>
#include <stdlib.h>
#include <QPushButton>
#include <QApplication>
#include "libvm/CameraVM.hpp"
#include "DeviceSelectView.hpp"

namespace Ui {
    class CameraView;
}

class CameraView : public QWidget
{
  Q_OBJECT

  public:
    explicit CameraView(QWidget *parent = 0);
    ~CameraView();

    public slots:
    void StartCam();
    void StopCam();

    void recordCam();
    void stopRecCam();
    void pauseRecCam();

  private:
    bool _isRecording;
    bool _isCamActive;
    Ui::CameraView *ui;
    vm::CameraVM* _vm;


    QString _selected;

    DeviceSelectView*   _deviceSelectView;
  DeviceSelectView* createDeviceSelectView();
  void clearDeviceSelectView();

    void selectInputDevice();
};

#endif // CAMERAVIEW_HPP
