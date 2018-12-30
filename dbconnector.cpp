#include "dbconnector.h"
#include "excepciones.h"
#include "globals.h"

/* CONSTRUCTOR */

/* PUBLIC MEMBER */
bool dbConnector::setUpConnectionToDatabase(QStringList connectionDetails)
{
    try
    {
        //Asignar valores a los data members
        dbDriver = connectionDetails[0];
        dbName = connectionDetails[1];
        dbUserName = connectionDetails[2];
        dbPassword = connectionDetails[3];
        dbHost = connectionDetails[4];
        qDebug() << connectionDetails;

        //Checking installed db plugins
        QStringList installedDrivers = QSqlDatabase::drivers(); //static function
        if(!installedDrivers.contains(dbDriver))
            throw(QObject::tr("Driver necesario para conectar a la Db no instalado")); //EXIT_FAILURE

        /* IMPORTANTE: No se puede utilizar el puntero "this" pues addDatabase() [static]
            No se puede crear un objeto QSqlDatabase y luego añadirle la database (NO funciona)
            La única manera es hacerlo tal como se indica aqui */

        //Open connection to db
        QString connectionName;
        connectionName.append(dbDriver).append("_").append(dbName);

        db = QSqlDatabase::addDatabase(dbDriver, connectionName); //static function
        db.setDatabaseName(dbName);
        db.setUserName(dbUserName);
        db.setPassword(dbPassword);
        db.setHostName(dbHost);
        if(!db.open())
             throw(QSqlDatabase::lastError()); //EXIT_FAILURE

        qDebug() << QObject::tr("DB connection ") << db.connectionName() << QObject::tr(" created.");
        qDebug() << QObject::tr("DB ") << db.databaseName() << QObject::tr(" opened successfully!");
        return EXIT_SUCCESS;
    }
    catch (const QSqlError &e)
    {
        EXCEPTION_HANDLER
        return EXIT_FAILURE;
    }
    catch (const QString &e)
    {
        EXCEPTION_HANDLER
        return EXIT_FAILURE;
    }
}



