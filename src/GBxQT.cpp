#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QSettings>
#include <QTimer>
#include <QTextCodec>
#include <QtWidgets/QMessageBox>
#include "Gui.h"
#include "const.h"
#include "Settings.h"
#include "Console.h"
#include <stdio.h>
#include <iostream>
#include <QStyleFactory>
#include <QOperatingSystemVersion>
#include <QFile>

void
parse_params (int /*argc*/, char *argv[])
{
  while (*++argv)
    {
 // Appearance settings
      if (strcmp (*argv, "-darkmode") == 0){
          Settings::darkmode = true;
      }

      if (strcmp (*argv, "-noautotheme") == 0){
          Settings::noautotheme = true;
      }

/*
 * communication threads priority is used when communication
 * or gui is freezing
 */
      if ((*argv)[0] == '-' && isdigit ((*argv)[1]) && (*argv)[1] >= '0'
      && (*argv)[1] <= '7')
    {
      switch ((*argv)[1] - '0')
        {
        case 0:
          Settings::priority = QThread::IdlePriority;
          break;
        case 1:
          Settings::priority = QThread::LowestPriority;
          break;
        case 2:
          Settings::priority = QThread::LowPriority;
          break;
        case 3:
          Settings::priority = QThread::NormalPriority;
          break;
        case 4:
          Settings::priority = QThread::HighPriority;
          break;
        case 5:
          Settings::priority = QThread::HighestPriority;
          break;
        case 6:
          Settings::priority = QThread::TimeCriticalPriority;
          break;
        case 7:
          Settings::priority = QThread::InheritPriority;
          break;
        }

    }
    }
}

int
main (int argc, char *argv[])
{
  parse_params (argc, argv);
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app (argc, argv);
#ifdef Q_OS_WIN
  app.setStyle(QStyleFactory::create("Fusion"));
  if(QOperatingSystemVersion::current() >= QOperatingSystemVersion(QOperatingSystemVersion::Windows10) && Settings::noautotheme != true){
      QSettings GetLightMode("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
      if(GetLightMode.value("AppsUseLightTheme").toString() == "0"){
          Settings::darkmode = true;
      }
  }

  if (Settings::darkmode == true)
  {
      QFile f(":qdarkstyle/style.qss");
      if (!f.exists())
      {
          qDebug() << "Unable to set stylesheet, file not found\n";
      }
      else
      {
          f.open(QFile::ReadOnly | QFile::Text);
          QTextStream ts(&f);
          qApp->setStyleSheet(ts.readAll());
      }
  }

#endif
  Gui window;
  QObject::connect(&app, SIGNAL(aboutToQuit()), &window, SLOT(exit()));
  window.show ();
  window.startup_info();
  return app.exec ();
}
