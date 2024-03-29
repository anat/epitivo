#ifndef __MAINWINDOWVM_HPP__
# define __MAINWINDOWVM_HPP__

# include <QObject>
# include <QString>

namespace vm
{
  class MainWindowVM : public QObject
  {
    //Q_OBJECT

    public:
    static MainWindowVM* GetInstance();
    static void DelInstance();

    public slots:
   	bool openFile(QString const & filename);

    signals:

    private:
      MainWindowVM();
      ~MainWindowVM();
    static MainWindowVM* _instance;
  };
}
#endif // !__MAINWINDOWVM_HPP__
