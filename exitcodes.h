/*! \file       exitcodes.h
 *  \author     Yury Osipov (yusosipov@ya.ru)
 *  \version    1.0.0.1
 *  \date       August, 2018
 *  \brief      Exit codes header file.
 *
 *  This file contains the exit codes of the application.
 */

#ifndef EXITCODES_H
#define EXITCODES_H

/*! \enum TdirErrEnum
 *  \brief Tdir errors enum.
 *
 *  This enum contains errors to generate on application
 *  exit to determine what happened.
 */
enum TdirErrEnum
{
    TDIR_APPLICATION_NO_ERROR,          /*!< No error occured */
    TDIR_ARG_ERR_WRONG_OPTION_POS,      /*!< Some options were misordered */
    TDIR_ARG_ERR_UNKNOWN_OPTION,        /*!< Unknown option was found */
    TDIR_ARG_ERR_OPTION_DUPLICATION,    /*!< Duplication of options was found */
    TDIR_ARG_ERR_TOO_MANY,              /*!< Too many arguments were found */
    TDIR_ARG_UNKNOWN_ERROR,             /*!< Unknown error has happened */
    TDIR_MEM_ALLOCATION_ERROR,          /*!< Memory allocation error has happened */
    TDIR_APPLICATION_ERROR              /*!< Application error has happened */
};

#endif // EXITCODES_H
