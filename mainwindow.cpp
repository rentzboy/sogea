#include "mainwindow.h"
#include "dbconnectionform.h"
#include "centralwidget.h"
#include "globals.h"
#include "excepciones.h"

/* CONSTRUCTOR */
mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
{
    //Retrieve persistent data from last user's sesion
    if(this->readUserSettings())
    {
        qDebug() << "Ha habido un problema al leer el archivo de la última sesión!";
        //Get user's screen resolution
        this->getScreenResolution();

        //OPCION #1: Ni sizeHint() ni sizePolicy() hacen nada .....
        this->sizeHint(); //Se llama automáticamente desde el ctror pero ni llamandolo directamente funciona....
        this->sizePolicy(); //managed by the QLayout of this widget's children (no se llama automat. desde el ctror)
        //OPCION #2:
        this->resize(resolutionWidth, resolutionHeight); //este si que no falla
        //OPCION #3:
        //this->setGeometry(0,0, resolutionWidth, resolutionHeight);
        //Al utilizar setGeometry se deja de llamar a sizeHint() automáticamente desde el constructor!
    }

    //Set up Window (+ configuraciones)
    this->setUpWindow();

    errorMsgMainwindow = new QErrorMessage(this);
    statusBar()->showMessage(QObject::tr("Ready"), 1500);
}

