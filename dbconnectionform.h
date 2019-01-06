#ifndef DBCONNECTIONFORM_H
#define DBCONNECTIONFORM_H

#include <QtWidgets>

class dbConnectionForm : public QDialog
{
    Q_OBJECT

public:
    explicit dbConnectionForm(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flag = Qt::Widget);
     /* Al crear el member sizeHint() override el detructor de la clase base tiene que ser virtual también,
      * x lo que hay que añadir override en el destructor;  Even though destructors are not inherited,
      * a destructor in a derived class overrides a base class destructor declared virtual; */
    ~dbConnectionForm() override = default;
    dbConnectionForm(const dbConnectionForm&) = delete; //borra las definiciones automáticas del compilador
    dbConnectionForm& operator =(const dbConnectionForm&) = delete; //tb se podrian haber declarado private
    //public members
    QString get_TypeComboActivated (void) const;
    QString get_DatabaseFieldText(void) const;
    QString get_UsernameFieldText(void) const;
    QString get_PasswordFieldText(void) const;
    QString get_ServerFieldText(void) const;
    QMap<QString, QString> get_DbConnectionDetails(void) const;
    void set_AlertVisible(bool);

public slots:
    void on_aceptar_clicked(void);
    void on_cancelar_clicked(void);
signals:
    void exportDbConnectionDetails(void);

private:
    //private members
    QSize sizeHint() const override;
    bool FieldsAreEmpty(void);
    void set_DbConnectionDetails(void);
    QString readUsernameSettings(void);
    //attributes
    QFormLayout *formLayout = Q_NULLPTR;
    QPushButton *aceptarButton = Q_NULLPTR;
    QPushButton *cancelarButton = Q_NULLPTR;
    QComboBox *typeComboBox = Q_NULLPTR;
    QLineEdit *databaseLineEdit = Q_NULLPTR;
    QLineEdit *usernameLineEdit = Q_NULLPTR;
    QLineEdit *passwordLineEdit = Q_NULLPTR;
    QLineEdit *serverLineEdit = Q_NULLPTR;
    QLabel *alertMessage = Q_NULLPTR;
    QMap<QString, QString> DbConnectionDetails;
    QErrorMessage *errorMsgdbConnectionForm = Q_NULLPTR;
};

#endif // DBCONNECTIONFORM_H
