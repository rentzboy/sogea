#include "mainwindow.h"
#include "dbconnectionform.h"
#include "excepciones.h"
#include "globals.h"
#include <QApplication>
#include <typeinfo>
#include  <exception>

int main(int argc, char *argv[])
{  
    try
    {
        QApplication a(argc, argv);
        mainWindow mainWindow;
        dbConnectionForm dbConnectorWindow;

        mainWindow.show();
        dbConnectorWindow.show();

        return QApplication::exec();
    }
    catch (excepciones &e)
    {
        e.saveToLogFile();
    }
    catch (std::exception &e)
    {
        CATCHEXCEPTION
    }
}
