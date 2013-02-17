#include "ExceptionObserver.h"

#include <QCoreApplication>
#include "ErrorData.h"
#include "event/ErrorEvent.h"

ExceptionObserver::ExceptionObserver(QObject* receiver)
  : m_receiver(receiver)
{

}

void ExceptionObserver::observe(const stromx::runtime::ExceptionObserver::Phase phase,
                                const stromx::runtime::OperatorError& ex,
                                const stromx::runtime::Thread* /*thread*/) const
{
    ErrorData data(ex, ErrorData::Type(phase));
    sendErrorData(data);
}

void ExceptionObserver::sendErrorData(const ErrorData& data) const
{
    QEvent* event = new ErrorEvent(data);
    QCoreApplication::instance()->postEvent(m_receiver, event);
}
