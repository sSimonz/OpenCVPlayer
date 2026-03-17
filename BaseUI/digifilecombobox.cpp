#include "digifilecombobox.h"

DigiFileComboBox::DigiFileComboBox(QWidget *parent) : DigiBaseComboBox(parent),
    m_enSetOption(false),
    m_bLocalflag(false)
{
    m_mode = QFileDialog::AnyFile;
    m_curDir.append(".");
    m_fileDialog = new DigiBaseFileDialog(this);
}

void DigiFileComboBox::setFileMode(QFileDialog::FileMode mode)
{
    m_mode = mode;
}
void DigiFileComboBox::setNameFilter(const QString& filter)
{
    m_filter = filter;
}
void DigiFileComboBox::setCurrentDir(const QString&curDir)
{
    m_curDir = curDir;
}
void DigiFileComboBox::setFileOption(QFileDialog::Option option)
{
    m_option = option;
}

void DigiFileComboBox::setTitle(const QString &title)
{
    m_title = title;
}

void DigiFileComboBox::setLocalFlag(bool bflag)
{
    m_bLocalflag = bflag;
}

void DigiFileComboBox::mousePressEvent(QMouseEvent* event)
{
    if(event->button() != Qt::LeftButton)
        return;
    QString strcur = m_curDir;

    m_fileDialog->setFileMode(m_mode);
    m_fileDialog->setDirectory(m_curDir);
    if(m_enSetOption)
        m_fileDialog->setFileOption(m_option);
    if(!m_filter.isEmpty())
        m_fileDialog->setNameFilter(m_filter);
    m_fileDialog->showDialog();

    QString selectFile = m_fileDialog->getSelectFile();

    if(!selectFile.isEmpty())
    {
        if(m_bLocalflag && strcur.mid(0,1) != selectFile.mid(0,1))
        {
            return;
        }
        insertItem(0,selectFile);
        setCurrentIndex(0);
        QFileInfo info(selectFile);
        if(info.isDir())
            m_curDir = selectFile;
        else
            m_curDir = info.absolutePath();
        emit currentTextChanged();
    }
}
