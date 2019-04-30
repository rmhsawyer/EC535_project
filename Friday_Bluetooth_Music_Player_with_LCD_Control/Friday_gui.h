#ifndef FRIDAY_GUI_H
#define FRIDAY_GUI_H

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


class Friday_gui;

class Friday_gui : public QWidget
{
	Q_OBJECT	

public:
	Friday_gui(QWidget *parent = 0);

private slots:
	void play_handler();
	void pause_handler();
	void start_handler();
	void stop_handler();
	void next_handler();
	void prev_handler();
	void change_handler();
	void reset_handler();
	void timer_update();

private:
	QTImer* timer;
	QGridLayout* layout;
	QListWidget* songs;
	QStringList* songList;
	QPushButton* play;
	QPushButton* start;
	QPushButton* next;
	QPushButton* prev;
	QPushButton* change;
	QPushButton* reset;
	QLineEdit* cmode;
	QLineEdit* cpeople;
	QLineEdit* nowplaying;

	// Variables for song name retrieval and script calls
	int state, mode, csong;
	QString dirName;
	QDir dir;
};

#endif