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

#define epicsExportSharedSymbols
#include "casChannelI.h"
#include "casAsyncIOI.h"

casChannelI::casChannelI ( casChannel & chanIn, const casCtx &  ctx ) :
        chanIntfForPV ( *ctx.getClient() ), pv ( *ctx.getPV() ), 
        chan ( chanIn ), cid ( ctx.getMsg()->m_cid ), 
        serverDeletePending ( false ), accessRightsEvPending ( false )
{
}

casChannelI::~casChannelI ()
{	    
    this->pv.destroyAllIO ( this->ioList );

    this->serverDeletePending = true;
    this->chan.destroyRequest ();
    
    // force PV delete if this is the last channel attached
    this->pv.deleteSignal ();
}

void casChannelI::uninstallFromPV ( casEventSys & eventSys )
{
    tsDLList < casMonitor > dest;
    this->removeSelfFromPV ( this->pv, dest );
    while ( casMonitor * pMon = dest.get () ) {
        eventSys.prepareMonitorForDestroy ( *pMon );
	}
}

void casChannelI::show ( unsigned level ) const
{
    printf ( "casChannelI: client id %u PV %s\n", 
        this->cid, this->pv.getName() );
    if ( level > 0 ) {
        this->chanIntfForPV::show ( level - 1 );
        this->chan.show ( level - 1 );
    }
}

caStatus casChannelI::cbFunc ( 
    casCoreClient &, 
    epicsGuard < casClientMutex > & clientGuard,
    epicsGuard < evSysMutex > & evGuard )
{
    caStatus stat = S_cas_success;
    {
	    stat = this->client().accessRightsResponse ( 
                    clientGuard, this );
    }
	if ( stat == S_cas_success ) {
		this->accessRightsEvPending = false;
	}
	return stat;
}

