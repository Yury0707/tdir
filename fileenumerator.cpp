/*! \file       fileenumerator.cpp
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief File Enumerator module source file.
 *
 *  This file contains the FileEnumerator class source code.
 */

#include "fileenumerator.h"
#include "options.h"

#include <QDir>
#include <QDateTime>
#include <QTextStream>

#ifdef __TDIR_DEBUG__
#include <QDebug>
#endif

/* Fixing terminal encoding for different OS compability */
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

/*!
 * \brief FileEnumerator::FileEnumerator
 * \param parent A parent object calling constructor
 *
 * FileEnumerator Constructor
 */
FileEnumerator::FileEnumerator(QObject *parent) : QObject (parent)
{
    _app = qobject_cast<QCoreApplication *>(parent);
    _out = new QTextStream(stdout);
    _out->setCodec(codecName);
    _currentDir = new QString(QDir::current().absolutePath());  /*!< Saving app launch (current) dir by default */
    _mask = new QStringList();
    _runRecursively = false;                                    /*!< No recursive file enumeration mode by default */
    _exitCode = TDIR_APPLICATION_NO_ERROR;                      /*!< Setting no error code by default */

    _fillOptionList();

}

/*!
 * \brief FileEnumerator::_fillOptionList
 *
 * Filling a known options for the application
 */
void FileEnumerator::_fillOptionList()
{
    _knownOptions["-r"] = TDIR_OPT_RECURSIVE;
    _knownOptions["-h"] = TDIR_OPT_HELP;
}

/*!
 * \brief FileEnumerator::~FileEnumerator
 *
 * FileEnumerator Destructor
 */
FileEnumerator::~FileEnumerator()
{
    if(_out) delete _out;
    if(_currentDir) delete _currentDir;
    if(_mask) delete _mask;
    if(_appArguments) delete _appArguments;
}

/*!
 * \brief FileEnumerator::viewHelp
 *
 * Application help view method
 */
void FileEnumerator::viewHelp()
{
    if(_exitCode !=0)
    {
        switch(_exitCode)
        {
            case  TDIR_ARG_ERR_WRONG_OPTION_POS:
                *_out << "A wrong options order has been detected." << endl << endl;
                break;
            case TDIR_ARG_ERR_UNKNOWN_OPTION:
                *_out << "An unknown option was set." << endl << endl;
                break;
            case TDIR_ARG_ERR_OPTION_DUPLICATION:
                *_out << "An option duplication has been detected." << endl << endl;
                break;
            case TDIR_ARG_ERR_TOO_MANY:
                *_out << "Too many arguments." << endl << endl;
                break;
            case TDIR_ARG_UNKNOWN_ERROR:
                *_out << "An unknown error has happened." << endl << endl;
                break;
            case TDIR_MEM_ALLOCATION_ERROR:
                *_out << "Memory allocation error has happened." << endl << endl;
                break;
            case TDIR_APPLICATION_ERROR:
                *_out << "An application error has happened." << endl << endl;
                break;
            default:
                *_out << "Unknown error has happened." << endl << endl;
                break;
        }
    }

    *_out << "\tTdir application help." << endl <<
             "Usage:" << endl <<
             "   tdir [path] [option]" << endl << endl;

    *_out << "path -" << "\t\tThis is an optional parameter. You can set a path here to start file enumeration from." << endl <<
             "\t\tAlso you can set here a file mask to enumerate files which match this mask." << endl <<
             "\t\tIf this parameter is empty current folder is a start path." << endl << endl;

    *_out << "option -" << "\tThis is an optional parameter. You can set it to \"-r\" for recursive file enumeration." << endl <<
             "\t\tAlso you can set it to \"-h\" to display help." << endl << endl;
}

/*!
 * \brief FileEnumerator::_exit
 */
void FileEnumerator::_exit()
{
    if(_exitCode != 0)
    {
        viewHelp();
    }
    exit(_exitCode);
}

/*!
 * \brief FileEnumerator::_findOptions
 * \return Number of found options (>=0) or error (<0)
 *
 * App call option finder method
 */
int FileEnumerator::_findOptions()
{
    int ret = 0;

    /* Check if args are accessible */
    if(_appArguments == nullptr)
    {
        ret = -1;
    }
    else
    {
        /* Trying to find options ("-*") in arg list */
        for(int argNum = 1; argNum < _appArguments->count(); argNum++ )
        {
            QString currentArg(_appArguments->at(argNum));
            if(currentArg.contains("-"))
            {
                /* Check if option is known one */
                if(!_knownOptions.contains(currentArg))
                {
                    ret = -2; /* Unknown option */
                }
                else
                {
                    /* Check if option was already found earlier (duplication) */
                    if(_options.values().contains(currentArg))
                    {
                        ret = -3; /* Option duplication */
                    }
                    else
                    {
                        /* Save option finally, incrementing option counter */
                        _options.insert(argNum, currentArg);
                        ret++;
                    }
                }
            }
        }
    }

    return ret;
}

