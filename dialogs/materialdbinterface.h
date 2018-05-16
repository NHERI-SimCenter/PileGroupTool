#ifndef MATERIALDBINTERFACE_H
#define MATERIALDBINTERFACE_H

#include <QWidget>

namespace Ui {
class MaterialDBInterface;
}

class MaterialDBInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MaterialDBInterface(QWidget *parent = 0);
    ~MaterialDBInterface();

private slots:
    void on_cb_selectDB_activated(const QString &arg1);
    void on_lst_materialSelection_clicked(const QModelIndex &index);
    void on_pb_reset_clicked();
    void on_pb_commit_clicked();
    void on_lned_numProperties_returnPressed();

private:
    Ui::MaterialDBInterface *ui;
};

#endif // MATERIALDBINTERFACE_H
