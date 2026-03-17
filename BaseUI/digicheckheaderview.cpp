#include "digicheckheaderview.h"

#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QSvgRenderer>
#include "Res/resfun.h"
#include "Base/ifontconfig.h"

DigiCheckHeaderView::DigiCheckHeaderView(int checkColumnIndex, Qt::Orientation orientation, QWidget *parent):
    QHeaderView(orientation, parent),
    m_CheckState(Qt::Unchecked),
    m_Alignment(Qt::AlignHCenter),
    m_CheckColumnIndex(checkColumnIndex),
    m_CheckBoxCurSize(20),
    m_CheckBoxSize(20)
{
    setSortIndicatorShown(true);
    setSectionsClickable(true);
    setStretchLastSection(true);

    m_Font = GetGlobalFont();
    QString family = GetGlobalFamily(QFont::Medium);
    m_Font.setFamily(family);
    LoadCheckBoxImgPath(ResFun::GetPngResUrlByName(ICON_PLAYBACK_CHECKBOX_UNSELECTED),
                        ResFun::GetPngResUrlByName(ICON_PLAYBACK_CHECKBOX_PARTSELECTED),
                        ResFun::GetPngResUrlByName(ICON_PLAYBACK_CHECKBOX_SELECTED));
}

DigiCheckHeaderView::~DigiCheckHeaderView()
{
    qDeleteAll(m_PixmapList);
}

void DigiCheckHeaderView::LoadCheckBoxImgPath(const QString &unCheckImg, const QString &paCheckedImg, const QString &checkedImg)
{
    ClearList();
    LoadImg(0, unCheckImg);
    LoadImg(1, paCheckedImg);
    LoadImg(2, checkedImg);
}

void DigiCheckHeaderView::SetCheckBoxAlignment(Qt::AlignmentFlag flag)
{
    m_Alignment = flag;
}

void DigiCheckHeaderView::SetCheckBoxSize(qreal size)
{
    m_CheckBoxSize = size;
}

void DigiCheckHeaderView::SetCheckState(Qt::CheckState state)
{
    onCheckStateChangeToTable(state);
}

Qt::CheckState DigiCheckHeaderView::GetCheckState()
{
    return m_CheckState;
}

void DigiCheckHeaderView::SetScale(qreal scale)
{
    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    m_CheckBoxCurSize = m_CheckBoxSize * scale;

    updateSection(m_CheckColumnIndex);
}

void DigiCheckHeaderView::onCheckStateChangeToTable(Qt::CheckState state)
{
    m_CheckState = state;
    this->updateSection(m_CheckColumnIndex);
}

void DigiCheckHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    if (visualIndexAt(event->pos().x()) == m_CheckColumnIndex)
    {
        QRect rect = checkBoxRect(m_CheckColumnIndex);
        if(rect.contains(event->pos()))
        {
            if(m_CheckState == Qt::Checked)
                m_CheckState = Qt::Unchecked;
            else
                m_CheckState = Qt::Checked;

            this->updateSection(m_CheckColumnIndex);
            emit CheckStateChangeSig(m_CheckState);
        }
        else
        {
            QHeaderView::mouseReleaseEvent(event);
        }
    }
    else
    {
        QHeaderView::mouseReleaseEvent(event);
    }
}

void DigiCheckHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    if(logicalIndex != m_CheckColumnIndex)
    {
        QHeaderView::paintSection(painter, rect, logicalIndex);
    }
    else
    {
        QRect rectBox = checkBoxRect(logicalIndex);
        DrawCheckBox(painter, m_CheckState, rectBox);
    }
    painter->restore();
}

QRect DigiCheckHeaderView::checkBoxRect(int logicalIndex) const
{
    //判断靠左、中、右
    int width;
    switch (m_Alignment)
    {
    case Qt::AlignLeft:
        width = 5 * m_CheckBoxCurSize/m_CheckBoxSize; // 5 * 缩放因子  靠左
        break;
    case Qt::AlignRight:
        width = sectionSize(logicalIndex)-(5 * m_CheckBoxCurSize/m_CheckBoxSize); // 总长-(5*缩放因子)  靠右
        break;
    default:
        width = (sectionSize(logicalIndex) - m_CheckBoxCurSize) / 2;  //居中
    }

    for (int i=0; i<logicalIndex; ++i)
        width += sectionSize(i);

    return QRect(width,
                 (height() - m_CheckBoxCurSize) / 2,
                 m_CheckBoxCurSize,
                 m_CheckBoxCurSize);
}

void DigiCheckHeaderView::ClearList()
{
    qDeleteAll(m_RendererList);
    qDeleteAll(m_PixmapList);
    m_RendererList.clear();
    m_PixmapList.clear();

    //初始为nullptr
    for (int i=0; i<3; i++)
    {
        QSvgRenderer *svgRender = nullptr;
        m_RendererList.append(svgRender);
        QPixmap *pixmap = nullptr;
        m_PixmapList.append(pixmap);
    }
}

void DigiCheckHeaderView::LoadImg(int index, const QString &path)
{
    if(path.contains(".svg"))
    {
        m_RendererList[index] = new QSvgRenderer(this);
        m_RendererList.at(index)->load(path);
    }
    else
    {
        m_PixmapList[index] = new QPixmap;
        m_PixmapList.at(index)->load(path);
    }
}

void DigiCheckHeaderView::DrawCheckBox(QPainter *painter, int index, const QRect &rectBox) const
{
    if (m_RendererList.at(index) != nullptr && m_RendererList.at(index)->isValid())
    {
        m_RendererList.at(index)->render(painter, rectBox);
    }
    else if (m_PixmapList.at(index) != nullptr && !m_PixmapList.at(index)->isNull())
    {
        painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter->drawPixmap(rectBox, *m_PixmapList[index], m_PixmapList[index]->rect());
    }
}
