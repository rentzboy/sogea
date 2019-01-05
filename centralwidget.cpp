#include "centralwidget.h"
#include "mainwindow.h"
#include "excepciones.h"
#include "dbconnectionform.h"
#include "globals.h"

/* CONSTRUCTOR */
centralWidget::centralWidget(QWidget *parent) : QWidget(parent)
{
    db = static_cast<mainWindow*>(this->parent())->get_mainDb();
    errorMsgCentralwidget = new QErrorMessage(this);
    this->setUpLayout();
    this->setUpTreeWidget();
    this->setUpTableWidget();
    this->setUpContextMenus();

    //IMPORTANT: To create Signal & Slot autoconnections
    this->tree->setObjectName(QObject::tr("tree"));
    this->table->setObjectName(QObject::tr("table"));
    this->selection->setObjectName(QObject::tr("selection"));
    this->aceptarButton->setObjectName(QObject::tr("aceptar"));
    this->cancelarButton->setObjectName(QObject::tr("cancelar"));
    QMetaObject::connectSlotsByName(this);

    qDebug() << "Se ha llamado al constructor de centralWidget";
}

/* PRIVATE MEMBERS */
void centralWidget::setUpLayout(void)
{
    auto *mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->setHorizontalSpacing(10);
    mainLayout->setVerticalSpacing(0); //Aun asi se añade un espacio entre rows

    tree = new QTreeWidget(this);
    table = new QTableView(this);
    textField = new QPlainTextEdit(this);
    textField->setPlaceholderText("Indique su consulta SQL");
    tree->setMaximumSize(300, 300);
    table->setMaximumSize(500, 300);
    textField->setMaximumSize(800, 150);
    textField->setPlainText(QObject::tr("SELECT almacen.crop.id_crop, almacen.crop.id_estimation, almacen.products.id_product, almacen.products.id_family FROM almacen.crop JOIN almacen.products ON almacen.crop.id_crop = almacen.products.id_product"));

    auto *buttonsLayout = new QHBoxLayout(Q_NULLPTR);
    buttonsLayout->setSpacing(10); //Distance between layout's items
    buttonsLayout->setStretch(1,0); //Aceptar button
    buttonsLayout->setStretch(2,0); //Cancelar button

    aceptarButton = new QPushButton(QObject::tr("Aceptar"), this);
    cancelarButton = new QPushButton(QObject::tr("Cancelar"), this);
    aceptarButton->setMaximumSize(80, 30);
    cancelarButton->setMaximumSize(80, 30);
    buttonsLayout->addWidget(aceptarButton);
    buttonsLayout->addWidget(cancelarButton);

    //Rows
    mainLayout->setRowMinimumHeight(0, 200); //db &table items
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowMinimumHeight(1, 200); //sql_query item
    mainLayout->setRowStretch(1, 0);
    mainLayout->setRowMinimumHeight(2, 50); //QHBoxLayout
    mainLayout->setRowStretch(2, 0);

    //Columns
    mainLayout->setColumnMinimumWidth(0, 200); //db item
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnMinimumWidth(1, 500); //table item
    mainLayout->setColumnStretch(1, 10);

    mainLayout->addWidget(tree, 0, 0);
    mainLayout->addWidget(table, 0, 1);
    mainLayout->addWidget(textField, 1, 0, 1, 2);
    mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
    //MUY IMPORTANTE: el setAlignment() después de addLayout()
    buttonsLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}
void centralWidget::switchBoldItem(QTreeWidgetItem *rootItem, bool bold)
{
    QFont tmp = rootItem->font(0);
    tmp.setBold(bold);
    rootItem->setFont(0, tmp);
    qDebug() << "Se ha llamado a switchBoldItem!";
}
void centralWidget::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    QMenu menu(table);
    menu.addAction(QObject::tr("Add row"), this, &centralWidget::add_tableRow);
    menu.addAction(QObject::tr("Delete row"), this, &centralWidget::delete_tableRow);
    menu.addAction(QObject::tr("Select row"), this, &centralWidget::select_tableRow);
    menu.exec(event->globalPos());
}
void centralWidget::setUpContextMenus(void)
{
    QAction *refresh = new QAction("Refresh", tree);
    QAction *showMetaData = new QAction("Show Metadata", tree);

    tree->addAction(refresh);
    tree->addAction(showMetaData);
    tree->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(refresh, &QAction::triggered, this, &centralWidget::refresh_table);
    connect(showMetaData, &QAction::triggered, this, &centralWidget::show_metaData);

    qDebug() << "Se han creado los contextual menus de centralWidget";
}
void centralWidget::executeSqlQuery(void)
{
    sqlModel = new QSqlQueryModel(model);
    sqlModel->setQuery(textField->toPlainText(), db); //Importante, indicar la DB !!
    if (sqlModel->lastError().isValid())
    {
        errorMsgCentralwidget->showMessage(sqlModel->lastError().databaseText());
        return;
    }
    table->setModel(sqlModel);
    if (sqlModel->lastError().isValid())
    {
        errorMsgCentralwidget->showMessage(sqlModel->lastError().databaseText());
        return;
    }

    STATUSBAR_MESSAGE("Ready", 1500);
    qDebug() << "Se ha llamado a slot_aceptar_clicekd!";
}