/* PRIVATE MEMBERS */
QSize mainWindow::sizeHint(void) const //override
{
    //Devuelve el tamaño "recomendado" del widget (Se llama automáticamente desde el constructor)
    //También lo utiliza sizePolicy()
    qDebug() << "Se ha llamado a sizeHint()";
    qDebug() << "Resolution.width = " << resolutionWidth << "Resolution.height = " << resolutionHeight;
    return {resolutionWidth, resolutionHeight};
}
QSizePolicy mainWindow::sizePolicy(void) const //override
{
    //Devuelve el resize policy del widget (NO se llama automáticamente desde el constructor)
    qDebug() << "Se ha llamado a sizePolicy()";
    return {QSizePolicy::Fixed, QSizePolicy::Fixed};
}
QSize mainWindow::minimumSizeHint(void) const //override
{
    //No impora pues los widgets que importan están en centralwidget
    //por lo que el minimumSizeHint se define allí
    qDebug() << "Se ha llamado a minimumSizeHint()";
    return {resolutionWidth/2, resolutionHeight/2};
}
void mainWindow::getScreenResolution(void)
{
    //Retrieve parameters from user's screen configuration
    QDesktopWidget *rootWindow = QApplication::desktop();
    if(rootWindow->screenCount() == 1 && rootWindow->isVirtualDesktop() == false)
    {
        resolutionWidth =rootWindow->availableGeometry(-1).width();
        resolutionHeight =rootWindow->availableGeometry(-1).height();
    }
}
void mainWindow::setUpWindow(void)
{
    //Menus & Toolbars
    this->createMenus();
    this->createToolsBars();
}
void mainWindow::createMenus(void)
{
    //Create window's main bar
    mainBar = new QMenuBar(this);
    //Create main bar's menu
    fileMenu = new QMenu(QObject::tr("&File"), this);
    editMenu = new QMenu(QObject::tr("&Edit"), this);
    helpMenu = new QMenu(QObject::tr("&Help"), this);
    //Add menus to main bar
    mainBar->addMenu(fileMenu);
    mainBar->addMenu(editMenu);
    mainBar->addMenu(helpMenu);
    //Add actions to main bar (FILE)
    fileMenu->addAction(QObject::tr("&Open"), this, &mainWindow::openAction, QKeySequence::Open);
    fileMenu->addAction(QObject::tr("&ConnectDB"), this, &mainWindow::dbConnectionAction);
    fileMenu->addAction(QObject::tr("&Save"), this, &mainWindow::openAction, QKeySequence::Save);
    fileMenu->addAction(QObject::tr("&Save as"), this, &mainWindow::openAction, QKeySequence::SaveAs);
    fileMenu->addAction(QObject::tr("&Close"), this, &mainWindow::closeAction, QKeySequence::Close);
    //Add submenus to main bar (EDIT)
    editMenu->addAction(QObject::tr("&Cut"), this, &mainWindow::openAction, QKeySequence::Cut);
    editMenu->addAction(QObject::tr("&Copy"), this, &mainWindow::openAction, QKeySequence::Copy);
    editMenu->addAction(QObject::tr("&Paste"), this, &mainWindow::openAction, QKeySequence::Paste);
    //Add submenus to main bar (HELP)
    helpMenu->addAction(QObject::tr("&Help"), this, &mainWindow::openAction, QKeySequence::HelpContents);
    helpMenu->addAction(QObject::tr("&Qt"), this, &mainWindow::openAction);

    qDebug() << "Se han creado los Menus correctamente!";
}
void mainWindow::createToolsBars(void)
{
    standardToolBar = new QToolBar(this);
    editToolBar = new QToolBar(this);

    QIcon openIcon(QIcon::fromTheme("document-open"));
    QIcon closeIcon(QIcon::fromTheme("window-close"));
    QIcon saveIcon(QIcon::fromTheme("document-save"));
    QIcon copyIcon(QIcon::fromTheme("edit-copy"));
    QIcon cutIcon(QIcon::fromTheme("edit-cut"));
    QIcon pasteIcon(QIcon::fromTheme("edit-paste"));
    QIcon lastIcon(QIcon::fromTheme("go-last"));
    QIcon nextIcon(QIcon::fromTheme("go-next"));

    standardToolBar->addAction(openIcon,"", this, &mainWindow::openAction);
    standardToolBar->addAction(closeIcon,"", this, &mainWindow::closeAction);
    standardToolBar->addAction(saveIcon,"", this, &mainWindow::saveAction);
    standardToolBar->addAction(lastIcon,"", this, &mainWindow::previousPageAction);
    standardToolBar->addAction(nextIcon,"", this, &mainWindow::nextpageAction);

    editToolBar->addAction(copyIcon,"", this, &mainWindow::saveAction);
    editToolBar->addAction(cutIcon,"", this, &mainWindow::saveAction);
    editToolBar->addAction(pasteIcon,"", this, &mainWindow::saveAction);

    standardToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    editToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //Create space for ToolBar (IMPORTANTE !!)
    addToolBar(Qt::TopToolBarArea, standardToolBar);
    addToolBar(Qt::TopToolBarArea, editToolBar);
    //addToolBarBreak(Qt::TopToolBarArea); //crea un nuevo espacio horizontal por debajo para otras toolbars
}
QSettings::Status mainWindow::readUserSettings(void)
{
    //Retrieve configuration from the last user's session
    QSettings userSettings(QObject::tr("Fx Team®"), QObject::tr("Sogea"));
    userSettings.beginGroup("mainwindow");
    this->resize(userSettings.value("size").toSize());
    this->move(userSettings.value("position").toPoint());
    userSettings.value("fullScreen");
    userSettings.endGroup();
    return userSettings.status();
}
QSettings::Status mainWindow::writeUserSettings(void)
{
    //Store configuration before closing the application
    QSettings userSettings(QObject::tr("Fx Team®"), QObject::tr("Sogea"));
    userSettings.beginGroup("mainwindow");
    userSettings.setValue("size", this->size()); //QSize
    userSettings.setValue("position", this->pos()); //QPoint
    userSettings.setValue("fullScreen", this->isFullScreen()); //bool
    userSettings.endGroup();
    return userSettings.status();
}
void mainWindow::closeEvent(QCloseEvent *event) //ACTIVAR CODE EN PRODUCCIÓN
{
    event->accept();

    //COMENTAMOS EL RESTO PARA TEMAS DE DESARROLLO, PERO ESTA BIEN
    //PARA PRODUCCIÓN HABRIA QUE ACTIVARLO !!
//    QMessageBox msgBox;
//     msgBox.setText("The document has been modified.");
//     msgBox.setInformativeText("Do you want to save your changes?");
//     msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//     msgBox.setDefaultButton(QMessageBox::Save);
//     int ret = msgBox.exec();

//     switch (ret) {
//         case QMessageBox::Save:
//             this->saveAction();
//             this->writeUserSettings();
//             event->accept();
//             break;
//         case QMessageBox::Discard:
//             event->accept();
//             this->writeUserSettings();
//             break;
//         case QMessageBox::Cancel:
//            event->ignore();
//     }
}
void mainWindow:: newPage(QWidget *widget) //NO PROBADO
{
    if(breadCrumblingIndex == breadCrumbling.count())
        breadCrumbling.append(widget);
    else
        breadCrumbling.insert(breadCrumblingIndex, widget);

     breadCrumblingIndex++;

     this->setCentralWidget(widget);
}
void mainWindow:: previousPage(void) //NO PROBADO
{
    if(--breadCrumblingIndex)
        this->setCentralWidget(breadCrumbling[breadCrumblingIndex]);
    else return this->setCentralWidget(Q_NULLPTR);
}
void mainWindow:: nextPage(void) //NO PROBADO
{
    if(breadCrumblingIndex != breadCrumbling.count())
        this->setCentralWidget(breadCrumbling[++breadCrumblingIndex]);
}

