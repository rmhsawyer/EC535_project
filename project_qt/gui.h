#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>
#include <stdlib.h>
#include <QDir>
#include <QString>
#include <QStringList>
#include <string>
#include <cstring>

// gui class - generates a user interface on the gumstix touch screen
class gui;

class gui : public QWidget
{
	Q_OBJECT

public:
	gui(QWidget *parent = 0);

// Declare button handlers
private slots:
	void play_handler();
	void pause_handler();
	void start_handler();
	void stop_handler();
	void change_handler();
	void reset_handler();
	void timer_update();

private:
	// Declare widgets to be displayed
	QTimer* timer;
	QGridLayout* layout;
	QListWidget* songs;
	QStringList* songList;
	QPushButton* play;
	QPushButton* pause;
	QPushButton* start;
	QPushButton* stop;
	QPushButton* change;
	QPushButton* reset;
	QLineEdit* cmode;
	QLineEdit* cpeople;
	QLineEdit* nowplaying;
	

	// Variables for song name retrieval and script calls
	int state, mode;
	QString dirName;
	QDir dir;

};



#endif
