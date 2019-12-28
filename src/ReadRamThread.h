#ifndef FL_READRAMTHREAD_H_
#define FL_READRAMTHREAD_H_
#include <QThread>

class ReadRamThread:public QThread
{
Q_OBJECT public:
  ReadRamThread (){}
  virtual void run ();
  bool end;
  QString filename;
  int cMode;
  public slots:void canceled (void);

signals:
  void set_progress (int ile, int max);
  void error (int err);
};

#endif
