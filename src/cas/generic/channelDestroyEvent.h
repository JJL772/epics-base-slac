/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*
 *      $Id$
 *
 *      Author  Jeffrey O. Hill
 *              johill@lanl.gov
 *              505 665 1831
 */

#ifndef channelDestroyEventh
#define channelDestroyEventh

#ifdef epicsExportSharedSymbols
#   define epicsExportSharedSymbols_channelDestroyEventh
#   undef epicsExportSharedSymbols
#endif

// external headers included here
#include "caProto.h"

#ifdef epicsExportSharedSymbols_channelDestroyEventh
#   define epicsExportSharedSymbols
#   include "shareLib.h"
#endif

#include "casEvent.h"

class casChannelI;

class channelDestroyEvent : public casEvent {
public:
    channelDestroyEvent ( casChannelI &, bool uninstallNeeded );
private:
    casChannelI & chan;
    bool uninstallNeeded;
    caStatus cbFunc ( 
        casCoreClient &, 
        epicsGuard < casClientMutex > &,
        epicsGuard < evSysMutex > & );
};

inline channelDestroyEvent::channelDestroyEvent ( 
    casChannelI & chanIn, bool uninstallNeededIn ) :
    chan ( chanIn ), uninstallNeeded ( uninstallNeededIn )
{
}

#endif // channelDestroyEventh

