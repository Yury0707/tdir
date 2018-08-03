#include <QCoreApplication>

#include "fileenumerator.h"
#include <QTextCodec>
#include <QDebug>

enum
{
    TDIR_ARG_NO_ERROR,
    TDIR_ARG_ERR_WRONG_OPTION_POS,
    TDIR_ARG_ERR_UNKNOWN_OPTION,
    TDIR_ARG_ERR_OPTION_DUPLICATION,
    TDIR_ARG_UNKNOWN_ERROR
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec *utfcodec = QTextCodec::codecForName("utf8");

    QTextCodec::setCodecForLocale(utfcodec);

    FileEnumerator *fileEnumerator =
            new FileEnumerator();

    /* Application was called without parameters */
    if(argc == 1)
    {
        /* Calling file enumerator method with default settings (from current dir) */
        fileEnumerator->listFiles();
        exit(0);
    }
    else if (argc == 2)
    {
        if(strcmp(argv[1], "-r") == 0)
        {
            fileEnumerator->setRecursiveMode(true);
            fileEnumerator->listFiles();
            exit(0);
        }
        else
        {
            QDir dir(argv[1]);
            if(dir.exists())
            {
                qDebug() << "Directory exists! Setting it to default one!" << dir.absolutePath();
                fileEnumerator->setCurrentDir(dir.absolutePath());
                fileEnumerator->listFiles();
                exit(0);
            }
            else
            {
                qDebug() << "No such directory!";
                qDebug() << "Checking for file (if mask was set)";
                QFile file(argv[1]);
                if(file.exists())
                {
                    fileEnumerator->listFiles(file);
                    exit(0);
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
                exit(0);
            }
            else
            {
                qDebug() << "No such directory!";
                qDebug() << "Checking for file (if mask was set)";
                QFile file(argv[1]);
                if(file.exists())
                {
                    fileEnumerator->listFiles(file);
                    exit(0);
                }
                else
                {
                    exit(-1);
                }
            }
        }
        else
        {
            /* Checking if mask was used */
            qDebug() << "Checking for mask . . .";
            fileEnumerator->listFiles(a.arguments());
        }
    }
    return a.exec();
}
