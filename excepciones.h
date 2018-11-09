#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <QtWidgets>
#include <QDateTime>
#include <QSysInfo>
#include <exception>

/* IMPORTANTE: Diseñamos esta clase pues no se pueden generar members de la clase base exceptions
 * ya que esta clase úncamente tiene el member what() */

class excepciones : public std::exception
{
public:
    excepciones(const std::exception&, const QString&, const QString&, int); //ctror #1
    excepciones(const QString&, const QString&, int, const QString&); //ctror #2 (no cambiar el orden x la macro)
    ~excepciones() = default;
    excepciones(const excepciones& e) = default; //throw expression: copy-initialize the expression !!
    excepciones& operator =(const excepciones&) = delete;

    void saveToLogFile(void);
    void saveToLogTable(void);

private:
     //Attributes
    QStringList logExceptions;
    QString hostName = Q_NULLPTR;
    QString operativeSystem = Q_NULLPTR;
    QDateTime systemTime;
    QString fileName = Q_NULLPTR;
    QString functionName = Q_NULLPTR;
    QString errorDescription = Q_NULLPTR;
    int fileLine;
    int userID ;
};

#endif // EXCEPCIONES_H
