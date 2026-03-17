#include "digibasefiledialog.h"
#include <QDebug>
DigiBaseFileDialog::DigiBaseFileDialog(QWidget *parent) : QFileDialog(parent)
  ,m_bOption(false)
{
    m_width = 900;
    m_height = 500;
    QDir dir;
    m_directory = dir.homePath();
    m_fileMode = QFileDialog::AnyFile;
    m_fileAcceptMode = QFileDialog::AcceptOpen;
}

DigiBaseFileDialog::~DigiBaseFileDialog()
{

}

void DigiBaseFileDialog::showDialog()
{

    QFileDialog::setFileMode(m_fileMode);
    QFileDialog::setDirectory(m_directory);
    QFileDialog::setAcceptMode(m_fileAcceptMode);

    if(m_bOption)
        setOption(m_fileOption);
    if(m_fileMode == QFileDialog::AnyFile ||\
       m_fileMode == QFileDialog::ExistingFile ||\
       m_fileMode == QFileDialog::ExistingFiles)
    {
        if(m_filters.isEmpty())
            QFileDialog::setNameFilter(m_filter);
        else
            QFileDialog::setNameFilters(m_filters);
    }

    if(exec() == QDialog::Accepted)
    {
        m_selectFiles = selectedFiles();
        m_selectFile = m_selectFiles.at(0);
    }
}
void DigiBaseFileDialog::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);
}
