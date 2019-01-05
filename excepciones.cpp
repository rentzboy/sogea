#include "excepciones.h"
#include "globals.h"
#include <QSqlError>
#include <QErrorMessage>
#include <fstream>   //istream
#include <sstream>   //ostringstream

/* CONSTRUCTORS */
excepciones::excepciones(const std::exception &e, const QString& _fileName, //std::exception
                                           const QString& _functionName, int _lineNumber)
                                            try : hostName(QSysInfo::machineHostName()),
                                                    operativeSystem(QSysInfo::productVersion()),
                                                    systemTime(QDateTime::currentDateTime()),
                                                    fileName(_fileName),
                                                    functionName(_functionName),
                                                    errorDescription(e.what()),
                                                    fileLine(_lineNumber),
                                                    userName(99)
{
    qDebug() << "Se ha llamado al constructor de excepciones";
    this->saveExceptionInterface();
}catch(const std::exception &ex)
{
    qCritical() << "Error en el constructor de excepciones: " << ex.what();
    exit(EXIT_FAILURE); //termina el programa.
}catch(...)
{
    qCritical() << "Error DESCONOCIDO en el constructor de excepciones!!";
    exit(EXIT_FAILURE); //termina el programa.
}

excepciones::excepciones(const QSqlError &e, const QString& _fileName, //QSqlError
                                           const QString& _functionName, int _lineNumber)
                                           try : hostName(QSysInfo::machineHostName()),
                                                   operativeSystem(QSysInfo::productVersion()),
                                                   systemTime(QDateTime::currentDateTime()),
                                                   fileName(_fileName),
                                                   functionName(_functionName),
                                                   errorDescription(e.databaseText()),
                                                   fileLine(_lineNumber),
                                                   userName(99)
{
    qDebug() << "Se ha llamado al constructor de excepciones";
    saveExceptionInterface();
}catch(const std::exception &ex)
{
    qCritical() << "Error en el constructor de excepciones: " << ex.what();
    exit(EXIT_FAILURE); //termina el programa.
}catch(...)
{
    qCritical() << "Error DESCONOCIDO en el constructor de excepciones!!";
    exit(EXIT_FAILURE); //termina el programa.
}

excepciones::excepciones(const QString& errorMsg, const QString& _fileName, //QString
                                          const QString& _functionName, int _lineNumber)
                                           try : hostName(QSysInfo::machineHostName()),
                                                   operativeSystem(QSysInfo::productVersion()),
                                                   systemTime(QDateTime::currentDateTime()),
                                                   fileName(_fileName),
                                                   functionName(_functionName),
                                                   errorDescription(errorMsg),
                                                   fileLine(_lineNumber),
                                                   userName(99)
{
    qDebug() << "Se ha llamado al constructor de excepciones";
    this->saveExceptionInterface();
}catch(const std::exception &ex)
{
    qCritical() << "Error en el constructor de excepciones: " << ex.what();
    exit(EXIT_FAILURE); //termina el programa.
}catch(...)
{
    //NO permite sacar la excepción mediante ningún tipo de MsgBox/Dialog, no se puede crear en el CATCH
    qCritical() << "Error DESCONOCIDO en el constructor de excepciones!!";
    exit(EXIT_FAILURE); //termina el programa.
}

