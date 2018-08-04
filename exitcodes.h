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
    TDIR_ARG_NO_ERROR,                  /*!< No error occured */
    TDIR_ARG_ERR_WRONG_OPTION_POS,      /*!< Some options were misordered */
    TDIR_ARG_ERR_UNKNOWN_OPTION,        /*!< Unknown option was found */
    TDIR_ARG_ERR_OPTION_DUPLICATION,    /*!< Duplication of options was found */
    TDIR_ARG_UNKNOWN_ERROR,             /*!< Unknown error has happened */
    TDIR_MEM_ALLOCATION_ERROR,          /*!< Memory allocation error has happened */
    TDIR_APPLICATION_ERROR
};

#endif // EXITCODES_H
