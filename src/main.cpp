#include <QApplication>
#include <QIcon>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icon.ico"));

    MainWindow ventana;
    ventana.show();

    return app.exec();
}