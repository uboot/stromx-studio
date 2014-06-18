#include "Common.h"

#include <QApplication>

namespace 
{
    static QMap<QString, QColor> gColorTable;
    static QStringList gVisualizationLabels;
}

const QColor Colors::DEFAULT(QColor(0x20, 0x75, 0xbc));
const QColor Colors::RED(QColor(0xbe, 0x20, 0x2e));
const QColor Colors::GREEN(QColor(0x01, 0x95, 0x47));
const QColor Colors::BLUE(QColor(0x20, 0x75, 0xbc));

const QMap<QString, QColor>& colorTable()
{
    if(0 == gColorTable.count())
    {
        gColorTable.insert(QApplication::tr("Red"), Colors::RED);
        gColorTable.insert(QApplication::tr("Green"), Colors::GREEN);
        gColorTable.insert(QApplication::tr("Blue"), Colors::BLUE);
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
        gVisualizationLabels << "Histogram";
        gVisualizationLabels << "Image";
    }
    
    return gVisualizationLabels;
}

QString fromStromxTitle(const std::string& str)
{
    return QString::fromUtf8(str.c_str());
}
