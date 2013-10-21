#include "widget/MatrixEditor.h"

#include <QPushButton>
#include <QVBoxLayout>

MatrixEditor::MatrixEditor(const Matrix& matrix, const int rows, const int cols, QWidget* parent)
  : QDialog(parent),
    m_matrix(matrix)
{
    QPushButton* okButton = new QPushButton(tr("Ok"), this);
    QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
    
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);
    
    setLayout(layout);
}
