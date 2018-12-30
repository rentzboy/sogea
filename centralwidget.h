#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

class QTreeWidget;
class centralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit centralWidget(QWidget *parent = Q_NULLPTR);
    ~centralWidget() = default;
    centralWidget(const centralWidget&) = delete; //delete: borra las definiciones automáticas del compilador
    centralWidget& operator =(const centralWidget&) = delete; //también se podrian haber declarado private
    //public members
    void refreshTreeWidget(void);

signals:
    void itemDoubleClicked(QTreeWidgetItem *item, int column);
public slots:
    void on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_selection_currentRowChanged(const QModelIndex /*&current*/, const QModelIndex /*&previous*/);
    void on_aceptar_clicked(void);
    void on_cancelar_clicked(void);
    void add_tableRow(void);
    void delete_tableRow(void);
    void select_tableRow(void);
    void refresh_table(void);
    void show_metaData(void);

protected:
        void contextMenuEvent(QContextMenuEvent *event);
private:
    //private members
    void setUpLayout(void);
    void setUpTreeWidget(void);
    void setUpTableWidget(void);
    void refreshTableWidget(const QString &tableName);
    void switchBoldItem(QTreeWidgetItem *rootItem, bool bold);
    void setUpContextMenus(void);
    void executeSqlQuery(void);
    //Layout and widgets
    QTreeWidget *tree = Q_NULLPTR;
    QTreeWidgetItem *activeDB = Q_NULLPTR;

    QSqlRelationalTableModel *model = Q_NULLPTR;
    QSqlQueryModel *sqlModel = Q_NULLPTR;
    QTableView *table = Q_NULLPTR;
    QItemSelectionModel *selection = Q_NULLPTR;

    QSqlDatabase* db = Q_NULLPTR;
    QString selectedTable;
    QPlainTextEdit *textField = Q_NULLPTR;
    QPushButton *aceptarButton = Q_NULLPTR;
    QPushButton *cancelarButton = Q_NULLPTR;
    QErrorMessage *errorMsgCentralwidget = Q_NULLPTR;
    static const auto MAX_CONNECTIONS = 10;
    std::array<QString, MAX_CONNECTIONS>  connectionName;
};

#endif // CENTRALWIDGET_H
