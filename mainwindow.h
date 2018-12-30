#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
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
//    void newFile(void);
    void openAction(void);
    void closeAction(void);
    void saveAction(void);
    void dbConnectionAction(void);
    void closeEvent(QCloseEvent *event) override;
    void previousPageAction(void);
    void nextpageAction(void);
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
    QSize minimumSizeHint(void) const override;
    QSizePolicy sizePolicy(void) const;
    void setUpWindows(void);
    void createMenus(void);
    void createToolsBars(void);
    void userScreenResolution(void);
    QSettings::Status readUserSettings(void);
    QSettings::Status writeUserSettings(void);
    void newPage(QWidget *widget);
    void previousPage(void);
    void nextPage(void);

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
    QErrorMessage *errorMsgMainwindow = Q_NULLPTR;
    typedef struct
    {
        QRect geometry;
        int count;
        bool virtualDesktop;
    }userScreen;
    int resolutionWidth{};
    int resolutionHeight{};
};

#endif // MAINWINDOW_H
