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

#include <QDebug>

#include "fileenumerator.h"
#include "exitcodes.h"

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

    // Task parented to the application so that it
    // will be deleted by the application.
    FileEnumerator *fileEnumerator = new FileEnumerator(&a);

    /* Checking for memory allocation problem */
    if(!fileEnumerator)
    {
        exit(TDIR_MEM_ALLOCATION_ERROR);
    }

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(fileEnumerator, &FileEnumerator::finished, &a, &QCoreApplication::quit);


    // This will run the task from the application event loop.
    QTimer::singleShot(0, fileEnumerator, SLOT(run()));

    return a.exec();
}
