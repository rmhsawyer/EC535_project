#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <gui.h>


// Main function to create GUI 
int main(int argc, char *argv[])
{
    // Create an application of type gui
    QApplication app(argc, argv);
    gui window;
    window.showFullScreen();
    window.show();

    return app.exec();
}

