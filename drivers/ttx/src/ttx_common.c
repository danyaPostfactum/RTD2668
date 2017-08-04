/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com)       *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_common.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <stdlib.h>         /* Provides: NULL */
#include <ttx_log.h>        /* Provides: TTX_Error() */

/*---------------------------------------------------------------------------*/
#if 0	// not used
void
TTX_onAssert       (char const TTX_ROM * const TTX_ROM_VAR file, int line)
/*****************************************************************************\
 *  @brief   The assertion-failure handler callback.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *  @param   file   file name which the error resides
 *  @param   line   line number of file which the error resides
 *
 *  @ingroup ttx
 *  @see     TTApi_DeInit()
\*****************************************************************************/
{
    if ( file != NULL )
    {
        /* Because RTD_Log() cannot print two or more values at onece.
                TTX_Log( LOGGER_ERROR, "Assert fail in %s, ln %d\n", file, line );
        */

//        TTX_Log( LOGGER_ERROR, "Assert fail in %s, ",           file );
//      TTX_Log( LOGGER_ERROR, "ln %d\n"            , (uint32_t)line );
    }

#ifdef WIN32
//    getchar();              /* wait for user to hit a key */
//    exit( EXIT_FAILURE );   /* exit application with abnormal termination */
#endif
    line = 0;
}
#endif

#if 0

/*---------------------------------------------------------------------------*/
void
TTX_DumpMem (const void* addr, uint32_t numOfBytes)
/*****************************************************************************\
 *  @brief   Display contents of memroy byte-by-byte starting from addr.
 *  @param   addr        starting address to be displayed.
 *  @param   numOfBytes  number of bytes of content to be displayed.
\*****************************************************************************/
{
#ifndef WIN32   /* Visual C++ doesn't support #warning */
#warning "TTX_DumpMem() is being compiled. It drains much spaces!"
#endif  /* end of #ifndef WIN32 */

#define DATA_BYTES_OF_EACH_ROW   (16)   // number of bytes of each row
    volatile const uint8_t*   ptr = (volatile const uint8_t*) (addr);
    uint32_t i;
    uint8_t j;
    for ( i=0; i<numOfBytes/DATA_BYTES_OF_EACH_ROW; i++ )
    {
        // Print address
        TTX_Log(LOGGER_VERBOSE, "\n0x%X ", (uint32_t)ptr);

        // Print data
        for ( j=0; j<DATA_BYTES_OF_EACH_ROW; j++ )  // seperate into two groups: left and right
        {
            TTX_Log(LOGGER_VERBOSE, "%X ", (uint32_t)*ptr );
            ptr++;

            // Print seperator for two groups.
            if ( j == DATA_BYTES_OF_EACH_ROW/2 - 1)
            {
                TTX_Log(LOGGER_VERBOSE, " ");   // Seperator is a blank space
            }
        }
    }
}
#endif


/*---------------------------------------------------------------------------*\
 * end of ttx_common.c                                                       *
\*---------------------------------------------------------------------------*/
#endif
