#include <QApplication>
#include <QTime>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    int currentExitCode = 0;

    // J'initialise mes fenetres :
    do {
        QApplication a(argc, argv);
        MainWindow w;
        w.setWindowState(Qt::WindowFullScreen);
        w.show();
        currentExitCode = a.exec();
    } while( currentExitCode == MainWindow::EXIT_CODE_REBOOT ); // tant que le signal de reboot est emit on ouvre des QApplication, sinon on ferme

    return currentExitCode;
}

