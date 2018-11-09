#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
{
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
}

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
    //Central widget
    mainWindowCentralWidget = new class centralWidget(this); //Elaborated type specifier
    this->setCentralWidget(mainWindowCentralWidget);
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
    fileMenu->addAction(QObject::tr("&Open"), this, &mainWindow::openFile, QKeySequence::Open);
    fileMenu->addAction(QObject::tr("&Save"), this, &mainWindow::openFile, QKeySequence::Save);
    fileMenu->addAction(QObject::tr("&Save as"), this, &mainWindow::openFile, QKeySequence::SaveAs);
    //Add submenus to main bar (EDIT)
    editMenu->addAction(QObject::tr("&Cut"), this, &mainWindow::openFile, QKeySequence::Cut);
    editMenu->addAction(QObject::tr("&Copy"), this, &mainWindow::openFile, QKeySequence::Copy);
    editMenu->addAction(QObject::tr("&Paste"), this, &mainWindow::openFile, QKeySequence::Paste);
    //Add submenus to main bar (HELP)
    helpMenu->addAction(QObject::tr("&Help"), this, &mainWindow::openFile, QKeySequence::HelpContents);
    helpMenu->addAction(QObject::tr("&Qt"), this, &mainWindow::openFile);
    //Show Ready within the status bar
    statusBar()->showMessage(tr("Ready"), 5000); //Created automatically for the MainWindow
    qDebug() << "Se han creado los Menus correctamente!";
}

void mainWindow::createToolsBars()
{
    standardToolBar = new QToolBar(this);
    editToolBar = new QToolBar(this);
    buttonOpen = new QToolButton(this);
    buttonClose = new QToolButton(this);
    buttonSave = new QToolButton(this);
    buttonCopy = new QToolButton(this);
    buttonCut = new QToolButton(this);
    buttonPaste = new QToolButton(this);

    QIcon openIcon(QIcon::fromTheme("document-open"));
    QIcon closeIcon(QIcon::fromTheme("window-close"));
    QIcon saveIcon(QIcon::fromTheme("document-save"));
    QIcon copyIcon(QIcon::fromTheme("edit-copy"));
    QIcon cutIcon(QIcon::fromTheme("edit-cut"));
    QIcon pasteIcon(QIcon::fromTheme("edit-paste"));

    standardToolBar->addAction(openIcon,"", this, &mainWindow::openFile);
    standardToolBar->addAction(closeIcon,"", this, &mainWindow::closeFile);
    standardToolBar->addAction(saveIcon,"", this, &mainWindow::saveFile);
    editToolBar->addAction(copyIcon,"", this, &mainWindow::saveFile);
    editToolBar->addAction(cutIcon,"", this, &mainWindow::saveFile);
    editToolBar->addAction(pasteIcon,"", this, &mainWindow::saveFile);

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
    auto *userSettings = new QSettings (QObject::tr("Fx Team®"), QObject::tr("Sogea"));
    userSettings->beginGroup("mainwindow");
    this->resize(userSettings->value("size").toSize());
    this->move(userSettings->value("position").toPoint());
    userSettings->value("fullScreen");
    userSettings->endGroup();
    return userSettings->status();
}

QSettings::Status mainWindow::writeUserSettings()
{
    //Store configuration before closing the application
    QSettings *userSettings = new QSettings (QObject::tr("Fx Team®"), QObject::tr("Sogea"));
    userSettings->beginGroup("mainwindow");
    userSettings->setValue("size", this->size()); //QSize
    userSettings->setValue("position", this->pos()); //QPoint
    userSettings->setValue("fullScreen", this->isFullScreen()); //bool
    userSettings->endGroup();
    return userSettings->status();
}

void mainWindow::closeEvent(QCloseEvent *event)
{
    this->writeUserSettings();
    event->accept();
}

void mainWindow::openFile()
{
    QMessageBox::warning(this, QObject::tr("ABRIR ARCHIVO"),
                                                    QObject::tr("ABRIRRRRRRRRR!"),
                                                    QMessageBox::Ok);

}

void mainWindow::closeFile()
{
    QMessageBox::warning(this, QObject::tr("ABRIR ARCHIVO"),
                                                    QObject::tr("ABRIRRRRRRRRR!"),
                                                    QMessageBox::Ok);

}

void mainWindow::saveFile()
{
    QMessageBox::warning(this, QObject::tr("ABRIR ARCHIVO"),
                                                    QObject::tr("ABRIRRRRRRRRR!"),
                                                    QMessageBox::Ok);

}

