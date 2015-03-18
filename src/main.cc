#include "binlog_browser.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  /*
     register the meta types of std:string and ENUM_MESSAGE_TYPE
     Since we will use it in SLOTS
  */
  qRegisterMetaType<std::string>("std::string");
  qRegisterMetaType<ENUM_MESSAGE_TYPE>("ENUM_MESSAGE_TYPE");
  BinlogBrowser *dialog = new BinlogBrowser;
  dialog->show();
  return app.exec();
}


