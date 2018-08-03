#include "fileenumerator.h"

#include <QDir>
#include <QDateTime>
#include <QCoreApplication>
#include <QTextStream>

#ifdef _WIN32
    static const char *codecName = "IBM866";
#else
    static const char *codecName = "UTF-8";
#endif

/*
 * You can change file_modification_time format here.
 * It uses the same format as QDateTime::toString method.
 */
const char *file_modification_time_format = "dd.MM.yyyy hh:mm:ss";

FileEnumerator::FileEnumerator()
{
    _out = new QTextStream(stdout);
    _out->setCodec(codecName);
    _currentDir = new QString(QDir::current().absolutePath());
    _runRecursively = false;
}

/* Method function sets current (start) directory to begin file enumeration */
void FileEnumerator::setCurrentDir(const QString &dirPath)
{
    if(_currentDir == NULL)
    {
        _currentDir = new QString(dirPath);
    }
    else
    {
        _currentDir->clear();
        _currentDir->append(dirPath);
    }
}

/* Method function sets recursive mode of file enumeration */
void FileEnumerator::setRecursiveMode(bool recursiveMode)
{
    _runRecursively = recursiveMode;
}

void FileEnumerator::listFiles(const QFile &file)
{
    _listFiles(file);
}

void FileEnumerator::_listFiles(const QFile &file)
{
    QFileInfo fileInfo(file);
    *_out << file.fileName() << " " <<
             fileInfo.size() << " " << fileInfo.lastModified().toString(file_modification_time_format) << endl;
}

/* Private file enumeration method function which can run recursively if it's needed. */
void FileEnumerator::_listFiles(const QDir &dir)
{
    QCoreApplication::processEvents();

    QStringList fileList =
           dir.entryList(QDir::Files);

    foreach (QString file, fileList) {
       QFileInfo fileInfo(dir.absoluteFilePath(file));
       *_out << dir.absoluteFilePath(file) << " " <<
              fileInfo.size() << " " << fileInfo.lastModified().toString(file_modification_time_format) << endl;
    }

    _out->flush();

    if(_runRecursively)
    {
        QStringList dirList =
               dir.entryList(QDir::Dirs);

        foreach (QString subdir, dirList) {
           if (subdir == QString(".") || subdir == QString(".."))
           {
               continue;
           }
           _listFiles(QDir(dir.absoluteFilePath(subdir)));
        }
    }
}

/* Public method function of enumeration files to be called by Class user. */
void FileEnumerator::listFiles()
{
    _listFiles(QDir(*_currentDir));
}

