#ifndef __CAMERAVM_HPP__
# define __CAMERAVM_HPP__

# include <QObject>
# include <QString>
# include <QImage>
# include <QLabel>
# include <QDir>

#include <cv.h>
#include <cv.hpp>
#include <cvaux.h>
#include <highgui.h>
#include <queue>

#include "app/MediaFile.hpp"

namespace vm
{
  class CameraVM : public QObject
  {
    //TODO uncomment if slot or signals
    //Q_OBJECT

 public:
    static CameraVM* GetInstance();
    static void DelInstance();

    void mapSurface(QLabel* surface);

    void StartCam();
    void StopCam();
    void PauseCam();

    void SliderChangeValue();

    void StartRecordCam();
    void StopRecCam();
    void PauseRecCam();

  protected:

  private:
    static CameraVM* _instance;

    CameraVM(); 
    ~CameraVM();

    IplImage* _iplImg;
    int _cameras;
    bool _isStop;
    bool _isPaused;
    bool _isRecording;
    bool _isPauseRecording;
    std::queue<std::pair<int, uint8_t*> >   _cachedEncFrames;
    QLabel * _surface;

    QImage ConvertIplImgtoQBitmpat(IplImage* img);

    MediaFile* _record;
  };
}
#endif // !__CAMERAVM_HPP__