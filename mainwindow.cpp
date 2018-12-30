#include "mainwindow.h"
#include "dbconnectionform.h"
#include "centralwidget.h"

/* CONSTRUCTOR */
mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
{
    errorMsgMainwindow = new QErrorMessage(this);
    //Retrieve persistent data from last user's sesion
    if(this->readUserSettings())
    {
        qDebug() << "Ha habido un problema al leer el archivo de la última sesión!";
        //Get user's screen resolution
        this->userScreenResolution();

        //this->sizeHint(); //Debería llamarse automáticamente desde el constructor pero no entiendo cuando se aplica
        this->resize(resolutionWidth, resolutionHeight); //este si que no falla nunca
        this->sizePolicy(); //No parece que funcione.............
        //Al utilizar setGeometry se deja de llamar a sizeHint() automáticamente desde el constructor!
        //this->setGeometry(0,0, resolutionWidth, resolutionHeight);
    }

    //Set up Window (+ configuraciones)
    this->setUpWindows();

    statusBar()->showMessage(QObject::tr("Ready"), 1500);
}

/* PRIVATE MEMBERS */
void mainWindow::userScreenResolution()
{
    //Retrieve parameters from user's screen configuration
    QSharedPointer <userScreen> userScreenData = QSharedPointer<userScreen>(new userScreen);
    QDesktopWidget *rootWindow = QApplication::desktop();
    userScreenData->count = rootWindow->screenCount();
    userScreenData->virtualDesktop = rootWindow->isVirtualDesktop();
    userScreenData->geometry = rootWindow->availableGeometry(-1);
    qDebug() << userScreenData->geometry << ";" << userScreenData->count << ";" <<  userScreenData->virtualDesktop;
    resolutionWidth =userScreenData->geometry.width();
    resolutionHeight =userScreenData->geometry.height();
    qDebug() << "RESOLUTION => WIDTH: " << resolutionWidth << " - HEIGHT: " << resolutionHeight;
}
void mainWindow::setUpWindows()
{
    //Menus & Toolbars
    this->createMenus();
    this->createToolsBars();
}
void mainWindow::createMenus()
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
void mainWindow::createToolsBars()
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
QSize mainWindow::sizeHint() const
{
    //Devuelve el tamaño del widget (Se llama automáticamente desde el constructor)
    //También lo utiliza sizePolicy()
    qDebug() << "Se ha llamado a sizeHint()";
    qDebug() << "SIZEHINT => width: "<< resolutionWidth << " - height: " << resolutionHeight;
    return {resolutionWidth, resolutionHeight};
}
QSize mainWindow::minimumSizeHint() const
{
    //No impora pues los widgets que importan están en centralwidget
    //por lo que el minimumSizeHint se define allí
    qDebug() << "Se ha llamado a minimumSizeHint()";
    return {resolutionWidth/2, resolutionHeight/2};
}
QSizePolicy mainWindow::sizePolicy() const
{
    //Devuelve el resize policy del widget (NO se llama automáticamente desde el constructor)
    qDebug() << "Se ha llamado a sizePolicy()";
    return {QSizePolicy::Fixed, QSizePolicy::Fixed};
}
QSettings::Status mainWindow::readUserSettings()
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
QSettings::Status mainWindow::writeUserSettings()
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
void mainWindow::openAction() //PENDING
{
    QMessageBox::warning(this, QObject::tr("ABRIR ARCHIVO"),
                                                    QObject::tr("ABRIRRRRRRRRR!"),
                                                    QMessageBox::Ok);

}
void mainWindow::closeAction()
{
    this->close(); //send a QCloseEvent
}
void mainWindow::saveAction() //PENDING
{
    QMessageBox::warning(this, QObject::tr("ABRIR ARCHIVO"),
                                                    QObject::tr("ABRIRRRRRRRRR!"),
                                                    QMessageBox::Ok);

}
void mainWindow::dbConnectionAction(void)
{
    auto *dbConnectorWidget = new dbConnectionForm(this);
    if(dbConnectorWidget->exec() == QDialog::Accepted)
    {
        auto *mainCentralWidget = new class centralWidget(this);
        this->newPage(mainCentralWidget);
        qDebug() << "Tamaño del deConnectorWidget: ";
    }
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

/* PUBLIC MEMBERS */
