#ifndef DIGIBASEFILEDIALOG_H
#define DIGIBASEFILEDIALOG_H

/**
 * @brief 客户端公共库中的文件对话框 FileDialog 基础封装类
 * 1. 可缩放，可进行缩放，基础大小默认固定值：900*500
 * 2. 可设置对话框的文件模式、接收模式、过滤器和当前目录、对话框属性
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QObject>
#include <QFileDialog>
#include "Base/iwidgetbase.h"

class DigiBaseFileDialog : public QFileDialog,public IWidgetBase
{
    Q_OBJECT
public:
    explicit DigiBaseFileDialog(QWidget *parent = nullptr);
    ~DigiBaseFileDialog();
    void setDirectory(QString path){
        m_directory = path;
    }//设置文件对话框的当前目录
    void setNameFilter(const QString filter){
        m_filter = filter;
    }//将文件对话框中使用的过滤器设置为给定的过滤器

    void setNameFilters(const QStringList filters){
        m_filters = filters;
    }//设置文件对话框中使用的过滤器

    void setFileMode(QFileDialog::FileMode fileMode){
        m_fileMode = fileMode;
    }//设置对话框的文件模式:AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly

    void setAcceptMode(QFileDialog::AcceptMode AcceptMode){
        m_fileAcceptMode = AcceptMode;
    }//设置对话框的接受模式:AcceptOpen打开文件, AcceptSave保存文件

    void setFileOption(QFileDialog::Option fileOption){
        m_bOption = true;
        m_fileOption = fileOption;
    }//设置对话框属性
    QString getSelectFile(){
        return m_selectFile;
    }//选择文件
    QStringList getSelectFiles(){
        return m_selectFiles;
    }//选择文件列表
    void showDialog();//显示文件对话框

    void SetScale(qreal scale);//缩放
signals:

public slots:
private:
    QString m_filter;//文件过虑器
    QStringList m_filters;//文件过虑器列表

    QString m_directory;//当前目录

    QString m_selectFile;//选择目录
    QStringList m_selectFiles;//选择目录列表


    bool m_bOption;//是否设置对话框属性
    QFileDialog::Option m_fileOption;//对话框属性

    QFileDialog::FileMode m_fileMode;//文件对话框模式
    QFileDialog::AcceptMode m_fileAcceptMode;//对话框接收模式
};

#endif // DIGIBASEFILEDIALOG_H
