#include "delegate/EditMatrixButton.h"

#include "Common.h"
#include "widget/MatrixEditor.h"

EditMatrixButton::EditMatrixButton(const int rows, const int cols,
                                   const QModelIndex & index, QWidget* parent)
  : QPushButton(parent),
    m_index(index),
    m_rows(rows),
    m_cols(cols)
{
    setText(tr("Edit..."));
    connect(this, SIGNAL(clicked(bool)), this, SLOT(openEditor()));
}

void EditMatrixButton::openEditor()
{
    // update the data from the model
    QVariant data = m_index.data(MatrixRole);
    if(! data.canConvert<Matrix>())
        return;
    
    Matrix matrix = data.value<Matrix>();
    MatrixEditor editor(matrix, m_rows, m_cols, this);
    
    if (editor.exec() == QDialog::Accepted)
    {
        m_matrix = editor.matrix();
        emit editedMatrix();
    }
}

const Matrix& EditMatrixButton::matrix() const
{
    return m_matrix;
}

void EditMatrixButton::setMatrix(const Matrix& matrix)
{
    m_matrix = matrix;
}

