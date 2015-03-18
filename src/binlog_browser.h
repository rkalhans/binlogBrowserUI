#ifndef B_BROWSER_H
#define B_BROWSER_H

#include "ui_MySQLbinlogbrowser.h"
#include <QtGui>
#include <QtCore>
enum ENUM_MESSAGE_TYPE
{
  MESSAGE,
  WARNING,
  ERROR
};
Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(ENUM_MESSAGE_TYPE);

class BinlogBrowser: public QWidget, public Ui::MySQLBinlogBrowser
{
  Q_OBJECT
private:
  pthread_t listener;
  bool started_listening;
  bool stop_listening;

protected:
  void show_startup_message();
public:
  BinlogBrowser(QWidget  *parent = 0);
  void* listen_and_update(void * widget);

/*SLOTS*/
public slots:
  void log_message(ENUM_MESSAGE_TYPE level, QString message);
  void start_mining();
  void stop_mining();
  void show_event_details();
  void clear_events_list();
  void enable_stopMining();
  void add_list_item(std::string type_str, std::string info_str);
  void cleanup();

/*SIGNALS */
signals:
  void signal_clear_eventslist();
  void signal_log_message(ENUM_MESSAGE_TYPE level, QString message);
  void signal_enable_stopMining();
  void signal_add_list_item(std::string type_str, std::string info_str);
  void signal_cleanup();
};

class Eventitem: private QObject, public QListWidgetItem
{
  Q_OBJECT
private:
  QString str;
  QWidget* parent_widget;
  QListWidget* parent_list;

public:
  Eventitem(QListWidget*, QString, QWidget*);
  void show_Event();
};

void* listener_entry(void *widget);
#endif /* B_BROWSER_H */
