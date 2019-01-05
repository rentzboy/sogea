#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <QtWidgets>
#include <QDateTime>
#include <QSysInfo>
#include <exception>
#include <QSqlError>

/* IMPORTANTE: Diseñamos esta clase pues no se pueden generar members de la clase base exceptions
 * ya que esta clase úncamente tiene el member what() */

class excepciones
{
public:
    excepciones(const std::exception&, const QString&, const QString&, int); //ctror #1
    excepciones(const QSqlError&, const QString&, const QString&, int);  //ctror #2
    excepciones(const QString&, const QString&, const QString&, int); //ctror #3
    ~excepciones() = default;  //ctror #4
    excepciones(const excepciones& e) = default; //throw expression: copy-initialize the expression !!
    excepciones& operator =(const excepciones&) = delete;

private:
    //members
    void saveExceptionInterface(void);
    bool saveToLogTable(void);
    void saveToLogFile(QString method); //Using Qt or STL
    //Attributes
    QStringList logExceptions;
    QString hostName = Q_NULLPTR;
    QString operativeSystem = Q_NULLPTR;
    QDateTime systemTime;
    QString fileName = Q_NULLPTR;
    QString functionName = Q_NULLPTR;
    QString errorDescription = Q_NULLPTR;
    int fileLine;
    int userName ;
};

#endif // EXCEPCIONES_H
