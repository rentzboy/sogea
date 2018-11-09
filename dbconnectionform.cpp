#include "dbconnectionform.h"

/* CONSTRUCTOR */
dbConnectionForm::dbConnectionForm(QWidget *parent, Qt::WindowFlags flag) : QDialog(parent, flag)
{
    errorMsgdbConnectionForm = new QErrorMessage(this);
    //Qt automatically calls sizeHint() @ the constructor for each Widget
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred); //horizontal & vertical
    this->setWindowTitle(QObject::tr("DB connector"));

    //Botones y campos del widget -hay que crearlos antes del setLayout()
    comboList = new QStringList; //RAII managed by Qt container
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

/* SLOTS */
void dbConnectionForm::accept(void)
{
    qDebug() << "Se ha hecho click en el botón Aceptar.";

    if(!this->checkFieldsNotEmpty())
    {
         //Funcionalidad extra
        if (!this->connectToDatabase())
        {
            //Hay que implementar la funcionalidad del slot accept() original
            this->done(QDialog::Accepted);
            this->hide();
        }
    }
}
void dbConnectionForm::reject(void)
{
    qDebug() << "Se ha hecho click en el botón Cancelar.";
    //Hay que implementar la funcionalidad del slot reject() original
    this->hide();
    this->done(QDialog::Rejected);
}

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
        return EXIT_FAILURE; //[1]
    }
    return EXIT_SUCCESS;  //[0]
}

/* PUBLIC MEMBERS */
bool dbConnectionForm::connectToDatabase(void)
{
    qDebug() << "Inicio proceso de conexión con la db";

    comboList = new QStringList;  //RAII managed by Qt container
    comboList->append(typeComboBox->currentText());  //[0]
    comboList->append(databaseLineEdit->text());           //[1]
    comboList->append(usernameLineEdit->text());         //[2]
    comboList->append(passwordLineEdit->text());         //[3]
    comboList->append(serverLineEdit->text());              //[4]

    // OPCION #1: Connect to database using dbconnector
    miConexion = new dbConnector; //dumbDB, no hay que deletearlo hasta que se cierra la conexión con la db
    return miConexion->setUpConnectionToDatabase(*comboList);

    /* OPCION #2 DEPRECATED: Connect to database
    //Checking installed db plugins
    QStringList installedDrivers = QSqlDatabase::drivers();
     qDebug() << "Drivers instalados: " << installedDrivers;
    if(!installedDrivers.contains(comboList->value(0)))
    {
        QMessageBox::warning(this, QObject::tr("Driver no instalado"),
                                                        QObject::tr("No se ha encontrado el driver para conectar a la database.\n"),
                                                        QMessageBox::Ok);
        qDebug() << "No se ha encontrado el driver para conectar a la database";
    }

    auto miConexion = std::unique_ptr <QSqlDatabase>(new QSqlDatabase);
    auto *miConexion = new QSqlDatabase;
    *miConexion = QSqlDatabase::addDatabase(comboList->value(0)); //static function
    miConexion->setDatabaseName(comboList->value(1));
    miConexion->setUserName(comboList->value(2));
    miConexion->setPassword(comboList->value(3));
    miConexion->setHostName(comboList->value(4));
    if(miConexion->open())
    {
        qDebug() << "DB opened successfully";
        qDebug() << miConexion->lastError();
        //Popup to close db connection
        dbConnectionForm::closeDatabaseDialog();
        return miConexion->lastError();
    }
    else
    {
        QMessageBox::warning(this,
                                                QObject::tr("No se ha conectado a la db"),
                                                QObject::tr("No se ha podido conectar a la database.\n"),
                                                QMessageBox::Ok);
        qDebug() << "DB fail";
        qDebug() << miConexion->lastError();
        return miConexion->lastError();
    }*/
}
void dbConnectionForm::closeDatabase(void)
{
    miConexion->close();
}
void dbConnectionForm::closeDatabaseDialog(void)
{
    //Pop-up to close db connection
    QMessageBox msgBoxCloseDb;
    msgBoxCloseDb.setWindowTitle(QObject::tr("Shut down db connection"));
    msgBoxCloseDb.setText(QObject::tr("Cerrar la conexión con la database"));
    msgBoxCloseDb.setWindowModality(Qt::NonModal);
    msgBoxCloseDb.setStandardButtons(QMessageBox::Ok);
    msgBoxCloseDb.exec();
    if(msgBoxCloseDb.result() == QMessageBox::Ok)
        dbConnectionForm::closeDatabase();
}
QString dbConnectionForm::getTypeComboActivated(void)
{return typeComboBox->currentText();}
QString dbConnectionForm::getDatabaseFieldText(void)
{return databaseLineEdit->text();}
QString dbConnectionForm::getUsernameFieldText(void)
{return usernameLineEdit->text();}
QString dbConnectionForm::getPasswordFieldText(void)
{return passwordLineEdit->text();}
QString dbConnectionForm::getServerFieldText(void)
{return serverLineEdit->text();}