/* PUBLIC SLOT MEMBERS */
void mainWindow::openAction(void) const//PENDING
{
    errorMsgMainwindow->showMessage("Pendiente codificar");

}
void mainWindow::closeAction(void)
{
    this->close(); //send a QCloseEvent
}
void mainWindow::saveAction(void) const //PENDING
{
    errorMsgMainwindow->showMessage("Pendiente codificar");
}
void mainWindow::previousPageAction(void)
{
    this->previousPage();
    qDebug() << "Se ha llamado a lastAction!";
}
void mainWindow::nextpageAction(void)
{
    this->nextPage();
    qDebug() << "Se ha llamado a nextAction!";
}
void mainWindow::dbConnectionAction(void)
{
    auto *dbConnectorWidget = new dbConnectionForm(this);
    bool repeat = true;
    do
    {
        if(dbConnectorWidget->exec() == QDialog::Accepted)
        {
            if(this->createNewDbConnection() == EXIT_SUCCESS)
            {
                dbConnectorWidget->set_AlertVisible(false);
                auto *mainCentralWidget = new class centralWidget(this);
                this->newPage(mainCentralWidget);
                repeat = false;
            }
            else
            {
                //Trabajamos en el objecto creado anteriormente pues por defecto
                //el objeto NO se destruye al cerrarlo object.setAttribute(Qt::WA_DeleteOnClose)
                //Pintar que ha habido un fallo en el formulario
                dbConnectorWidget->set_AlertVisible(true);
            }
        }
        else //QDialog::Canceled
            repeat = false;
    }while(repeat);
}
bool mainWindow::createNewDbConnection(void)
{
    try
    {
        //Checking installed db plugins
        QStringList installedDrivers = QSqlDatabase::drivers(); //static function
        if(!installedDrivers.contains(DbDetails["databaseType"]))
            throw(QObject::tr("Driver necesario para conectar a la Db no instalado")); //EXIT_FAILURE

        /* IMPORTANTE: No se puede utilizar el puntero "this" pues addDatabase() [static]
            No se puede crear un objeto QSqlDatabase y luego añadirle la database (NO funciona)
            La única manera es hacerlo tal como se indica aqui */

        //Open connection to db
        QString connectionName;
        connectionName.append(DbDetails["databaseType"]);
        connectionName.append("_").append(DbDetails["database"]);

        mainDb = QSqlDatabase::addDatabase(DbDetails["databaseType"], connectionName);
        mainDb.setDatabaseName(DbDetails["database"]);
        mainDb.setUserName(DbDetails["username"]);
        mainDb.setPassword(DbDetails["password"]);
        mainDb.setHostName(DbDetails["serverUrl"]);
        if(!mainDb.open())
             throw(mainDb.lastError()); //EXIT_FAILURE
        
        qDebug() << QObject::tr("DB connection ") <<  mainDb.connectionName()
                        << QObject::tr(" created.");
        qDebug() << QObject::tr("DB ") <<  mainDb.databaseName()
                        << QObject::tr(" opened successfully!");
        return EXIT_SUCCESS;
    }
    catch (const QSqlError &e)
    {
        EXCEPTION_HANDLER
        return EXIT_FAILURE;
    }
    catch (const QString &e)
    {
        EXCEPTION_HANDLER
        return EXIT_FAILURE;
    }
}
void mainWindow::closeDatabaseDialog(void) //SIN TERMINAR -NO VA AQUI !!
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
        errorMsgMainwindow->showMessage(QObject::tr("Se ha cerrado la conexión con la Db"));
        break;
    case QMessageBox::Cancel:
        break;
    }
}
void mainWindow::importDbConnectionDetails(void)
{
    DbDetails = static_cast<dbConnectionForm*>(QObject::sender())->get_DbConnectionDetails();
}
QSqlDatabase mainWindow::get_mainDb() const
{
    return mainDb;
}
/* PUBLIC MEMBERS */
