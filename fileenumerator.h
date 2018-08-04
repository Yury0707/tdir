/*! \file       fileenumerator.h
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief      File Enumerator module header file.
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
    Q_CLASSINFO("e-mail", "yusosipov@ya.ru")
    private:
        QCoreApplication *_app;             /*!< A pointer to the application to get it's args */
        QStringList *_appArguments;         /*!< A pointer to stored application arguments */


        QMap<int, QString> _options;        /*!< A map between arg pos & stored option */

        QMap<QString, int> _knownOptions;   /*!< A map between a known option & option ID (see options.h) */

        QString *_currentDir;               /*!< A root path to start file enumeration */
        QStringList *_mask;                 /*!< A pointer to a stored file mask */

        QTextStream *_out;                  /*!< A pointer to store the setted up standard output stream */

        bool _runRecursively;               /*!< Recursive file enumeration flag */

        int _exitCode;                      /*!< Stores exit code to pass on application exit */

        void _fillOptionList();

        int _findOptions();

        void _listFiles(const QDir &dir);
        void _listFiles(const QFile &file);

        void _exit();
    public:
        FileEnumerator(QObject *parent = nullptr);
        ~FileEnumerator();
        void setCurrentDir(const QString &dirPath);
        void setRecursiveMode(const bool recursiveMode);
        void setMask(const QString mask);
        void listFiles();
        void listFiles(const QFile &file);
        void viewHelp();
public slots:
        void run();
signals:
        void finished();
};

#endif // FILEENUMERATOR_H
