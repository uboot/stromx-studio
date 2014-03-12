/* 
*  Copyright 2014 Matthias Fuchs
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

#ifndef FINDPACKAGESDIALOG_H
#define FINDPACKAGESDIALOG_H

#include <QDialog>
#include <QFileInfoList>

/**
 * \brief Dialog which displays a list of packages.
 */
class FindPackagesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindPackagesDialog(QWidget* parent = 0);
    
    void setPackages(const QFileInfoList & packages);
    const QFileInfoList & selectedPackages() const { return m_selectedPackages; }
    
private:
    QFileInfoList m_selectedPackages;
};

#endif // FINDPACKAGESDIALOG_H