/* PUBLIC MEMBERS */
void centralWidget::refreshTreeWidget(void)
{
    this->setUpTreeWidget();
    STATUSBAR_MESSAGE("Ready", 1500);
    qDebug() << "Se ha refrescado el TreeWidget!";
}
void centralWidget::setUpTreeWidget(void)
{
    tree->clear(); //Importante, no olvidar !!

    //Primer nivel (conexiones)
    QString tmpUser = db.userName();
    QString tmpName = db.databaseName();

    qDebug() << "Tablas de la DB: " << db.tables();

    auto *root = new QTreeWidgetItem(tree);
    root->setText(0, tmpUser.append('@').append(tmpName));

    //Segundo nivel (tablas para la conexión seleccionada)
    foreach(QString str, db.tables())
    {
        auto *child = new QTreeWidgetItem(root);
        child->setText(0, str);
    }
}
void centralWidget::setUpTableWidget(void)
{   
    //Construct SQL model
    model = new QSqlRelationalTableModel(this, db);
    model->setEditStrategy(QSqlRelationalTableModel::OnRowChange);
    table->setModel(model);
    table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::CurrentChanged);

    //Construct the Selection model
    selection = new QItemSelectionModel(model, this);
    table->setSelectionModel(selection); //Importante !!!

    //Enable Sorting using headers + call sortbycolumn()
    table->setSortingEnabled(true);
}
void centralWidget::refreshTableWidget(const QString &tableName)
{
    //Sets the database table on which the model operates
    model->setTable(tableName);
    if (model->lastError().isValid())
    {
        errorMsgCentralwidget->showMessage(sqlModel->lastError().databaseText());
        return;
    }

    //Populates the model with data from the table
    model->select();
    if (model->lastError().isValid())
    {
        errorMsgCentralwidget->showMessage(sqlModel->lastError().databaseText());
        return;
    }

    //Show table
    table->setModel(model);
    if (model->lastError().isValid())
    {
        errorMsgCentralwidget->showMessage(sqlModel->lastError().databaseText());
        return;
    }

    STATUSBAR_MESSAGE("Ready", 1500);
}

/* SLOTS */
void centralWidget::on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //Recuperar el nombre de la tabla seleccionada
    selectedTable = item->text(column);
    if(!selectedTable.contains('@', Qt::CaseInsensitive))
        this->refreshTableWidget(selectedTable);

    qDebug() << "Se ha llamado a slot_itemDoubleClicked!";
    if(activeDB == Q_NULLPTR)
    {
        activeDB = item;
        switchBoldItem(activeDB, true);
    }
    else if (item != activeDB)
    {
        switchBoldItem(item, true);
        switchBoldItem(activeDB, false);
        activeDB = item;
    }
    else return;
}
void centralWidget::on_selection_currentRowChanged(const QModelIndex /*&current*/, const QModelIndex /*&previous*/)
{
    qDebug() << "Se ha llamado a slot_currentRowChanged!";
}
void centralWidget::on_aceptar_clicked()
{
    this->executeSqlQuery();
}
void centralWidget::on_cancelar_clicked()
{
    textField->clear();
    STATUSBAR_MESSAGE("Ready", 1500);
    qDebug() << "Se ha llamado a slot_cancelar_clicekd!";
}
void centralWidget::add_tableRow(void)
{
    //Comprobación inicial
    auto *modelo = qobject_cast<QSqlTableModel *>(table->model());
    if (!modelo)
        return;

    //Recuperar la fila actual
    QModelIndex insertIndex = table->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row() ;
    qDebug() << "Fila seleccionada: " << row;

    //Insertar nueva fila en blanco
    model->insertRow(row);

    //Preparar la nueva fila para editarla
    insertIndex = model->index(row, 0);
    table->setCurrentIndex(insertIndex);
    table->edit(insertIndex);
}
void centralWidget::delete_tableRow(void)
{

}
void centralWidget::select_tableRow(void)
{


}
void centralWidget::refresh_table()
{
    errorMsgCentralwidget->showMessage("Se ha llamado a refresh_table");

}
void centralWidget::show_metaData()
{
    errorMsgCentralwidget->showMessage("Se ha llamado a show_metaData");
}
