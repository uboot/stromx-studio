#include "Common.h"

#include <QApplication>

namespace 
{
    static QMap<QString, QColor> gColorTable;
    static QStringList gVisualizationLabels;
}

const QMap<QString, QColor>& colorTable()
{
    if(0 == gColorTable.count())
    {
        gColorTable.insert(QApplication::tr("Red"), QColor(0xbe, 0x20, 0x2e));
        gColorTable.insert(QApplication::tr("Green"), QColor(0x01, 0x95, 0x47));
        gColorTable.insert(QApplication::tr("Blue"), QColor(0x20, 0x75, 0xbc));
    }
    
    return gColorTable;
}

QStringList & visualizationLabels()
{
    if (gVisualizationLabels.count() == 0)
    {    
        gVisualizationLabels << "Automatic";
        gVisualizationLabels << "Lines";
        gVisualizationLabels << "Points";
    }
    
    return gVisualizationLabels;
}
