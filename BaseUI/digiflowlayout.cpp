#include "digiflowlayout.h"
#include <QDebug>

DigiFlowlayout::DigiFlowlayout(QWidget *parent, int margin, int hSpacing, int vSpacing):
    QLayout(parent),
    m_keepWsize(false),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing),
    m_Scale(1)
{
#ifdef Q_OS_MAC
    m_hSpace = 6;
    m_vSpace = 10;
#endif
    setContentsMargins(margin, margin, margin, margin);
    m_Margin = contentsMargins();
}

DigiFlowlayout::DigiFlowlayout(int margin, int hSpacing, int vSpacing):
    QLayout(nullptr),
    m_keepWsize(false),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing),
    m_Scale(1)
{
    setContentsMargins(margin, margin, margin, margin);
    m_Margin = contentsMargins();
}

DigiFlowlayout::~DigiFlowlayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void DigiFlowlayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

void DigiFlowlayout::keepWidgetSize(bool flag)
{
    m_keepWsize = flag;
}

int DigiFlowlayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return static_cast<int>(m_hSpace * m_Scale);
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int DigiFlowlayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return static_cast<int>(m_vSpace * m_Scale);
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

void DigiFlowlayout::setHorizontalSpacing(int hSpacing)
{
    m_hSpace = hSpacing;
}

void DigiFlowlayout::setVerticalSpacing(int vSpacing)
{
    m_vSpace = vSpacing;
}

int DigiFlowlayout::count() const
{
    return itemList.size();
}

QLayoutItem *DigiFlowlayout::itemAt(int index) const
{
    if (index >= 0 && index < itemList.size())
        return itemList.value(index);
    else
        return nullptr;
}

QLayoutItem *DigiFlowlayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return nullptr;
}

void DigiFlowlayout::SetScale(qreal scale)
{
    m_Scale = scale;
    setContentsMargins(m_Margin * scale);
}

Qt::Orientations DigiFlowlayout::expandingDirections() const
{
    return Qt::Orientations();
}

bool DigiFlowlayout::hasHeightForWidth() const
{
    return true;
}

int DigiFlowlayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void DigiFlowlayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize DigiFlowlayout::sizeHint() const
{
    return minimumSize();
}

QSize DigiFlowlayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
        size = size.expandedTo(item->minimumSize());

    size += QSize(2*contentsMargins().top(), 2*contentsMargins().top());
    return size;
}

int DigiFlowlayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    if(m_keepWsize)
    {
        bool bEmpty = true;
        int width = 0;
        const int spaceX = horizontalSpacing();
        const int spaceY = verticalSpacing();
        QList<QLayoutItem*> ls;
        foreach (QLayoutItem *item, itemList) {
            width += item->sizeHint().width();
            if(width + spaceX*(ls.count()-1) > effectiveRect.width())
            {
                width = width - item->sizeHint().width();
                foreach (QLayoutItem* tempItem, ls) {
                    if (!testOnly)
                        tempItem->setGeometry(QRect(x, y, tempItem->sizeHint().width(), tempItem->sizeHint().height()));
                    x += tempItem->sizeHint().width() + spaceX;
                    lineHeight = qMax(lineHeight, tempItem->sizeHint().height());
                }

                ls.clear();
                ls.append(item);
                width = item->sizeHint().width();
                x = effectiveRect.x();
                y += lineHeight + spaceY;
                bEmpty = false;
                lineHeight = 0;
            }
            else
            {
                ls.append(item);
            }
        }

        if(!ls.isEmpty())
        {
            foreach (QLayoutItem* tempItem, ls) {
                if (!testOnly)
                    tempItem->setGeometry(QRect(x, y, tempItem->sizeHint().width(), tempItem->sizeHint().height()));
                x += tempItem->sizeHint().width() + spaceX;
                lineHeight = qMax(lineHeight, tempItem->sizeHint().height());
            }
            y += lineHeight + spaceY;
            bEmpty = false;
        }

        if(!bEmpty)
            y = y - spaceY;
        return y - rect.y() + bottom;
    }
    else
    {
        QLayoutItem *item;
        foreach (item, itemList) {
            QWidget *wid = item->widget();
            int spaceX = horizontalSpacing();
            if (spaceX == -1)
                spaceX = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
            int spaceY = verticalSpacing();
            if (spaceY == -1)
                spaceY = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

            int nextX = x + item->sizeHint().width() + spaceX;
            if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
                x = effectiveRect.x();
                y = y + lineHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                lineHeight = 0;
            }

            if (!testOnly)
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

            x = nextX;
            lineHeight = qMax(lineHeight, item->sizeHint().height());
        }
        return y + lineHeight - rect.y() + bottom;
    }
}

int DigiFlowlayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}
