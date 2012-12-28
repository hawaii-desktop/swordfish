#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include <QModelIndexList>

class QFileSystemModel;
class VJob;

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InfoDialog(QWidget *parent = 0);
    ~InfoDialog();

    void setModel(QFileSystemModel *model);
    void setModelIndexList(const QModelIndexList &list);
    
private:
    Ui::InfoDialog *ui;
    QFileSystemModel *m_model;

    void setupForOneItem(const QModelIndex &index);
    void setupForMultipleItems(const QModelIndexList &list);

private slots:
    void directorySizeCalculated(VJob *job);
};

#endif // INFODIALOG_H
