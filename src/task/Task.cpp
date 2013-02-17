#include "Task.h"

#include <QFutureWatcher>
#include <QtConcurrentRun>

#ifdef __GNUG__
    #include <tr1/functional>
#else
    #include <functional>
#endif

using namespace stromx::runtime;

Task::Task(QObject* parent)
  : QObject(parent),
    m_watcher(new QFutureWatcher<void>(this))
{
    connect(m_watcher, SIGNAL(finished()), this, SLOT(handleFutureFinished()));
}

void Task::start()
{
    m_watcher->setFuture(QtConcurrent::run(std::tr1::bind(&runTask, this)));
}

Task::~Task()
{
    m_watcher->waitForFinished();
}

void Task::handleFutureFinished()
{
    emit finished();
    delete this;
}

void Task::runTask(Task* task)
{
    task->run();
}
