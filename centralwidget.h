#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtWidgets>

class centralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit centralWidget(QWidget *parent = Q_NULLPTR);
    ~centralWidget() = default;
    centralWidget(const centralWidget&) = delete; //delete: borra las definiciones automáticas del compilador
    centralWidget& operator =(const centralWidget&) = delete; //también se podrian haber declarado private

signals:
public slots:
private:
    //members
    void setUpLayout(void);
    void setUpTreeWidget(void);
    //Layout and widgets
    QGridLayout *mainLayout = Q_NULLPTR;
    QHBoxLayout buttonsLayout; //NO lo creo con new pues no puedo anclarlo al widget mediante (this).
    QTreeWidget *dbField = Q_NULLPTR;
    QTableView *dbDataField = Q_NULLPTR;
    QPlainTextEdit *sqlField = Q_NULLPTR;
    QPushButton *aceptarButton = Q_NULLPTR;
    QPushButton *cancelarButton = Q_NULLPTR;
    QErrorMessage *errorMsgCentralWidget = Q_NULLPTR;
};

#endif // CENTRALWIDGET_H
