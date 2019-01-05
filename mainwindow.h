#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include<QSqlDatabase>
class centralWidget; //FORWARD DECLARATION
class dbConnectionForm;

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = Q_NULLPTR);
    ~mainWindow() override = default;
    mainWindow(const mainWindow&) = delete; //delete: borra las definiciones automáticas del compilador
    mainWindow& operator =(const mainWindow&) = delete; //también se podrian haber declarado private

public slots:
    void openAction(void) const;
    void closeAction(void);
    void saveAction(void) const;
    void dbConnectionAction(void);
    void closeEvent(QCloseEvent *event) override;
    void previousPageAction(void);
    void nextpageAction(void);
    bool createNewDbConnection(void);
    void closeDatabaseDialog(void);
    void importDbConnectionDetails(void);
    QSqlDatabase  get_mainDb(void) const;
//    void newFile(void);
//    bool saveFile(const QString &fileName);
//    bool saveAsFile(void);
//    void exitApp(void);
//    static void showAboutMsg(void);
//    void documentWasModified(void);
//    void commitData(QSessionManager &manager);

signals:

private:
    //Members
    QSize sizeHint(void) const override;
    QSizePolicy sizePolicy(void) const;
    QSize minimumSizeHint(void) const override;
    void getScreenResolution(void);
    void setUpWindow(void);
    void createMenus(void);
    void createToolsBars(void);
    QSettings::Status readUserSettings(void);
    QSettings::Status writeUserSettings(void);
    void newPage(QWidget *widget);
    void previousPage(void);
    void nextPage(void);

    //Database
    QSqlDatabase mainDb;
    //Menu bar & Menus
    QMenuBar *mainBar = Q_NULLPTR;
    QMenu *fileMenu = Q_NULLPTR;
    QMenu *editMenu = Q_NULLPTR;
    QMenu *helpMenu = Q_NULLPTR;
    //ToolBar & Buttons
    QToolBar *standardToolBar = Q_NULLPTR;
    QToolBar *editToolBar = Q_NULLPTR;
    //Central widget
    QList<QWidget*> breadCrumbling;
    uint16_t breadCrumblingIndex = 0;
    //Others
    QMap<QString, QString> DbDetails;
    QErrorMessage *errorMsgMainwindow = Q_NULLPTR;
    //Screen resolution
    int resolutionWidth = 1280;
    int resolutionHeight = 1024;
};

#endif // MAINWINDOW_H
