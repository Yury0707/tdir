/*! \file       fileenumerator.cpp
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief File Enumerator module source file.
 *
 *  This file contains the FileEnumerator class source code.
 */

#include "fileenumerator.h"

#include <QDir>
#include <QDateTime>

#include <QTextStream>

#ifdef __TDIR_DEBUG__
#include <QDebug>
#endif

#ifdef _WIN32
    static const char *codecName = "IBM866";
#else
    static const char *codecName = "UTF-8";
#endif

/*
 * You can change file_modification_time format here.
 * It uses the same format as QDateTime::toString method.
 */
static const char *file_modification_time_format = "dd.MM.yyyy hh:mm:ss";

FileEnumerator::FileEnumerator(QObject *parent) : QObject (parent)
{
    _app = qobject_cast<QCoreApplication *>(parent);
    _out = new QTextStream(stdout);
    _out->setCodec(codecName);
    _currentDir = new QString(QDir::current().absolutePath());
    _runRecursively = false;
}

FileEnumerator::~FileEnumerator()
{
    if(_out) delete _out;
    if(_currentDir) delete _currentDir;
}

void FileEnumerator::run()
{
    /* Check for ability to get application call arguments */
    if(_app == nullptr)
    {
#ifdef __TDIR_DEBUG__
        qWarning() << "Can't get application call arguments!";
#endif
        exit(TDIR_APPLICATION_ERROR);
    }

    _appArguments =  new QStringList(_app->arguments());

    /* Check for memory allocation error */
    if(_appArguments == nullptr)
    {
#ifdef __TDIR_DEBUG__
        qWarning() << "Can't allocate memory for application call arguments!";
#endif
        exit(TDIR_MEM_ALLOCATION_ERROR);
    }


#ifdef __REFACTORING__
    /* Application was called without parameters */
    if(argc == 1)
    {
        /* Calling file enumerator method with default settings (from current dir) */
        fileEnumerator->listFiles();
        exit(TDIR_ARG_NO_ERROR);
    }
    else if (argc == 2)
    {
        if(strcmp(argv[1], "-r") == 0)
        {
            fileEnumerator->setRecursiveMode(true);
            fileEnumerator->listFiles();
            exit(TDIR_ARG_NO_ERROR);
        }
        else
        {
            QDir dir(argv[1]);
            if(dir.exists())
            {
                qDebug() << "Directory exists! Setting it to default one!" << dir.absolutePath();
                fileEnumerator->setCurrentDir(dir.absolutePath());
                fileEnumerator->listFiles();
                exit(TDIR_ARG_NO_ERROR);
            }
            else
            {
                qDebug() << "No such directory!";
                qDebug() << "Checking for file (if mask was set)";
                QFile file(argv[1]);
                if(file.exists())
                {
                    fileEnumerator->listFiles(file);
                    exit(TDIR_ARG_NO_ERROR);
                }
                else
                {
                    exit(-1);
                }
            }
        }
    }
    else
    {
        if(a.arguments().contains("-r"))
        {
            QStringList argList = a.arguments();

            int recursiveOptLastPos = argList.lastIndexOf("-r");
            if(recursiveOptLastPos + 1 != argList.count())
            {
                qDebug() << "Wrong -r option position!";
                exit(TDIR_ARG_ERR_WRONG_OPTION_POS);
            }
            else
            {
                int recursiveOptFirstPos = argList.indexOf("-r");
                if(recursiveOptFirstPos != recursiveOptLastPos)
                {
                    qDebug() << "Recursive option duplication found!";
                    exit(TDIR_ARG_ERR_OPTION_DUPLICATION);
                }
                else
                {
                    qDebug() << "Setting recursive mode . . .";
                    fileEnumerator->setRecursiveMode(true);
                }
            }
            QDir dir(argv[1]);
            if(dir.exists())
            {
                qDebug() << "Directory exists! Setting it to default one!" << dir.absolutePath();
                fileEnumerator->setCurrentDir(dir.absolutePath());
                fileEnumerator->listFiles();
                exit(TDIR_ARG_NO_ERROR);
            }
            else
            {
                qDebug() << "No such directory!";
                qDebug() << "Checking for file (if mask was set)";
                if(argc == 3)
                {
                    QFile file(argv[1]);
                    if(file.exists())
                    {
                        fileEnumerator->listFiles(file);
                        exit(TDIR_ARG_NO_ERROR);
                    }
                    else
                    {
                        exit(-1);
                    }
                }
                else
                {
                    /* There is more then one file in a list */
                    /* Looking for files & dirs recursively */
                    fileEnumerator->setRecursiveMode(true);
                    fileEnumerator->listFiles(a.arguments());
                    exit(TDIR_ARG_NO_ERROR);
                }
            }
        }
        else
        {
            /* Checking if mask was used */
            qDebug() << "Checking for mask . . .";
            fileEnumerator->listFiles(a.arguments());
            exit(TDIR_ARG_NO_ERROR);
        }
    }
#endif
    emit finished();
}

/* Method function sets current (start) directory to begin file enumeration */
void FileEnumerator::setCurrentDir(const QString &dirPath)
{
    if(_currentDir == nullptr)
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

void FileEnumerator::listFiles(const QStringList args)
{
    int fileCounter = 0;
    for (int i = 1; i < args.count(); i++)
    {
        QFile file(args.at(i));
        if(file.exists())
        {
               fileCounter++;
               QFileInfo fileInfo(file);
               if(!fileInfo.isDir())
               {
                    *_out << file.fileName() << " " <<
                             fileInfo.size() << " " << fileInfo.lastModified().toString(file_modification_time_format) << endl;
               }
        }
    }
}

