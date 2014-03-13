#include "widget/FindPackagesDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>

FindPackagesDialog::FindPackagesDialog(QWidget* parent)
  : QDialog(parent),
    m_packageListWidget(0),
    m_showOnStartupCheckbox(0)
{
    m_packageListWidget = new QListWidget(this);
    m_packageListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_showOnStartupCheckbox = new QCheckBox(tr("Show dialog on startup"), this);
    QLabel* label = new QLabel(this);
    label->setText(tr("\
The packages below were found on your system. Select packages and click \
<b>Load</b> to load the selected packages."
    ));
    QPushButton* selectAllButton = new QPushButton(tr("Select all"), this);
    connect(selectAllButton, SIGNAL(clicked(bool)), this, SLOT(selectAll()));
    QPushButton* selectNoneButton = new QPushButton(tr("Select none"), this);
    connect(selectNoneButton, SIGNAL(clicked(bool)),
            m_packageListWidget->selectionModel(), SLOT(clearSelection()));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    QPushButton* loadButton = new QPushButton(tr("Load"), this);
    connect(loadButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    loadButton->setDefault(true);
    
    QHBoxLayout* labelLayout = new QHBoxLayout;
    labelLayout->addWidget(label, 1);
    labelLayout->addWidget(selectAllButton);
    labelLayout->addWidget(selectNoneButton);
    
    QHBoxLayout* dialogButtonLayout = new QHBoxLayout;
    dialogButtonLayout->addWidget(m_showOnStartupCheckbox, 1);
    dialogButtonLayout->addWidget(cancelButton);
    dialogButtonLayout->addWidget(loadButton);
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(labelLayout);
    mainLayout->addWidget(m_packageListWidget);
    mainLayout->addLayout(dialogButtonLayout);
    
    setLayout(mainLayout);
}

void FindPackagesDialog::setPackages(const QFileInfoList& packages)
{
    m_packages = packages;
    
    QStringList packagePaths;
    foreach (const QFileInfo & info, packages)
        packagePaths.push_back(info.absoluteFilePath());
    
    m_packageListWidget->clear();
    m_packageListWidget->insertItems(0, packagePaths);
}

void FindPackagesDialog::setShowOnStartup(const bool value)
{
    m_showOnStartupCheckbox->setChecked(value);
}

bool FindPackagesDialog::showOnStartup() const
{
    return m_showOnStartupCheckbox->isChecked();
}

void FindPackagesDialog::selectAll()
{
    int numPackages = m_packages.count();
    if (numPackages == 0)
        return;
    
    for (int i = 0; i < numPackages; ++i)
    {
        QModelIndex index = m_packageListWidget->model()->index(i, 0);
        m_packageListWidget->selectionModel()->select(index,
                                                      QItemSelectionModel::Select);
    }
}

const QFileInfoList FindPackagesDialog::selectedPackages() const
{
    QModelIndexList indices = m_packageListWidget->selectionModel()->selectedRows();
    
    QFileInfoList packages;
    foreach (const QModelIndex & index, indices)
        packages.append(m_packages[index.row()]);
    
    return packages;
}