/* PRIVATE MEMBERS */
void excepciones::saveExceptionInterface(void)
{
    /* Al generarse una exception se intenta guardar en la Db, pero si se genera
     * una exception en saveToLogTable(), este caso se intenta guardar
     *  en un archivo de texto. Si da fallo también aqui se muestra un qCritical() */
    /* Importante: Yo en saveToLogFile() quiero guardar la exception inicial,
     * no la que se ha generado en saveToLogTable(), pues si veo que se ha guardado
     * la exception en saveToLogFile() ya doy por hecho que ha fallado saveToLogTable() */
    /* Podria eliminar saveExceptionInterface() y llamar a las 2 funciones directamente
     * desde los constructors, pero resulta más fácil hacer los cambios asi */
    if(this->saveToLogTable() == EXIT_SUCCESS)
        this->saveToLogFile("STL");
}
bool excepciones::saveToLogTable(void)
{
    try
    {
        qDebug() << "Se ha llamado a saveToLogTable() => PENDIENTE CODIFICAR";
        //OJO: Despues de codificar esta parte en el interface cambiar
        //this->saveToLogTable() == EXIT_SUCCESS) ==> this->saveToLogTable() == EXIT_FAILURE)

        return EXIT_SUCCESS;
    }catch(const QSqlError &e)
    {
        qCritical() << "Error en el constructor de excepciones al guardar en la DB: " << e.databaseText();
        return EXIT_FAILURE; //no exit() !!

    }catch (QString &e)
    {
        qCritical() << "Error en el constructor de excepciones al guardar en la DB: " << e;
        return EXIT_FAILURE; //no exit() !!
    }
}
void excepciones::saveToLogFile(QString method)
{
    try
    {
        qDebug() << "Se ha llamado a saveToLogFile()";
        logExceptions << QObject::tr("Time: ") << systemTime.toString() << "\n"
                                 << QObject::tr("System Operative: ") << operativeSystem << "\n"
                                << QObject::tr("Hostname: ") << hostName << "\n"
                                 << QObject::tr("User: ") << QString::number(userName) << "\n"
                                 << QObject::tr("File: ") << fileName << "\n"
                                 << QObject::tr("Function name: ") << functionName << "\n"
                                 << QObject::tr("Line: ") << QString::number(fileLine) << "\n"
                                 << QObject::tr("Error Description: ") << errorDescription << "\n\n\n";

        if(method == "STL") //OPCION #1: STL
        {
            std::string fileUrl ("/home/renti/Qt/Proyectos/sogea/Sogea/Logs/exceptions.txt");
            std::ofstream outputFile(fileUrl, std::ios_base::out | std::ios_base::app);
            outputFile.exceptions(std::ios_base::badbit | std::ios_base::failbit); //Activar las excepciones
            if(!outputFile.is_open())
                throw(QObject::tr("No se ha podido abrir (STL) el archivo de excepcionts.txt !"));
                
            qDebug() << "Volcando excepción al archivo.....";
            outputFile << QObject::tr("Time: ").toStdString() << systemTime.toString().toStdString() << "\n"
                       << QObject::tr("System Operative: ").toStdString() << operativeSystem.toStdString() << "\n"
                      << QObject::tr("Hostname: ").toStdString() << hostName.toStdString() << "\n"
                       << QObject::tr("User: ").toStdString() << QString::number(userName).toStdString() << "\n"
                       << QObject::tr("File: ").toStdString() << fileName.toStdString() << "\n"
                       << QObject::tr("Function name: ").toStdString() << functionName.toStdString() << "\n"
                       << QObject::tr("Line: ").toStdString() << QString::number(fileLine).toStdString() << "\n"
                       << QObject::tr("Error Description: ").toStdString() << errorDescription.toStdString() << "\n\n\n";
                qDebug() << QObject::tr("Se ha grabado (STL) un error al archivo de texto");
        }
        else //OPCION #2: Qt
        {
            QFile logFile("/home/renti/Qt/Proyectos/sogea/Sogea/Logs/exceptions.txt");
             if (!logFile.open(QFile::WriteOnly | QFile::Append))
                 throw(QObject::tr("No se ha podido abrir (Qt) el archivo de excepcionts.txt !"));
             
             
                 qDebug() << "Volcando excepción al archivo.....";
                 QTextStream out(&logFile);
                 QString iterator;
                 foreach(iterator, logExceptions)
                     out << iterator;
                 if(out.status() !=QTextStream::Ok)
                     throw(QObject::tr("Eror al grabar (Qt) el archivo de excepcionts.txt !"));

                 qDebug() << (QObject::tr("Se ha grabado (Qt) un error al archivo de texto"));
             
        }
    }catch(const std::ios_base::failure &e)
    {
         qCritical() << "Error en el constructor de excepciones al guardar en el archivo de texto: " << e.what();
        exit(EXIT_FAILURE); //termina el programa.
    }catch (const QString &e)
    {
        qCritical() << "Error en el constructor de excepciones al guardar en el archivo de texto: " << e;
        exit(EXIT_FAILURE); //termina el programa.
    }
}

/* PUBLIC MEMBERS */
