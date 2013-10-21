#include "widget/MatrixEditor.h"

#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace
{
    template <class T>
    T valueAt(const stromx::runtime::Matrix & matrix, const int i, const int j)
    {
        switch(matrix.valueType())
        {
        case stromx::runtime::Matrix::INT_8:
            return matrix.at<int8_t>(i, j);
        case stromx::runtime::Matrix::UINT_8:
            return matrix.at<uint8_t>(i, j);
        case stromx::runtime::Matrix::INT_16:
            return matrix.at<int16_t>(i, j);
        case stromx::runtime::Matrix::UINT_16:
            return matrix.at<uint16_t>(i, j);
        case stromx::runtime::Matrix::INT_32:
            return matrix.at<int32_t>(i, j);
        case stromx::runtime::Matrix::UINT_32:
            return matrix.at<uint32_t>(i, j);
        case stromx::runtime::Matrix::FLOAT_32:
            return matrix.at<float>(i, j);
        case stromx::runtime::Matrix::FLOAT_64:
            return matrix.at<double>(i, j);
        case stromx::runtime::Matrix::NONE:
            return 0.0;
        }
        
        return 0.0;
    }
}

MatrixEditor::MatrixEditor(const Matrix& matrix, const int rows, const int cols, QWidget* parent)
  : QDialog(parent),
    m_matrix(matrix)
{
    m_table = new QTableWidget(this);
    fillTable();
    
    QPushButton* okButton = new QPushButton(tr("Ok"), this);
    QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
    
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    
    QVBoxLayout* main = new QVBoxLayout();
    main->addWidget(m_table);
    
    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(cancelButton);
    buttons->addWidget(okButton);
    main->addLayout(buttons);
    
    setLayout(main);
}

void MatrixEditor::fillTable()
{
    m_table->setRowCount(m_matrix.rows());
    m_table->setColumnCount(m_matrix.cols());
    
    for (unsigned int i = 0; i < m_matrix.rows(); ++i)
    {
        for (unsigned int j = 0; j < m_matrix.cols(); ++j)
        {
            double value = valueAt<double>(m_matrix, i, j);
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(value));
            m_table->setItem(i, j, item);
        }
    }
    
}
