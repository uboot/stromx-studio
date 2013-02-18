#include "delegate/ChooseImageButton.h"

#include <QFileDialog>
#include <QSettings>
#include <iostream>

ChooseImageButton::ChooseImageButton(QWidget* parent)
  : QPushButton(parent)
{
    setText(tr("Choose..."));
    
    connect(this, SIGNAL(clicked(bool)), this, SLOT(chooseImage()));
}

void ChooseImageButton::chooseImage()
{    
    QSettings settings("stromx", "stromx-studio");
    QString lastDir = settings.value("lastImageOpened", QDir::home().absolutePath()).toString();
    
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                lastDir, tr("Images (*.png *.xpm *.jpg *.bmp *.tif)"), 0, QFileDialog::DontUseNativeDialog); 
    
    if(file.isNull())
        return;
    
    m_image = QImage(file);
    
    if(m_image.isNull())
        return;
    
    // remember the last file
    settings.setValue("lastImageOpened", file);
    
    emit choseImage();
}

void ChooseImageButton::reset()
{
    m_image = QImage();
}

