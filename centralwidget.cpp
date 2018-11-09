#include "centralwidget.h"

/* CONSTRUCTOR */
centralWidget::centralWidget(QWidget *parent) : QWidget(parent)
{
    errorMsgCentralWidget = new QErrorMessage(this);
    this->setUpLayout();
    this->setUpTreeWidget();

    qDebug() << "Se ha llamado al constructor centralWidget()";
}

/* PRIVATE MEMBERS */
void centralWidget::setUpLayout(void)
{
    mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->setHorizontalSpacing(10);
    mainLayout->setVerticalSpacing(0); //Aun asi se añade un espacio entre rows

    dbField = new QTreeWidget(this);
    dbDataField = new QTableView(this);
    sqlField = new QPlainTextEdit(this);
    sqlField->setPlaceholderText("Indique su consulta SQL");
    dbField->setMaximumSize(300, 300);
    dbDataField->setMaximumSize(500, 300);
    sqlField->setMaximumSize(800, 150);

    buttonsLayout.setSpacing(10); //Distance between layout's items
    buttonsLayout.setStretch(1,0); //Aceptar button
    buttonsLayout.setStretch(2,0); //Cancelar button

    aceptarButton = new QPushButton(QObject::tr("Aceptar"), this);
    cancelarButton = new QPushButton(QObject::tr("Cancelar"), this);
    aceptarButton->setMaximumSize(80, 30);
    cancelarButton->setMaximumSize(80, 30);
    buttonsLayout.addWidget(aceptarButton);
    buttonsLayout.addWidget(cancelarButton);

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

    mainLayout->addWidget(dbField, 0, 0);
    mainLayout->addWidget(dbDataField, 0, 1);
    mainLayout->addWidget(sqlField, 1, 0, 1, 2);
    mainLayout->addLayout(&buttonsLayout, 2, 0, 1, 2);
    //MUY IMPORTANTE: el setAlignment() después de addLayout()
    buttonsLayout.setAlignment(Qt::AlignLeft | Qt::AlignTop);

    qDebug() << "Se ha llamado a setUpLayout()";
}
void centralWidget::setUpTreeWidget(void)
{
    dbField->show();
    QTreeWidgetItem myDatabase(dbField);
    myDatabase.setText(1, "my_db_schema");
    dbField->setColumnCount(1);
    dbField->setHeaderLabel(QObject::tr("database"));
    dbField->addTopLevelItem(&myDatabase);

    qDebug() << "Se ha llamado a setUpTreeWidget()";
}
