/*! \file       fileenumerator.h
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief File Enumerator module header file.
 *
 *  This file contains the FileEnumerator class declaration.
 */

#ifndef FILEENUMERATOR_H
#define FILEENUMERATOR_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QCoreApplication>

#include <QDir>
#include <QTextStream>

#include "exitcodes.h"

/*! \class FileEnumerator
 *  \brief File Enumerator Class.
 *
 *  This class enumerates files in the specified
 *  directory or recursively.
 */
class FileEnumerator : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Yury Osipov")
    Q_CLASSINFO("e-mail", "yusosipov@gosniias.ru")
    private:
        QCoreApplication *_app;
        QStringList *_appArguments;
        QString *_currentDir;
        QTextStream *_out;
        bool _runRecursively;
        void _listFiles(const QDir &dir);
        void _listFiles(const QFile &file);
    public:
        FileEnumerator(QObject *parent = nullptr);
        ~FileEnumerator();
        void setCurrentDir(const QString &dirPath);
        void setRecursiveMode(bool recursiveMode);
        void listFiles();
        void listFiles(const QFile &file);
        void listFiles(const QStringList args);
public slots:
        void run();
signals:
        void finished();
};

#endif // FILEENUMERATOR_H
