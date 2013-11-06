#include "widget/MatrixEditor.h"

#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>

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
    
    template <class T>
    void setValueAt(stromx::runtime::Matrix & matrix,
                    const int i, const int j, const T value)
    {
        switch(matrix.valueType())
        {
        case stromx::runtime::Matrix::INT_8:
            matrix.at<int8_t>(i, j) = value;
            break;
        case stromx::runtime::Matrix::UINT_8:
            matrix.at<uint8_t>(i, j) = value;
            break;
        case stromx::runtime::Matrix::INT_16:
            matrix.at<int16_t>(i, j) = value;
            break;
        case stromx::runtime::Matrix::UINT_16:
            matrix.at<uint16_t>(i, j) = value;
            break;
        case stromx::runtime::Matrix::INT_32:
            matrix.at<int32_t>(i, j) = value;
            break;
        case stromx::runtime::Matrix::UINT_32:
            matrix.at<uint32_t>(i, j) = value;
            break;
        case stromx::runtime::Matrix::FLOAT_32:
            matrix.at<float>(i, j) = value;
            break;
        case stromx::runtime::Matrix::FLOAT_64:
            matrix.at<double>(i, j) = value;
            break;
        default:
            ;
        }
    }
}

MatrixEditor::MatrixEditor(const Matrix& matrix, const int rows, const int cols, QWidget* parent)
  : QDialog(parent),
    m_matrix(matrix),
    m_table(0)
{
    QFormLayout* dimensions = new QFormLayout();
    
    QSpinBox* rowsSpinBox = new QSpinBox();
    rowsSpinBox->setMinimum(0);
    rowsSpinBox->setValue(matrix.rows());
    rowsSpinBox->setEnabled(rows == 0);
    rowsSpinBox->setSingleStep(1);
    connect(rowsSpinBox, SIGNAL(valueChanged(int)), SLOT(handleRowsChanged(int)));
    dimensions->addRow(tr("Rows:"), rowsSpinBox);
    
    QSpinBox* colsSpinBox = new QSpinBox();
    colsSpinBox->setMinimum(0);
    colsSpinBox->setValue(matrix.cols());
    colsSpinBox->setEnabled(cols == 0);
    colsSpinBox->setSingleStep(1);
    connect(colsSpinBox, SIGNAL(valueChanged(int)), SLOT(handleColsChanged(int)));
    dimensions->addRow(tr("Columns:"), colsSpinBox);
    
    m_table = new QTableWidget(this);
    fillTable();
    connect(m_table, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(checkItemContent(QTableWidgetItem*)));
    
    QPushButton* okButton = new QPushButton(tr("Ok"), this);
    QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
    
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    
    QVBoxLayout* main = new QVBoxLayout();
    main->addLayout(dimensions);
    main->addWidget(m_table);
    
    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(cancelButton);
    buttons->addWidget(okButton);
    main->addLayout(buttons);
    
    setLayout(main);
    
    connect(this, SIGNAL(accepted()), this, SLOT(readTable()));
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

void MatrixEditor::readTable()
{
    m_matrix.resize(m_table->rowCount(), m_table->columnCount(), m_matrix.valueType());
    
    for (unsigned int i = 0; i < m_matrix.rows(); ++i)
    {
        for (unsigned int j = 0; j < m_matrix.cols(); ++j)
        {
            QVariant data = m_table->item(i, j)->data(Qt::EditRole);
            setValueAt(m_matrix, i, j, data.toDouble());
        }
    }  
}

void MatrixEditor::handleRowsChanged(const int rows)
{
    int prevRows = m_table->rowCount();
    m_table->setRowCount(rows);
    
    for (int i = prevRows; i < rows; ++i)
    {
        for (int j = 0; j < m_table->columnCount(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("0"));
            m_table->setItem(i, j, item);
        }
    }
}

void MatrixEditor::handleColsChanged(const int cols)
{
    int prevCols = m_table->columnCount();
    m_table->setColumnCount(cols);
    
    for (int i = 0; i < m_table->rowCount(); ++i)
    {
        for (int j = prevCols; j < cols; ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("0"));
            m_table->setItem(i, j, item);
        }
    }
}

void MatrixEditor::checkItemContent(QTableWidgetItem* item)
{
    QVariant data = item->data(Qt::EditRole);
    
    item->setData(Qt::EditRole, data.toDouble());
    item->setData(Qt::DisplayRole, data.toDouble());
}


