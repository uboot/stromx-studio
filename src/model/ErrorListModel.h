/* 
*  Copyright 2012 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ERRORLISTMODEL_H
#define ERRORLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "ErrorData.h"
#include "ExceptionObserver.h"

/** \brief Model of a list of errors.
 * 
 * This class models a list of errors. It contains a stromx <em>exception observer</em>
 * which asynchronously receives stromx exceptions, converts them to error data objects
 * and sends these as events to the model. Call the member observer() of this observer
 * to add an error to the error list model. 
 */
class ErrorListModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    /** The columns of the list. */
    enum Column
    {
        /** The time when the exception causing the error was caught. */
        TIME,
        /** A description of the error. */
        DESCRIPTION
    };
    
    /** Constructs a thread list model. */
    explicit ErrorListModel(QObject *parent = 0);
    
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    /** Returns an exception observer which sends events to this model. */
    ExceptionObserver* exceptionObserver() const { return const_cast<ExceptionObserver*>(&m_observer); }
 
public slots:
    /** Clears all errors in the list. */
    void clear();
    
protected:
    /** Receives the error data events which are sent by the exception observer. */
    virtual void customEvent(QEvent* event);
    
private:
    /** The maximal amount of errors which which are remembered and displayed. */
    static const qint32 MAX_ERRORS;
    
    QList<ErrorData> m_errorList;
    ExceptionObserver m_observer;
};
    
#endif // ERRORLISTMODEL_H
