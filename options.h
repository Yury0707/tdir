/*! \file       exitcodes.h
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief      Options header file.
 *
 *  This file contains the set of options for the application launch.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

/*! \enum TdirOptEnum
 *  \brief Tdir options enum.
 *
 *  This enum contains options to call on application launch.
 */
enum TdirOptEnum
{
    TDIR_OPT_RECURSIVE,      /*!< No error occured */
    TDIR_OPT_HELP,           /*!< Some options were misordered */
    TDIR_OPT_UNKNOWN,        /*!< Unknown option was found */
};

#endif // OPTIONS_H
