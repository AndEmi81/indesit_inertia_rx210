/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	  Servizio di sincronizzazione
\n	    Il file contiene le funzioni per gestire la sottoscrizione alla ricezione di un evento e per
\n      la notifica dell'evento.
\n
\file		syncSvc.h
\ingroup       LIB_Notify
\date	    20/09/2011
\version	01.00.00
\author		Marco Sperandini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		20/09/2011
\n		marco.sperandini
\n		changes
\n 			Creation
\n
*/
#ifndef _SYNCSVC_H_
#define _SYNCSVC_H_

#include "notify.h"

#define EV_NONE 	0x0
#define EV_10MS 	0x1
#define EV_ZC   	0x2	/* ZC both front */
#define EV_ZC_POS   0x3 /* ZC up front */
#define EV_ZC_NEG   0x4 /* ZC down front */

extern struct SubscriberNode *syncSvcList;
#define SUBSCRIBE_SYNC( node ) subscribe( &syncSvcList, node )
#define UN_SUBSCRIBE_SYNC( node ) unSubscribe( &syncSvcList, node )
#define IS_SUBSCRIBE_SYNC( node ) isSubscribed( &syncSvcList, node )
#endif /* _SYNCSVC_H_ */
/* *********** */
/* End of file */
/* *********** */

