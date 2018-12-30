#ifndef dbConnector_H
#define dbConnector_H

#include <QtWidgets>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlError>

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
    QErrorMessage errorMsgDbconnector;
};

#endif // dbConnector_H
