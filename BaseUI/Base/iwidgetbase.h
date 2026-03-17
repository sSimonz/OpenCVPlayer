#ifndef IWIDGETBASE_H
#define IWIDGETBASE_H

#include <QtGlobal>
#include <QObject>
#include <QMargins>
#include "ibaseui.h"

class IWidgetBase{
public:
    virtual ~IWidgetBase(){}
    virtual void SetScale(qreal scale) = 0;
    virtual void SetWidth(qreal width) {m_width = width;}
    virtual void SetHeight(qreal height) {m_height = height;}
    virtual void SetSize(qreal width, qreal height) {m_width = width; m_height = height;}
    virtual qreal GetWidth() {return m_width;}
    virtual qreal GetHeight() {return m_height;}

protected:
    qreal m_width;
    qreal m_height;
};

struct LayoutInfo{
    QMargins margins;
    int spacing;    //其它Layout使用
    int hspacing;   //QGridLayout使用
    int vspacing;   //QGridLayout使用
};

template <typename T>
QList<T> findChildrenRecursively(QObject* parent){
    QList<T> ls;
    auto list = parent->findChildren<T>(QString(),Qt::FindDirectChildrenOnly);
    ls.append(list);
    foreach (T t, list) {
        auto subList = findChildrenRecursively<T>(t);
        ls.append(subList);
    }
    return ls;
}

#endif // IWIDGETBASE_H
