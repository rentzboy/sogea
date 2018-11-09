#include "excepciones.h"
#include "globals.h"

/* CONSTRUCTORS */
excepciones::excepciones(const std::exception &e, const QString& _fileName,
                                           const QString& _functionName, int _lineNumber)
    try : hostName(QSysInfo::machineHostName()), operativeSystem(QSysInfo::productVersion()),
           systemTime(QDateTime::currentDateTime()), fileName(_fileName), functionName(_functionName),
           errorDescription(e.what()), fileLine(_lineNumber), userID(99)
{
    qDebug() << "Se ha llamado al constructor de excepciones";
    //throw e;
}catch(std::exception &ex)
{
    /* Catch de excepciones del constructor
     * Utilizamos la clase genérica <exception> pues el fallo se ha producido en la clase derivada
     * NO permite sacar la excepción mediante ningún tipo de MsgBox/Dialog, no se puede crear en el CATCH */
    qCritical() << "Error en el constructor de excepciones: " << ex.what();
    exit(EXIT_FAILURE); //termina el programa.
}

excepciones::excepciones(const QString& _fileName,
                                           const QString& _functionName, int _lineNumber, const QString& errorMsg)
    try : hostName(QSysInfo::machineHostName()), operativeSystem(QSysInfo::productVersion()),
           systemTime(QDateTime::currentDateTime()), fileName(_fileName), functionName(_functionName),
           errorDescription(errorMsg), fileLine(_lineNumber), userID(99)
{
    qDebug() << "Se ha llamado al constructor de excepciones";
    //throw e;
}catch(std::exception &ex)
{
    /* Catch de excepciones del constructor
     * Utilizamos la clase genérica <exception> pues el fallo se ha producido en la clase derivada
     * NO permite sacar la excepción mediante ningún tipo de MsgBox/Dialog, no se puede crear en el CATCH */
    qCritical() << "Error en el constructor de excepciones: " << ex.what();
    exit(EXIT_FAILURE); //termina el programa.
}

/* PUBLIC MEMBERS */
void excepciones::saveToLogFile()
{
    qDebug() << "Se ha llamado a saveToLogFile()";

    logExceptions << hostName << operativeSystem << systemTime.toString() << QString::number(userID)
                             << fileName << QString::number(fileLine) << functionName << errorDescription;

    //QFile logFile("/home/renti/Qt/Proyectos/Sogea/amandilamelaaaa.txt");
    QFile logFile("/home/renti/Qt/Proyectos/Sogea/Sogea/Logs/exceptions.txt");
     if (!logFile.open(QFile::WriteOnly | QFile::Append))
        qCritical() << "No se ha podido abrir el archivo exceptions.txt !";
     else
     {
         qDebug() << "Volcando excepción al archivo.....";
         QTextStream out(&logFile);
         QString iterator;
         foreach(iterator, logExceptions)
         {
             out << qSetFieldWidth(30) << left << iterator;
         }
         out << "\n";
     }
}
void excepciones::saveToLogTable()
{
    qDebug() << "Se ha llamado a saveToLogTable()";
    logExceptions << hostName << operativeSystem << systemTime.toString() << QString::number(userID)
                             << fileName << QString::number(fileLine) << functionName << errorDescription;
}

