#include "delegate/EditMatrixButton.h"

#include "widget/MatrixEditor.h"

EditMatrixButton::EditMatrixButton(const Matrix & matrix, const int rows,
                                   const int cols, QWidget* parent)
  : QPushButton(parent),
    m_matrix(matrix),
    m_rows(rows),
    m_cols(cols)
{
    setText(tr("Edit..."));
    connect(this, SIGNAL(clicked(bool)), this, SLOT(openEditor()));
}

void EditMatrixButton::openEditor()
{
    MatrixEditor editor(m_matrix, m_rows, m_cols, this);
    if (editor.exec() == QDialog::Accepted)
        m_matrix = editor.matrix();
    
    emit finishedEditing();
}

const Matrix& EditMatrixButton::matrix()
{
    return m_matrix;
}

