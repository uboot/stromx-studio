#include "model/ThreadModel.h"

#include <stromx/runtime/Thread.h>
#include "Common.h"
#include "cmd/RenameThreadCmd.h"
#include "cmd/SetThreadColorCmd.h"
#include "model/StreamModel.h"

ThreadModel::ThreadModel(stromx::runtime::Thread* thread, StreamModel* stream)
  : QObject(stream),
    m_thread(thread),
    m_stream(stream)
{
    Q_ASSERT(m_thread);
    m_name = QString::fromStdString(m_thread->name());
    m_color = QColor::fromRgb(m_thread->color().r(), m_thread->color().g(), m_thread->color().b());
}

ThreadModel::ThreadModel(StreamModel* stream)
  : QObject(stream),
    m_thread(0),
    m_stream(stream),
    m_color(*(colorTable().begin()))
{
    m_name = tr("New thread");
}

void ThreadModel::setThread(stromx::runtime::Thread* thread)
{
    m_thread = thread;
    if(m_thread)
    {
        updateStromxColor();
        m_thread->setName(m_name.toStdString());
    }
}

void ThreadModel::setName(const QString& name)
{
    if(name != m_name)
    {
        QUndoCommand* cmd = new RenameThreadCmd(this, name);
        m_stream->undoStack()->push(cmd);
    }
}

void ThreadModel::doSetName(const QString& name)
{
    m_name = name;
    if(m_thread)
        m_thread->setName(name.toStdString());
    
    emit nameChanged(m_name);
    emit changed(this);
}

void ThreadModel::setColor(const QColor& color)
{
    if(color != m_color)
    {
        QUndoCommand* cmd = new SetThreadColorCmd(this, color);
        m_stream->undoStack()->push(cmd);
    }
}

void ThreadModel::doSetColor(const QColor& color)
{
    m_color = color;
    if (m_thread)
        updateStromxColor();
    
    emit colorChanged(m_color);
    emit changed(this);
}

void ThreadModel::updateStromxColor()
{
    stromx::runtime::Color stromxColor(m_color.red(), m_color.green(), m_color.blue());
    m_thread->setColor(stromxColor);
}

QDataStream& operator<<(QDataStream& stream, const ThreadModel* thread)
{
    return stream << thread->m_color;
}

QDataStream& operator>>(QDataStream& stream, ThreadModel* thread)
{
    stream >> thread->m_color;
    thread->updateStromxColor();
    return stream;
}
