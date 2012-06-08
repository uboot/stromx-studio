#include "ExceptionObserver.h"

ExceptionObserver::ExceptionObserver(ErrorListModel* model)
  : m_model(model)
{

}

void ExceptionObserver::observe(const stromx::core::ExceptionObserver::Phase phase,
                                const stromx::core::OperatorError& ex,
                                const stromx::core::Thread*const thread) const
{

}
