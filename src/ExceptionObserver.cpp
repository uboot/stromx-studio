#include "ExceptionObserver.h"

#include <QCoreApplication>
#include "ErrorData.h"
#include "ErrorEvent.h"

ExceptionObserver::ExceptionObserver(QObject* receiver)
  : m_receiver(receiver)
{

}

void ExceptionObserver::observe(const stromx::core::ExceptionObserver::Phase phase,
                                const stromx::core::OperatorError& ex,
                                const stromx::core::Thread*const thread) const
{
    ErrorData data(ex, ErrorData::Type(phase));
    sendErrorData(data);
}

void ExceptionObserver::sendErrorData(const ErrorData& data) const
{
    QEvent* event = new ErrorEvent(data);
    QCoreApplication::instance()->postEvent(m_receiver, event);
}
