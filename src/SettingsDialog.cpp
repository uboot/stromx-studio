#include "SettingsDialog.h"

#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <boost/concept_check.hpp>

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent)
{
    QGroupBox* box = 0;
    QLabel* label = 0;
    QLabel* documentation = 0;
    QSpinBox* spinBox = 0;
    QVBoxLayout* groupBoxLayout = 0;
    QHBoxLayout* editLayout = 0;
    
    
    QVBoxLayout* dialogLayout = new QVBoxLayout;
    
    box = new QGroupBox(tr("Stream access"));
    label = new QLabel(tr("Time out (ms)"));
    documentation = new QLabel(tr("\
Operations which read or write a parameter of an operator \
can block some time if the stream is active and under heavy load. \
However, if either the stream design or the one of the operators is \
faulty, the operation can be blocked forever.\n\
When accessing a parameter stroms-studio waits a pre-defined amount of time \
after which it decides that the latter is the case. This time is defined here. \
The same value is valid when stromx-studio reads the data which is directed to \
one of the observer windows."));
    documentation->setWordWrap(true);
    spinBox = new QSpinBox();
    
    groupBoxLayout = new QVBoxLayout;
    editLayout = new QHBoxLayout;
    editLayout->addWidget(label);
    editLayout->addWidget(spinBox);
    groupBoxLayout->addItem(editLayout);
    groupBoxLayout->addWidget(documentation);
    box->setLayout(groupBoxLayout);
    dialogLayout->addWidget(box);
    
    box = new QGroupBox(tr("Slow processing"));
    label = new QLabel(tr("Delay (ms)"));
    documentation = new QLabel(tr("\
Slow processing refers to a mode where the stream delays the execution for a \
pre-defined amount of time each time it sent data to an operator input. \
The improves the visualization of the data for streams which process data \
very fast. The length of the delay is defined here."));
    documentation->setWordWrap(true);
    spinBox = new QSpinBox();
    
    groupBoxLayout = new QVBoxLayout;
    editLayout = new QHBoxLayout;
    editLayout->addWidget(label);
    editLayout->addWidget(spinBox);
    groupBoxLayout->addItem(editLayout);
    groupBoxLayout->addWidget(documentation);
    box->setLayout(groupBoxLayout);
    dialogLayout->addWidget(box);
    
    setLayout(dialogLayout);
}


void SettingsDialog::setModel(StreamModel* stream)
{
    m_stream = stream;
}

