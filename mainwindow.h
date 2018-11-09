#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "centralwidget.h"

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
    void openFile(void);
    void closeFile(void);
    void saveFile(void);
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
    void closeEvent(QCloseEvent *event) override;
    QSizePolicy sizePolicy(void) const;
    void setUpWindows(void);
    void createMenus(void);
    void createToolsBars(void);
    void userScreenResolution(void);
    QSettings::Status readUserSettings(void);
    QSettings::Status writeUserSettings(void);
    //Settings
    QSettings *userSettings = Q_NULLPTR;
    //Menu bar & Menus
    QMenuBar *mainBar = Q_NULLPTR;
    QMenu *fileMenu = Q_NULLPTR;
    QMenu *editMenu = Q_NULLPTR;
    QMenu *helpMenu = Q_NULLPTR;
    //ToolBar & Buttons
    QToolBar *standardToolBar = Q_NULLPTR;
    QToolBar *editToolBar = Q_NULLPTR;
    QToolButton *buttonOpen = Q_NULLPTR;
    QToolButton *buttonClose = Q_NULLPTR;
    QToolButton *buttonSave = Q_NULLPTR;
    QToolButton *buttonCopy = Q_NULLPTR;
    QToolButton *buttonCut = Q_NULLPTR;
    QToolButton *buttonPaste = Q_NULLPTR;
    //Dock Widgets
    //Central widget
    class centralWidget *mainWindowCentralWidget = Q_NULLPTR;
    //Others
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
