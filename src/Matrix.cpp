#include "Matrix.h"

#include "Config.h"

const std::string Matrix::TYPE = "Matrix";
const std::string Matrix::PACKAGE = "stromx-studio";
const stromx::runtime::Version Matrix::VERSION 
    = stromx::runtime::Version(STROMX_STUDIO_VERSION_MAJOR, 
                               STROMX_STUDIO_VERSION_MINOR, 
                               STROMX_STUDIO_VERSION_PATCH);
    
Matrix::Matrix(const Matrix& matrix)
  : MatrixWrapper(),
    m_data(0)
{
    allocate(matrix.rows(), matrix.cols(), matrix.valueType());
    memcpy(m_data, matrix.m_data, bufferSize());
}

Matrix::Matrix(const unsigned int rows, const unsigned int cols, const Matrix::ValueType valueType)
  : m_data(0) 
{
    allocate(rows, cols, valueType);
}

const Matrix & Matrix::operator=(const Matrix& matrix)
{
    allocate(matrix.rows(), matrix.cols(), matrix.valueType());
    memcpy(m_data, matrix.m_data, bufferSize());
    
    return *this;
}

void Matrix::allocate(const unsigned int rows, const unsigned int cols, const ValueType valueType)
{
    if(m_data)
    {
        delete [] m_data;
        m_data = 0;
    }
    
    unsigned int valueSize = Matrix::valueSize(valueType);
    unsigned int bufferSize = rows * cols * valueSize;
    m_data = new uint8_t[bufferSize];
    
    setBuffer(m_data, bufferSize);
    initializeMatrix(rows, cols, cols * valueSize, m_data, valueType);
}
