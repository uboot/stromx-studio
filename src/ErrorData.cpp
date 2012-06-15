#include "ErrorData.h"

#include <QApplication>
#include <stromx/core/OperatorException.h>
#include <stromx/core/OperatorInfo.h>

ErrorData::ErrorData(const stromx::core::OperatorError& exception, Type type)
  : m_time(QDateTime::currentDateTime()),
    m_type(type)
{
    QString opType = QString("%1/%2").arg(QString::fromStdString(exception.op().type()))
                                     .arg(QString::fromStdString(exception.op().package()));
                                     
    QString errorType;
    switch(type)
    {
    case INITIALIZATION:
        errorType = QApplication::tr("Initialization error");
        break;
    case ACTIVATION:
        errorType = QApplication::tr("Activation error");
        break;
    case EXECUTION:
        errorType = QApplication::tr("Execution error");
        break;
    case DEACTIVATION:
        errorType = QApplication::tr("Deactivation error");
        break;
    case DEINITIALIZATION:
        errorType = QApplication::tr("Deinitialization error");
        break;
    default:
        errorType = QApplication::tr("Unknown error");
        break;
    }
    
    QString opName = exception.name().empty() ? QString("") :
                     QApplication::tr("\"%1\" ").arg(QString::fromStdString(exception.name()));
    
    m_title = QApplication::tr("%1 in operator %2of type %3").arg(errorType).arg(opName).arg(opType);
    
    m_description = QString::fromStdString(exception.message());
}
