#include "dbconnectionform.h"
#include "globals.h"
#include "mainwindow.h"
#include <memory>
#include <QSettings>

/* CONSTRUCTOR */
dbConnectionForm::dbConnectionForm(QWidget *parent, Qt::WindowFlags flag) : QDialog(parent, flag)
{
    qDebug() << "Se ha llamado al constructor de dbConnectionForm";
    errorMsgdbConnectionForm = new QErrorMessage(this);

    //Qt automatically calls sizeHint() @ the constructor for each Widget
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred); //horizontal & vertical
    this->setWindowTitle(QObject::tr("DB connector"));

    //Botones y campos del widget -hay que crearlos antes del setLayout()
    QStringList comboList; //RAII managed by Qt container QList<QString>
    comboList.append("QPSQL");
    comboList.append("QMYSQL");
    comboList.append("QSQLlite");
    typeComboBox = new QComboBox(this);
    typeComboBox->addItems(comboList);
    databaseLineEdit = new QLineEdit(QObject::tr(DATABASE_NAME), this);
    usernameLineEdit = new QLineEdit(this->readUsernameSettings(),this);
    passwordLineEdit = new QLineEdit(QObject::tr("torranz"),this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    serverLineEdit = new QLineEdit(QObject::tr(DATABASE_SERVER_URL),this);
    aceptarButton = new QPushButton(QObject::tr("Aceptar"), this);
    cancelarButton = new QPushButton(QObject::tr("Cancelar"), this);
    alertMessage = new QLabel(QObject::tr("Error: Revise los datos"), this);
    alertMessage->setVisible(false);

    //Layouts del widget
    formLayout = new QFormLayout(this);
    formLayout->addRow(QObject::tr("Type:"), typeComboBox);
    formLayout->addRow(QObject::tr("Database:"), databaseLineEdit);
    formLayout->addRow(QObject::tr("Username:"), usernameLineEdit);
    formLayout->addRow(QObject::tr("Password:"), passwordLineEdit);
    formLayout->addRow(QObject::tr("Server:"), serverLineEdit);
    formLayout->addWidget(alertMessage);
    formLayout->addWidget(aceptarButton);
    formLayout->addWidget(cancelarButton);
    this->setLayout(formLayout);

    //Signals & Slots
    this->aceptarButton->setObjectName(QObject::tr("aceptar"));
    this->cancelarButton->setObjectName(QObject::tr("cancelar"));
    QMetaObject::connectSlotsByName(this);
    connect(this, &dbConnectionForm::exportDbConnectionDetails,
                  static_cast<mainWindow*>( this->parent()), &mainWindow::importDbConnectionDetails);
}

/* PUBLIC SLOTS MEMBERS */
void dbConnectionForm::on_aceptar_clicked(void)
{
    qDebug() << "Se ha hecho click en el botón Aceptar.";
    if(!this->FieldsAreEmpty())
    {
        this->set_DbConnectionDetails();
        emit exportDbConnectionDetails(); //Signal
        this->done(QDialog::Accepted); //Funcionalidad del slot accept() original
    }
}
void dbConnectionForm::on_cancelar_clicked(void)
{
    qDebug() << "Se ha hecho click en el botón Cancelar.";
    this->done(QDialog::Rejected); //Funcionalidad del slot reject() original
}

/* PRIVATE MEMBERS */
QSize dbConnectionForm::sizeHint() const
{
    QSize size(300, 400);
    return size;
}
bool dbConnectionForm::FieldsAreEmpty(void)
{
    //Check if any field is empty
    if(typeComboBox->currentText().isEmpty() || databaseLineEdit->text().isEmpty() ||
        usernameLineEdit->text().isEmpty() || passwordLineEdit->text().isEmpty() ||
        serverLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, QObject::tr("Hay campos vacios"),
                                                        QObject::tr("Por favor revise los campos!"),
                                                        QMessageBox::Ok);
        return true;
    }
    return false;
}
void dbConnectionForm::set_DbConnectionDetails()
{
    DbConnectionDetails["databaseType"] = this->get_TypeComboActivated();
    DbConnectionDetails["database"] = this->get_DatabaseFieldText();
    DbConnectionDetails["username"] = this->get_UsernameFieldText();
    DbConnectionDetails["password"] = this->get_PasswordFieldText();
    DbConnectionDetails["serverUrl"] = this->get_ServerFieldText();
}
QString dbConnectionForm::readUsernameSettings(void)
{
    //Retrieve configuration from the last user's session
    QSettings userSettings (QObject::tr("Fx Team®"), QObject::tr("Sogea"), this);
    userSettings.beginGroup(QObject::tr("mainwindow"));
    userSettings.value(QObject::tr("username"));
    QString tmp = userSettings.value(QObject::tr("username")).toString();
    userSettings.endGroup();
    return tmp;
}

/* PUBLIC MEMBERS */
QString dbConnectionForm::get_TypeComboActivated(void) const
{return typeComboBox->currentText();}
QString dbConnectionForm::get_DatabaseFieldText(void) const
{return databaseLineEdit->text();}
QString dbConnectionForm::get_UsernameFieldText(void) const
{return usernameLineEdit->text();}
QString dbConnectionForm::get_PasswordFieldText(void) const
{return passwordLineEdit->text();}
QString dbConnectionForm::get_ServerFieldText(void) const
{return serverLineEdit->text();}
QMap<QString, QString> dbConnectionForm::get_DbConnectionDetails() const
{return DbConnectionDetails;}
void dbConnectionForm::set_AlertVisible(bool active)
{
    alertMessage->setStyleSheet(QObject::tr("color:red"));
    alertMessage->setVisible(active);
}
