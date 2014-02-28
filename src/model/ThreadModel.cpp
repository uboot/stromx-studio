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
}

const QColor ThreadModel::color() const
{
    return QColor::fromRgb(m_thread->color().r(), m_thread->color().g(), m_thread->color().b());
}

void ThreadModel::setColor(const QColor& color)
{
    if(color != this->color())
    {
        QUndoCommand* cmd = new SetThreadColorCmd(this, color);
        m_stream->undoStack()->push(cmd);
    }
}

const QString ThreadModel::name() const
{
    return QString::fromStdString(m_thread->name());
}

void ThreadModel::setName(const QString& name)
{
    if(name != this->name())
    {
        QUndoCommand* cmd = new RenameThreadCmd(this, name);
        m_stream->undoStack()->push(cmd);
    }
}

void ThreadModel::doSetName(const QString& name)
{
    if(m_thread)
        m_thread->setName(name.toStdString());
    
    emit nameChanged(this->name());
    emit changed(this);
}

void ThreadModel::doSetColor(const QColor& color)
{
    updateStromxColor(color);
    
    emit colorChanged(this->color());
    emit changed(this);
}

void ThreadModel::updateStromxColor(const QColor& color)
{
    stromx::runtime::Color stromxColor(color.red(), color.green(), color.blue());
    m_thread->setColor(stromxColor);
}

QDataStream& operator<<(QDataStream& stream, const ThreadModel* thread)
{
    return stream << thread->color();
}

QDataStream& operator>>(QDataStream& stream, ThreadModel* thread)
{
    QColor color;
    stream >> color;
    thread->updateStromxColor(color);
    return stream;
}
