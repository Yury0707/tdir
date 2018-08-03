#ifndef FILEENUMERATOR_H
#define FILEENUMERATOR_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QDir>
#include <QTextStream>

class FileEnumerator
{
    private:
        QString *_currentDir;
        QTextStream *_out;
        bool _runRecursively;
        void _listFiles(const QDir &dir);
        void _listFiles(const QFile &file);
    public:
        FileEnumerator();
        void setCurrentDir(const QString &dirPath);
        void setRecursiveMode(bool recursiveMode);
        void listFiles();
        void listFiles(const QFile &file);
        void listFiles(const QStringList args);
};

#endif // FILEENUMERATOR_H
