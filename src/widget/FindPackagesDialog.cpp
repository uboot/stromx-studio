#include "widget/FindPackagesDialog.h"

#include <QListWidget>

FindPackagesDialog::FindPackagesDialog(QWidget* parent): QDialog(parent)
{
    QListWidget* listWidget = new QListWidget(this);
}

void FindPackagesDialog::setPackages(const QFileInfoList& packages)
{

}
