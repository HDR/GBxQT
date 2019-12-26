#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QString>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QWinTaskbarButton>
#include "Settings.h"
#include "Console.h"

class Gui:public QWidget
{
  Q_OBJECT Settings * settings;
  Console *console;
  QGridLayout *grid;
  QVBoxLayout *left;
  QVBoxLayout *right;
  QVBoxLayout *center;
  QHBoxLayout *down;
  QProgressBar *progress;
  QWinTaskbarButton *winTaskbar;
  QLabel *image;
  QPixmap *logo;
  QPushButton *cancel_btn;
  QPushButton *status_btn;
  QPushButton *rflash_btn;
  QPushButton *wflash_btn;
  QPushButton *rram_btn;
  QPushButton *wram_btn;
  QPushButton *eflash_btn;
  QPushButton *eram_btn;
  QString file_name;
  QString path;

public:
    Gui (QWidget * parent = nullptr);
  static int port_type;


  public slots:void startup_info (void);
  void setEnabledButtons (bool stan);
  void setRamButtons ();
  void write_flash (void);
  void read_flash (void);
  void read_ram (void);
  void write_ram (void);
  void show_info (void);
  void erase_flash (void);
  void erase_ram (void);
  void setProgress (int ile, int max);
};
