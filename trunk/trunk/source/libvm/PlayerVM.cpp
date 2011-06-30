#include "PlayerVM.hpp"
#include <iostream>
namespace vm
{
  PlayerVM *PlayerVM::_instance = 0;

  PlayerVM::PlayerVM() :
    _iplImg(0),
    _record(0)
  {
    _isStop  = true;
    _isPaused  = false;
    _record = 0;
  }
  PlayerVM::~PlayerVM()
  {
    delete _iplImg;
  }

  PlayerVM* PlayerVM::GetInstance()
  {
    if (!_instance)
      _instance = new PlayerVM;
    return _instance;
  }

  void PlayerVM::DelInstance()
  {
    if (_instance)
      delete _instance;
  }

  void PlayerVM::mapSurface(QLabel *surface)
  {
    _surface = surface;// no delete
  }

  QImage PlayerVM::ConvertIplImgtoQBitmpat(IplImage* img)
  {
    return QImage((const uchar *)img->imageData, img->width, img->height, QImage::Format_RGB888).rgbSwapped();
  }


  void PlayerVM::SliderChangeValue()
  {
    IplImage* cloneImg = cvCloneImage(_iplImg);
    int p = 0;//ui->tbSlider->value();
    int w = cloneImg->width;
    int h = cloneImg->height;
    for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
      {
        uchar* temp_ptr = &((uchar*)(cloneImg->imageData + cloneImg->widthStep*i))[j*3];
        int nVal =temp_ptr[0] + p;
        if (nVal < 0)
          nVal = 0;
        if (nVal > 255)
          nVal = 255;
        temp_ptr[0] = nVal;
        nVal =temp_ptr[1] + p;
        if (nVal < 0)
          nVal = 0;
        if (nVal > 255)
          nVal = 255;
        temp_ptr[1] = nVal;
        nVal =temp_ptr[2] + p;
        if (nVal < 0)
          nVal = 0;
        if (nVal > 255)
          nVal = 255;
        temp_ptr[2] = nVal;

      }
    }
  }

  void PlayerVM::Play()
  {
    if (!_record)
    {
      _record = new MediaFile(_recordFilepath, false, Video);
      _record->Start();
      _isStop = false;
    }
    std::cout << "Start plading" << std::endl;
    
      while(!_isStop)
      {
        if (_isStop)
          break;

        if (!_isPaused) 
        {
          QImage * nFrame = _record->GetNextVideoFrame();
          if (nFrame->isNull())
          {
            std::cout << "quit playing" << std::endl;
            _isStop = true;
            return ;
          }
          nFrame->scaled(_surface->size());
          _surface->setPixmap(QPixmap::fromImage(*nFrame).scaled(_surface->size()));
          delete nFrame;
          nFrame = 0;
          usleep(100000);
          QApplication::processEvents();
          QCoreApplication::sendPostedEvents(NULL, 0);
          //cvWaitKey(40);
        }
        else
          cvWaitKey(500);
      }
  }

  void PlayerVM::Stop()
  {
    if (!_isStop)
    {
      _isStop = true;
      _surface->clear();
    }

    std::cout << "Stop plaing" << std::endl;
    if (_record)
    {
      _record->Stop();
      // save
      delete _record;
      _record = 0;
    }
  }

  void PlayerVM::Pause()
  {
    std::cout << "Pause plaing" << std::endl;
    if (!_isPaused)
    {
      _isPaused = true;
    }

  }

  void PlayerVM::setFilepath(QString const & filepath)
  {
    _recordFilepath = filepath;
  }
}
