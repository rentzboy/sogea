#include "dbconnector.h"

/* CONSTRUCTOR */
bool dbConnector::setUpConnectionToDatabase(QStringList connectionDetails)
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
     qDebug() << "Drivers instalados: " << installedDrivers;
    if(!installedDrivers.contains(dbDriver))
    {
        errorMsgdbconnector.showMessage(QObject::tr("No se ha encontrado el driver para conectar a la database"));
        return EXIT_FAILURE;
    }

    /* IMPORTANTE: No se puede utilizar el puntero "this" pues addDatabase() [static]
        No se puede crear un objeto QSqlDatabase y luego añadirle la database (NO funciona)
        La única manera es hacerlo tal como se indica aqui */

    //Open connection to db
    db = QSqlDatabase::addDatabase(dbDriver); //static function
    db.setDatabaseName(dbName);
    db.setUserName(dbUserName);
    db.setPassword(dbPassword);
    db.setHostName(dbHost);
    if(!db.open())
    {
        QString errorToShow = QObject::tr("No se ha podido establecer conexión con la db." );
        errorToShow.append(db.lastError().text());
        errorMsgdbconnector.showMessage(errorToShow);
        return EXIT_FAILURE;
    }
    
    qDebug() << "DB opened successfully! \nDatos de la conexión: " << dbConnector::database();
    return EXIT_SUCCESS;
}


