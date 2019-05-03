#include <gui.h>

char rpeople[4];
char rmode[4];
int uiplay = 1;

// Constructor to generate the GUI
gui::gui(QWidget *parent) : QWidget(parent)
{
	// Create the songs list widget
	songs = new QListWidget(this);

	// Search the directory for songs
	dirName = "/media/card/project/mp3s/";
	dir.setPath(dirName);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::Name);
	songList = new QStringList();
	*songList = dir.entryList();

	// Add all the song names into the songs list widget
	int songNum = songList->size();
	for(int i = 0; i < songNum; i++)
	{
		QListWidgetItem *item = new QListWidgetItem();
		item->setText((*songList)[i]);
		qDebug() << (*songList)[i];
		songs->insertItem(i,item);
	}

	// Create a new grid layout for widgets to be added to
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timer_update()));
	timer->start(1000);


	layout = new QGridLayout(this);

	// Create play button and connect it to a button handler
	play = new QPushButton("Play", this);
	connect(play, SIGNAL(clicked()), this, SLOT(play_handler()));

	// Create start button and connect it to a button handler
	start = new QPushButton("Start", this);
	connect(start, SIGNAL(clicked()), this, SLOT(start_handler()));

	// Create stop button and connect it to a button handler
	change = new QPushButton("Change", this);
	connect(change, SIGNAL(clicked()), this, SLOT(change_handler()));

	reset = new QPushButton("Reset", this);
	connect(reset, SIGNAL(clicked()), this, SLOT(reset_handler()));

	cmode = new QLineEdit;
	cmode->setText("0");

	cpeople = new QLineEdit;
	cpeople->setText("0");

	nowplaying = new QLineEdit;
	nowplaying->setText("");
	
	uiplay = 1;


	layout->addWidget(songs, 0, 0, -1, 1);//
	layout->addWidget(start, 0, 1);
	layout->addWidget(nowplaying, 1, 1);
	layout->addWidget(play, 2, 1);
	layout->addWidget(cpeople, 3, 1);
	layout->addWidget(reset, 4, 1);
	layout->addWidget(cmode, 5, 1);	
	layout->addWidget(change, 6, 1);
	this->setLayout(layout);

	state = 0;
	mode = 0;
}



void gui::timer_update()
{
	char line[256];
	char *token;
	char delim[] = "\n";

	FILE * pFile;
	pFile = fopen("/dev/mygpio","r+");
	if (pFile==NULL) {
		fputs("mygpio module isn't loaded\n",stderr);
		return;
	}
	fread(line,sizeof(char),255,pFile);

	int i = 0;

	//qDebug() << "In read\n";
	for(token = strtok(line, delim); token != NULL; token = strtok(NULL, delim)) {
		i++;//this is the line number
		if(i == 2){
			strcpy(rpeople, token);	
			cpeople->setText(rpeople);	
		}else if(i == 4){
			strcpy(rmode, token);
			cmode->setText(rmode);
		}
	}
	if((uiplay == 1) && strcmp(rpeople, "0")){
		//qDebug() << rpeople;
		if(state == 0)
			start_handler();
		else if(state == 2)
			play_handler();
	}
	
	if(!strcmp(rpeople, "0") && (state == 1)){
		play_handler();
		uiplay = 1;
	}
  	return;
}


// change button handler
void gui::change_handler()
{
	mode += 1;
	mode %= 5;
	QString mst[5] = {"0", "1", "2", "3", "4"};
	qDebug() << mode;
	cmode->setText(mst[mode]);
	QString command = "echo m";
	QString command2 = " > /dev/mygpio";
	command += mst[mode];
	command += command2;

	qDebug() << command;
	QByteArray ba = command.toLocal8Bit();
  	const char *c = ba.data();
	system(c);
}


void gui::reset_handler()
{
	QString command = "echo r";
	QString command2 = " > /dev/mygpio";
	command += command2;

	qDebug() << command;
	QByteArray ba = command.toLocal8Bit();
  	const char *c = ba.data();
	system(c);
}


//state == 0: no song is playing.
//state == 1: a song is playing.
//state == 2: a song is paused.
// Play button handler
void gui::play_handler()
{
	// If  a song is paused, play it and set state to playing
	if(state == 2 || state == 0)
	{
		int status = system("ash /media/card/project/scripts/music_control.sh -g");
		state = 1;
		uiplay = 1;
		play->setText("pause");
	}else if(state == 1){
		int status = system("ash /media/card/project/scripts/music_control.sh -p");
		state = 2;
		uiplay = 0;
		play->setText("Play");
	}
}

// Pause button handler
void gui::pause_handler()
{
	// If a song is playing, pause it and set state to paused
	if(state == 1)
	{
		int status = system("ash /media/card/project/scripts/music_control.sh -p");
		state = 2;
	}
}

// Start button handler
void gui::start_handler()
{
	// If there is no song playing, start a song and set the state to playing
	if(state==0 || state==2)
	{
		std::string file = "ash /media/card/project/scripts/music_control.sh -s /media/card/project/mp3s/" + songs->currentItem()->text().toStdString();
		int status = system(file.c_str());
		state = 1;
		play->setText("pause");
		nowplaying->setText(songs->currentItem()->text());
	}
	// If there is a song playing, stop it first, then start the new one
	else
	{
		stop_handler();
		start_handler();
	}
}

// Stop button handler
void gui::stop_handler()
{
	// Then if there is a song playing, stop it and set state to stopped
	if(state!=0)
	{
		int status = system("ash /media/card/project/scripts/music_control.sh -k");
		state = 0;
	}
}
