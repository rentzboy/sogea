#ifndef dbConnector_H
#define dbConnector_H

#include <QtWidgets>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlError>

// ESTE CONECTOR ES UNA MIERDA Y NO SIRVE. SE HACE TODO EN FILE dbConnector
class dbConnector : public QSqlDatabase
{
public:
    //Use addDatabase(), removeDatabase(), and database() to get valid QSqlDatabase objects.
    explicit dbConnector() = default; //dbConnector(): creates an empty, invalid QSqlDatabase object.
    ~dbConnector() = default;
    dbConnector(const dbConnector&) = delete; //delete: borra las definiciones automáticas del compilador
    dbConnector& operator =(const dbConnector&) = delete; //también se podrian haber declarado private
    bool setUpConnectionToDatabase(const QStringList connectionDetails);

private:
    //Attributes
    QStringList installedDrivers;
    QString dbDriver;
    QString dbName;
    QString dbUserName;
    QString dbPassword;
    QString dbHost;
    QSqlDatabase db;
    QErrorMessage errorMsgdbconnector;
};

#endif // dbConnector_H
