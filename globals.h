#ifndef GLOBALS_H
#define GLOBALS_H
#endif // GLOBALS_H

#include <QtWidgets>
#include "dbconnectionform.h"

/* GLOBAL VARIABLES */

/* DEFINES */
#define DATABASE_TYPE                       "QPSQL"
#define DATABASE_NAME                      "Sogea"
#define DATABASE_QPSQL_NAME       "QPSQL_Sogea"
#define DATABASE_SERVER_URL        "localhost"
#define EXCEPTION_HANDLER                             excepciones ex(e, __FILE__, __FUNCTION__,  __LINE__);
/*Deprecated
#define CATCH_STD_EXCEPTION        excepciones catchedException (e, __FILE__, __FUNCTION__,  __LINE__); \

#define THROW_EXCEPCION(msg)       excepciones throwedException (__FILE__, __FUNCTION__,  __LINE__, #msg); \
                                                                     throw throwedException; //continuación de la linea anterior
                                                                     */

#define STATUSBAR_MESSAGE(msg, timeout)     static_cast<mainWindow*>(this->parent())->statusBar()->showMessage(QObject::tr(msg), timeout);



