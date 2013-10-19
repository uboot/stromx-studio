#include "widget/MatrixEditor.h"

MatrixEditor::MatrixEditor(const Matrix& matrix, const int rows, const int cols, QWidget* parent)
  : QDialog(parent),
    m_matrix(matrix)
{

}
