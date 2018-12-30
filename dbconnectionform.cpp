#include "dbconnectionform.h"
#include "globals.h"
#include "dbconnector.h"
#include <memory>

/* CONSTRUCTOR */
dbConnectionForm::dbConnectionForm(QWidget *parent, Qt::WindowFlags flag) : QDialog(parent, flag)
{
    qDebug() << "Se ha llamado al constructor de dbConnectionForm";

    errorMsgdbConnectionForm = new QErrorMessage(this);
    //Qt automatically calls sizeHint() @ the constructor for each Widget
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred); //horizontal & vertical
    this->setWindowTitle(QObject::tr("DB connector"));

    //Botones y campos del widget -hay que crearlos antes del setLayout()
    comboList = new QStringList; //RAII managed by Qt container QList<QString>
    comboList->append("QPSQL");
    comboList->append("QMYSQL");
    comboList->append("QSQLlite");
    typeComboBox = new QComboBox(this);
    typeComboBox->addItems(*comboList);
    databaseLineEdit = new QLineEdit(QObject::tr("Sogea"), this);
    usernameLineEdit = new QLineEdit(QObject::tr("postgres"),this);
    passwordLineEdit = new QLineEdit(QObject::tr("torranz"),this);
    serverLineEdit = new QLineEdit(QObject::tr("localhost"),this);
    aceptarButton = new QPushButton(QObject::tr("Aceptar"), this);
    cancelarButton = new QPushButton(QObject::tr("Cancelar"), this);

    //Layouts del widget
    formLayout = new QFormLayout(this);
    formLayout->addRow(QObject::tr("Type:"), typeComboBox);
    formLayout->addRow(QObject::tr("Database:"), databaseLineEdit);
    formLayout->addRow(QObject::tr("Username:"), usernameLineEdit);
    formLayout->addRow(QObject::tr("Password:"), passwordLineEdit);
    formLayout->addRow(QObject::tr("Server:"), serverLineEdit);
    formLayout->addWidget(aceptarButton);
    formLayout->addWidget(cancelarButton);
    this->setLayout(formLayout);

    connect(aceptarButton, &QPushButton::clicked, this, &dbConnectionForm::accept);
    connect(cancelarButton, &QPushButton::clicked, this, &dbConnectionForm::reject);
}

/* PUBLIC SLOTS MEMBERS */
void dbConnectionForm::accept(void) //override
{
    qDebug() << "Se ha hecho click en el botón Aceptar.";
    if(!this->checkFieldsNotEmpty())
    {
        if (!this->createNewDbConnection()) //Funcionalidad extra
            this->done(QDialog::Accepted); //Funcionalidad del slot accept() original
    }
}
void dbConnectionForm::reject(void) //override
{
    qDebug() << "Se ha hecho click en el botón Cancelar.";
    this->done(QDialog::Rejected); //Funcionalidad del slot reject() original
}
/* PUBLIC MEMBERS */
bool dbConnectionForm::createNewDbConnection(void)
{
    if(connectionId>MAXDBCONNECTIONS-1)
    {
        errorMsgdbConnectionForm->showMessage(QObject::tr("Se ha alcanzado el máximo de conexiones!"));
        return EXIT_FAILURE;
    }

    qDebug() << "Inicio proceso de conexión con la db";

    comboList = new QStringList;  //RAII managed by Qt container QList<QString>
    comboList->append(typeComboBox->currentText());  //[0]
    comboList->append(databaseLineEdit->text());           //[1]
    comboList->append(usernameLineEdit->text());         //[2]
    comboList->append(passwordLineEdit->text());         //[3]
    comboList->append(serverLineEdit->text());              //[4]

    //Connect to database using dbconnector
    dbConnection[connectionId] = new dbConnector; //dumbDB (global), no hay que deletearlo hasta cerrar la conexión con la db
    if(!dbConnection[connectionId]->setUpConnectionToDatabase(*comboList))
    {
        connectionId++;
        qDebug() << QObject::tr("Conexiones abiertas con bases de datos: ") << connectionId;
        return EXIT_SUCCESS;
    }
    else return EXIT_FAILURE;
}
void dbConnectionForm::closeDatabaseDialog(void) //SIN TERMINAR -NO VA AQUI !!
{
    //Pop-up to close db connection
    QMessageBox msgBoxCloseDb;
    msgBoxCloseDb.setWindowTitle(QObject::tr("Shut down db connection"));
    msgBoxCloseDb.setText(QObject::tr("Confirma que quier cerrar la conexión con la database ?"));
    msgBoxCloseDb.setWindowModality(Qt::NonModal);
    msgBoxCloseDb.setStandardButtons(QMessageBox::Ok);
    msgBoxCloseDb.setStandardButtons(QMessageBox::Cancel);
    int boton = msgBoxCloseDb.exec();
    switch (boton) {
        case QMessageBox::Ok: //eliminar la conexion con la dabase actual
        errorMsgdbConnectionForm->showMessage(QObject::tr("Se ha cerrado la conexión con la Db"));
        break;
    case QMessageBox::Cancel:
        break;
    }
}
QString dbConnectionForm::getTypeComboActivated(void) const
{return typeComboBox->currentText();}
QString dbConnectionForm::getDatabaseFieldText(void) const
{return databaseLineEdit->text();}
QString dbConnectionForm::getUsernameFieldText(void) const
{return usernameLineEdit->text();}
QString dbConnectionForm::getPasswordFieldText(void) const
{return passwordLineEdit->text();}
QString dbConnectionForm::getServerFieldText(void) const
{return serverLineEdit->text();}

/* PRIVATE MEMBERS */
QSize dbConnectionForm::sizeHint() const
{
    QSize size(300, 400);
    return size;
}
bool dbConnectionForm::checkFieldsNotEmpty(void)
{
    //Check if any field is empty
    if(typeComboBox->currentText().isEmpty() || databaseLineEdit->text().isEmpty() ||
        usernameLineEdit->text().isEmpty() || passwordLineEdit->text().isEmpty() ||
        serverLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, QObject::tr("Hay campos vacios"),
                                                        QObject::tr("Por favor revise los campos!"),
                                                        QMessageBox::Ok);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
