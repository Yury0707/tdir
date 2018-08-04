/*! \file       main.cpp
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief      Main Application File.
 *
 *  This file contains the main function of the application.
 */

#include <QCoreApplication>
#include <QTextCodec>
#include <QTimer>

#ifdef __TDIR_DEBUG__
#include <QDebug>
#endif

#include "fileenumerator.h"
#include "exitcodes.h"

int _CRT_glob = 0;

/*!
 * Main application function.
 * \param[in] argc Application launch argument counter
 * \param[in] argv Application launch argument values array pointer
 * \return Application exit code
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec *utfcodec = QTextCodec::codecForName("utf8");

    QTextCodec::setCodecForLocale(utfcodec);

    /* Destructor will be called on application exit */
    FileEnumerator *fileEnumerator = new FileEnumerator(&a);

    /* Checking for memory allocation problem */
    if(!fileEnumerator)
    {
#ifdef __TDIR_DEBUG__
        qWarning() << "Can't allocate memory for FileEnumerator class object!";
#endif
        exit(TDIR_MEM_ALLOCATION_ERROR);
    }

    /* Connecting FileEnumerator finished() signal with application quit() slot to exit on finish */
    QObject::connect(fileEnumerator, &FileEnumerator::finished, &a, &QCoreApplication::quit);

    /* Run the FileEnumerator run() method function from the application event loop on a timer hit */
    QTimer::singleShot(0, fileEnumerator, SLOT(run()));

    return a.exec();
}