/*!
 * \brief FileEnumerator::run
 *
 * Class main working function
 */
void FileEnumerator::run()
{
    /* Check for ability to get application call arguments */
    if(_app == nullptr)
    {
#ifdef __TDIR_DEBUG__
        qWarning() << "Can't get application call arguments!";
#endif
        _exitCode = TDIR_APPLICATION_ERROR;
        _exit();
    }

    _appArguments =  new QStringList(_app->arguments());

    /* Check for memory allocation error */
    if(_appArguments == nullptr)
    {
#ifdef __TDIR_DEBUG__
        qWarning() << "Can't allocate memory for application call arguments!";
#endif
        _exitCode = TDIR_MEM_ALLOCATION_ERROR;
        _exit();
    }

    if(_appArguments->isEmpty())
    {
#ifdef __TDIR_DEBUG__
        qWarning() << "Empty argument list!";
#endif
        _exitCode = TDIR_APPLICATION_ERROR;
        _exit();
    }

    /* Application was launched without arguments */
    if(_appArguments->count() == 1)
    {
        /* Calling file enumerator method with default settings (from current dir) */
        listFiles();
        _exitCode = TDIR_APPLICATION_NO_ERROR;
        _exit();
    }

    /* Find recursive option & it's position & duplication */
     int options = _findOptions();

     /* Exiting with right code on error */
     if(options < 0)
     {
        switch (options)
        {
            case -1:
#ifdef __TDIR_DEBUG__
                qWarning() << "Error getting arguments!";
#endif
                _exitCode = TDIR_APPLICATION_ERROR;
                _exit();
            break;
            case -2:
#ifdef __TDIR_DEBUG__
                qWarning() << "Unknown option was found!";
#endif
                _exitCode = TDIR_OPT_UNKNOWN;
                _exit();
            break;
        case -3:
#ifdef __TDIR_DEBUG__
                qWarning() << "Option duplication was found!";
#endif
                _exitCode = TDIR_ARG_ERR_OPTION_DUPLICATION;
                _exit();
            break;
        default:
#ifdef __TDIR_DEBUG__
                qWarning() << "Unknown error during options parcing has happened!";
#endif
                _exitCode = TDIR_ARG_UNKNOWN_ERROR;
                _exit();
            break;
        }
     }

     /* If application was called with one argument */
     if(_appArguments->count() == 2)
     {
         if(options > 0)
         {
             /* Check if option is recursive mode */
             if(_options.values().contains("-r"))
             {
                 /* Go recursively from current path */
                 setRecursiveMode(true);
                 listFiles();
                 _exitCode = TDIR_APPLICATION_NO_ERROR;
                 _exit();
             }
             /* Check if option is help */
             else if(_options.values().contains("-h"))
             {
                 viewHelp();
                 _exitCode = TDIR_APPLICATION_NO_ERROR;
                 _exit();
             }
         }
         /* Check if it is a valid path */
         else
         {
             QDir dir(_appArguments->at(1));

             /* Check if it is a valid directory name */
             if(dir.exists())
             {
                 qDebug() << "Directory exists! Setting it to default one!";
                 setCurrentDir(dir.absolutePath());
                 listFiles();
                 _exitCode = TDIR_APPLICATION_NO_ERROR;
                 _exit();
             }
             /* Check if mask was applied */
             else
             {
                 /* Check if it is a mask here */
                 setMask(_appArguments->at(1));
                 listFiles();
                 _exitCode = TDIR_APPLICATION_NO_ERROR;
                 _exit();
             }
         }
     }

     /* If application was called with one argument */
     if(_appArguments->count() == 3)
     {
         if(options > 0)
         {
             /* Check if option is recursive mode */
             if(_options.values().contains("-r"))
             {
                 if(!(_options.key("-r") == 2))
                 {
                     qDebug() << "Wrong option position!";
                     _exitCode = TDIR_ARG_ERR_WRONG_OPTION_POS;
                     _exit();
                 }
                 else
                 {
                     setRecursiveMode(true);

                     QDir dir(_appArguments->at(1));

                     /* Check if it is a valid directory name */
                     if(dir.exists())
                     {
                         qDebug() << "Directory exists! Setting it to default one!";
                         setCurrentDir(dir.absolutePath());
                         listFiles();
                         _exitCode = TDIR_APPLICATION_NO_ERROR;
                         _exit();
                     }
                     /* Check if help option is the first */
                     else if((_options.key("-h") == 1))
                     {
                         qDebug() << "Wrong option position!";
                         _exitCode = TDIR_ARG_ERR_WRONG_OPTION_POS;
                         _exit();
                     }
                     else
                     {
                         /* Check if it is a mask here */
                         setMask(_appArguments->at(1));
                         listFiles();
                         _exitCode = TDIR_APPLICATION_NO_ERROR;
                         _exit();
                     }
                 }
             }
             /* Check if option is help */
             else if(_options.values().contains("-h"))
             {
                 if(!(_options.key("-h") == 2))
                 {
                     qDebug() << "Wrong option position!";
                     _exitCode = TDIR_ARG_ERR_WRONG_OPTION_POS;
                     _exit();
                 }
                 else
                 {
                    viewHelp();
                    _exitCode = TDIR_APPLICATION_NO_ERROR;
                    _exit();
                 }
             }
             else
             {
                 _exitCode = TDIR_ARG_ERR_UNKNOWN_OPTION;
                 _exit();
             }
         }
         else
         {
             _exitCode = TDIR_ARG_ERR_WRONG_OPTION_POS;
             _exit();
         }
     }

     /* If application was called with too many arguments (>3) */
     if(_appArguments->count() > 3)
     {
        _exitCode = TDIR_ARG_ERR_TOO_MANY;
        _exit();
     }

    emit finished();
}

