#ifndef DBCONNECTIONFORM_H
#define DBCONNECTIONFORM_H

#include <QtWidgets>
#include "dbconnector.h"
#include <memory>

class dbConnectionForm : public QDialog
{
    Q_OBJECT

public:
    explicit dbConnectionForm(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flag = Qt::Widget);
     /* Al crear el member sizeHint() override el detructor de la clase base tiene que ser virtual también,
    x lo que hay que añadir override en el destructor;  Even though destructors are not inherited,
    a destructor in a derived class overrides a base class destructor declared virtual;*/
    ~dbConnectionForm() override = default;
    dbConnectionForm(const dbConnectionForm&) = delete; //delete: borra las definiciones automáticas del compilador
    dbConnectionForm& operator =(const dbConnectionForm&) = delete; //también se podrian haber declarado private

    QString getTypeComboActivated(void);
    QString getDatabaseFieldText(void);
    QString getUsernameFieldText(void);
    QString getPasswordFieldText(void);
    QString getServerFieldText(void);
    void closeDatabaseDialog(void);

public slots:
    bool connectToDatabase(void);
    void closeDatabase(void);
    void accept(void) override;
    void reject(void) override;

signals:

private:
    //members
    QSize sizeHint() const override;
    bool checkFieldsNotEmpty(void);
    //attributes
    QFormLayout *formLayout = Q_NULLPTR;
    QPushButton *aceptarButton = Q_NULLPTR;
    QPushButton *cancelarButton = Q_NULLPTR;
    QComboBox *typeComboBox = Q_NULLPTR;
    QLineEdit *databaseLineEdit = Q_NULLPTR;
    QLineEdit *usernameLineEdit = Q_NULLPTR;
    QLineEdit *passwordLineEdit = Q_NULLPTR;
    QLineEdit *serverLineEdit = Q_NULLPTR;
    QStringList *comboList = Q_NULLPTR;
    dbConnector *miConexion = Q_NULLPTR;
    QDialog *closeDbDialog = Q_NULLPTR;
    QErrorMessage *errorMsgdbConnectionForm = Q_NULLPTR;
};

#endif // DBCONNECTIONFORM_H
