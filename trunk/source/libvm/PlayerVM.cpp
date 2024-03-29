#include "PlayerVM.hpp"
#include "../libview/playlistwidget.hpp"
#include <stdint.h>
#include <iostream>
namespace vm
{
  PlayerVM *PlayerVM::_instance = 0;

  PlayerVM::PlayerVM() :
    _iplImg(0),
    _record(0),
    _codec(0)
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

  void PlayerVM::Play(int followPlayList = 0)
  {
    int tmpCount;
    uint8_t tmpSwap;

    if (!_record)
    {
      _record = new MediaFile(_recordFilepath, false, Video);
      _record->Start();
      _isStop = false;
      _codec = new VideoCodec(640, 480, COLOR_BGR);
    }
    std::cout << "Start playing" << std::endl;

    uint8_t * buff2  =  new uint8_t[640 * 480 * 3];
    //printf("play.buff2 = %p\n", buff2);
    while(!_isStop)
    {
      if (_isStop)
        break;

      if (!_isPaused) 
      {
        uint8_t * buff  = _record->GetNextVideoFrame();
	//printf("play.buff = %p\n", buff);
        if (!buff)
        {
          std::cout << "quit playing -> no buff" << std::endl;
          _isStop = true;
          this->Stop();
          if (followPlayList)
            PlayListWidget::GetInstance()->PlayNext();
          return ;
        }

        _codec->setResultBuff(buff);
        _codec->decode(buff2);

	for (tmpCount = 0; tmpCount < 640*480*3; tmpCount += 3)
	  {
	    tmpSwap = buff2[tmpCount+2];
	    buff2[tmpCount+2] = buff2[tmpCount];
	    buff2[tmpCount] = tmpSwap;
	  }
	
        QImage nFrame(buff2, 640, 480, QImage::Format_RGB888 );
        if (nFrame.isNull())
        {
          std::cout << "quit playing -> no frame" << std::endl;
          _isStop = true;
          this->Stop();
          if (followPlayList)
              PlayListWidget::GetInstance()->PlayNext();
          return ;
        }
        nFrame.scaled(_surface->size());
        _surface->setPixmap(QPixmap::fromImage(nFrame).scaled(_surface->size()));

        QApplication::processEvents();
         QCoreApplication::sendPostedEvents();
        usleep(100000);
        QApplication::processEvents();
         QCoreApplication::sendPostedEvents();
        //QCoreApplication::sendPostedEvents(NULL, 0);
      }
      else
      {
          QApplication::processEvents();
           QCoreApplication::sendPostedEvents();
        cvWaitKey(500);
        QApplication::processEvents();
         QCoreApplication::sendPostedEvents();
        }
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