/*!
 * \brief FileEnumerator::setCurrentDir
 * \param dirPath
 *
 * Method function sets current (start) directory to start file enumeration
 */
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

/*!
 * \brief FileEnumerator::setMask
 * \param[in] mask File mask
 *
 * This method stores a mask to a private variable
 */
void FileEnumerator::setMask(const QString mask)
{
    if(_mask == nullptr)
    {
        _mask = new QStringList();
        _mask->append(mask);
    }
    else
    {
        _mask->clear();
        _mask->append(mask);
    }
}

/*!
 * \brief FileEnumerator::setRecursiveMode
 * \param[in] recursiveMode Set to "true" to use recursive file enumeration
 *
 * This method sets a stored recursive flag
 */
void FileEnumerator::setRecursiveMode(const bool recursiveMode)
{
    _runRecursively = recursiveMode;
}

/*!
 * \brief FileEnumerator::listFiles
 * \param[in] file A file to show info for
 *
 * A public class method to view info for a 1 file only.
 * Gives an interface for a private method.
 */
void FileEnumerator::listFiles(const QFile &file)
{
    _listFiles(file);
}

/*!
 * \brief FileEnumerator::_listFiles
 * \param[in] file A file to show info for
 *
 * A private class method to view info for a 1 file only
 */
void FileEnumerator::_listFiles(const QFile &file)
{
    QFileInfo fileInfo(file);
    *_out << file.fileName() << " " <<
             fileInfo.size() << " " << fileInfo.lastModified().toString(file_modification_time_format) << endl;
}


/*!
 * \brief FileEnumerator::_listFiles
 * \param[in] dir A root dir to start enumeration from
 *
 * Private file enumeration method function which can run recursively if it's needed.
 */
void FileEnumerator::_listFiles(const QDir &dir)
{
    QCoreApplication::processEvents();      /*!< Processing events not to freeze event loop */

    QStringList fileList;

    /* Check if mask exists */
    if(_mask)
    {
        if(!_mask->isEmpty())
        {
            /* Building a file list according to mask */
            fileList = dir.entryList(*_mask, QDir::Files);
        }
        else
        {
            /* If mask is empty - looking for all files in dir */
            fileList = dir.entryList(QDir::Files);
        }
    }
    else
    {
        /* If mask doesn't exist - looking for all files in dir */
        fileList = dir.entryList(QDir::Files);
    }

    /* Showing file information according to format:
       path/file_name file_size file_modification_time */
    foreach (QString file, fileList) {
       QFileInfo fileInfo(dir.absoluteFilePath(file));
       *_out << dir.absoluteFilePath(file) << " " <<
              fileInfo.size() << " " << fileInfo.lastModified().toString(file_modification_time_format) << endl;
    }

    _out->flush();

    /* Checking if recursive algorythm needed */
    if(_runRecursively)
    {
        /* Building a dir list for current root dir */
        QStringList dirList =
               dir.entryList(QDir::Dirs);

        /* Calling this function recursively for all dirs in a list */
        foreach (QString subdir, dirList) {
           /* Skipping "." & ".." directories */
           if (subdir == QString(".") || subdir == QString(".."))
           {
               continue;
           }
           _listFiles(QDir(dir.absoluteFilePath(subdir)));
        }
    }
}

/*!
 * \brief FileEnumerator::listFiles
 *
 * Public method function of enumeration files to be called by a Class user.
 */
void FileEnumerator::listFiles()
{
    _listFiles(QDir(*_currentDir));
}

