#ifndef GLOBALS_H
#define GLOBALS_H
#endif // GLOBALS_H

#include <QtWidgets>

/* GLOBAL VARIABLES */
extern QErrorMessage globlalErrorMsg;

/* DEFINES */
#define CATCHEXCEPTION                    excepciones catchedException (e, __FILE__, __FUNCTION__,  __LINE__); \
                                                                     catchedException.saveToLogFile();
#define THROWEXCEPCION(msg)         excepciones throwedException (__FILE__, __FUNCTION__,  __LINE__, #msg); \
                                                                     throw throwedException;




