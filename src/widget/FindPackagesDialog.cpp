#include "widget/FindPackagesDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>

FindPackagesDialog::FindPackagesDialog(QWidget* parent): QDialog(parent)
{
    QLabel* label = new QLabel(this);
    label->setText(tr("\
The packages below were found on your system. Select packages and click \
<b>Load</b> to load the selected packages."
    ));
    QPushButton* selectAllButton = new QPushButton(tr("Select all"), this);
    QPushButton* selectNoneButton = new QPushButton(tr("Select none"), this);
    QListWidget* listWidget = new QListWidget(this);
    QCheckBox* showOnStartupCheckbox = new QCheckBox(tr("Show dialog on startup"), this);;
    QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
    QPushButton* loadButton = new QPushButton(tr("Load"), this);
    okButton->setDefault(true);
    
    QHBoxLayout* labelLayout = new QHBoxLayout;
    labelLayout->addWidget(label, 1);
    labelLayout->addWidget(selectAllButton);
    labelLayout->addWidget(selectNoneButton);
    
    QHBoxLayout* dialogButtonLayout = new QHBoxLayout;
    dialogButtonLayout->addWidget(showOnStartupCheckbox, 1);
    dialogButtonLayout->addWidget(cancelButton);
    dialogButtonLayout->addWidget(loadButton);
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(labelLayout);
    mainLayout->addWidget(listWidget);
    mainLayout->addLayout(dialogButtonLayout);
    
    setLayout(mainLayout);
}

void FindPackagesDialog::setPackages(const QFileInfoList& packages)
{
    m_packages = packages;
}
