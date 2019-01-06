#include "mainwindow.h"
#include "dbconnectionform.h"
#include "excepciones.h"
#include "globals.h"
#include <QApplication>
#include <typeinfo>
#include  <exception>
//xxxx
int main(int argc, char *argv[])
{  
    try
    {
        QApplication a(argc, argv);
        mainWindow mainWidget;
        mainWidget.show();

        return QApplication::exec();
    }
    catch (std::exception &e)
    {
        EXCEPTION_HANDLER
    }
    catch (QString &e)
    {
        EXCEPTION_HANDLER
    }
}
