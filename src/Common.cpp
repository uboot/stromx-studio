#include "Common.h"

#include <QApplication>

namespace 
{
    static QMap<QString, QColor> gColorTable;
}

const QMap<QString, QColor>& colorTable()
{
    if(0 == gColorTable.count())
    {
        gColorTable.insert(QApplication::tr("Red"), QColor(0xbe, 0x20, 0x2e));
        gColorTable.insert(QApplication::tr("Green"), QColor(0x01, 0x95, 0x47));
        gColorTable.insert(QApplication::tr("Blue"), QColor(0x20, 0x75, 0xbc));
        gColorTable.insert(QApplication::tr("Black"), QColor(0x00, 0x00, 0x00));
    }
    
    return gColorTable;
}
