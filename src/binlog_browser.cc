#include <ostream>
#include <QtGui>
#include "binlog_browser.h"
#include "binlog_api.h"



Eventitem::Eventitem(QListWidget * parent,
                     QString text, QWidget *wid):QListWidgetItem(parent)
{
  this->str= text;
  this->parent_widget= wid;
  this->parent_list= parent;
}

void
Eventitem::show_Event()
{
  static_cast<BinlogBrowser*>(parent_widget)->eventText->setText(str);
}

void
BinlogBrowser::show_startup_message()
{
  /* BASIC startup message */
  log_message(MESSAGE, "Welcome to MySQL Binlog Browser!");
  log_message(MESSAGE, "Version 1.0");
  log_message(MESSAGE, "Author: Rohit Kalhans <rohit.kalhans@oracle.com>");
  log_message(WARNING, "This software is in alpha phase.");
}

BinlogBrowser::BinlogBrowser(QWidget * parent)
{
  setupUi(this); // this sets up GUI
  /* Signal and slots connections. Add more here */
  connect(startMining, SIGNAL(clicked()), this, SLOT(start_mining()));
  connect(stopMining, SIGNAL(clicked()), this, SLOT(stop_mining()));
  connect(eventsList, SIGNAL(itemSelectionChanged ()), this,
              SLOT(show_event_details()));
  connect(this, SIGNAL(signal_clear_eventslist()), this, SLOT(clear_events_list()));
  connect(this, SIGNAL(signal_log_message(ENUM_MESSAGE_TYPE, QString)),
          this, SLOT(log_message(ENUM_MESSAGE_TYPE, QString)));
  connect(this, SIGNAL(signal_enable_stopMining()), this, SLOT(enable_stopMining()));
  connect(this, SIGNAL(signal_add_list_item(std::string, std::string)),
          this, SLOT(add_list_item(std::string, std::string)));
  connect(this, SIGNAL(signal_cleanup()), this, SLOT(cleanup()));
  started_listening= false;
  stop_listening= false;
  show_startup_message();
}

void
BinlogBrowser::show_event_details()
{
  QListWidgetItem * item=
         (QListWidgetItem *)(eventsList->selectedItems()).takeFirst();
  static_cast<Eventitem *>(item)->show_Event();
}
void
BinlogBrowser::start_mining()
{
  FilepathName->setDisabled(true);
  startMining->setDisabled(true);
  pthread_create(&listener, NULL, listener_entry, (void*)this);
}

void
BinlogBrowser::log_message(ENUM_MESSAGE_TYPE level, QString line)
{
  QTextCursor cursor = console->textCursor();
  QString errorHtml = "<font color=\"Red\">ERROR: ";
  QString messageHtml = "<font color=\"Dark green\">INFO :  ";
  QString warningHtml = "<font color=\"Deep Pink\">WARN : ";
  QString endHtml = "</font><br />";

  switch(level)
  {
    case MESSAGE: line = messageHtml % line; break;
    case WARNING: line = warningHtml% line; break;
    case ERROR: line = errorHtml % line; break;
    default:;
  }

  line = line % endHtml;
  console->insertHtml(line);
  cursor.movePosition(QTextCursor::End);
  console->setTextCursor(cursor);
}


void
BinlogBrowser::stop_mining()
{
  if (started_listening)
    stop_listening= true;
  log_message(MESSAGE, "Disconnecting on user request.");
  pthread_cancel(listener);
  cleanup();
}


void
BinlogBrowser::cleanup()
{
  FilepathName->setDisabled(false);
  startMining->setDisabled(false);
  started_listening= false;
  stop_listening= false;
  stopMining->setDisabled(true);
  log_message(MESSAGE, "Disconnected!");
}

void
BinlogBrowser::clear_events_list()
{
  this->eventsList->clear();
}


void
BinlogBrowser:: enable_stopMining()
{
  stopMining->setDisabled(false);
}

void
BinlogBrowser::add_list_item(std::string type_str, std::string info_str)
{
  Eventitem* Listelement= new Eventitem(eventsList,
                                        QString::fromUtf8(info_str.c_str()),
                                        this);
  Listelement->setText(QApplication::
                       translate("MySQLBinlogBrowser",
                       type_str.c_str(), 0,
                       QApplication::UnicodeUTF8));
  Listelement->show_Event();
  Listelement->setSelected(true);
  eventsList->scrollToBottom();
}




using mysql::Binary_log;
using mysql::system::create_transport;
using mysql::system::get_event_type_str;
using namespace std;

void*
BinlogBrowser::listen_and_update(void * widget)
{
  BinlogBrowser *browser= this;
  Binary_log_event *event;
  int result, error;
  QString url= browser->FilepathName->toPlainText();
  QByteArray ba = url.toLocal8Bit();
  const char *c_str = ba.data();
  /* Clean up the existing list elements */
  emit browser->signal_clear_eventslist();

  QString connectmessage="Trying to connect to ";
  connectmessage= connectmessage%url;
  emit browser->signal_log_message(MESSAGE,connectmessage);
  Binary_log binlog(create_transport(c_str));
  std::ostringstream info;
  QString info_str;
  if ((error= binlog.connect()))
  {
    emit browser->signal_log_message(ERROR,
                                "Unable to connect. Check if your URL is valid!");
    emit browser->signal_log_message(ERROR, (QString)bapi_error_messages[error]);
    goto err;
  }
  else
  {
    emit browser->signal_log_message(MESSAGE,"Connected!");
  }
  started_listening= true;
  emit browser->signal_enable_stopMining();
  while (!stop_listening)
  {
    info.str(std::string());
    result = binlog.wait_for_next_event(&event);
    if (result != ERR_OK)
      break;
    event->print_long_info(info);
    emit browser->signal_add_list_item(get_event_type_str(event->get_event_type()),
                                       info.str());
  }
err:
  emit browser->signal_cleanup();
  return NULL;
}


void* listener_entry(void* widget)
{
  BinlogBrowser* bb= (BinlogBrowser*)widget;
  bb->listen_and_update(widget);
}
