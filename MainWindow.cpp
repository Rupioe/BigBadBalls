#include <QHBoxLayout>
#include "MainWindow.h"

#include <QApplication>
#include <QUrl>


int const MainWindow::EXIT_CODE_REBOOT = -42; // sucre syntaxique

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->mainScene = new MainScene();
    mainView = new QGraphicsView();
    mainView->setScene(mainScene);
    mainView->scale(0.2, 0.2);
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect( mainScene, SIGNAL (triggered()),this, SLOT (slotReboot()));
    connect( mainScene, SIGNAL (quitsig()),this, SLOT (slotQuit()));

    // Ici la vue du perso, plus immersive !
    QGraphicsView* rouj = new QGraphicsView();
    rouj->setScene(mainScene);
    rouj->scale(0.50,0.50);
    rouj->setWindowTitle("Third Person");
    rouj->resize(1300,900);
    rouj->show();

    // Ici la vue de toute la map pour le developpement :
    this->setCentralWidget(rouj);
    this->setWindowTitle("Whole");
    this->resize(1320, 800);

    rouj->setBackgroundBrush(Qt::black);
}

void MainWindow::slotReboot() {
    qApp->exit( MainWindow::EXIT_CODE_REBOOT );
}
void MainWindow::slotQuit() {
    qApp->exit( 0 );
}
