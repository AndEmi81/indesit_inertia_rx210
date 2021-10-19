/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	  Lib gestione sottoscrizione e notfica eventi
\n	    Il file contiene le funzioni per gestire la sottoscrizione alla ricezione di un evento e per 
\n      la notifica dell'evento.
\n
\file		notify.h
\ingroup    LIB_Notify
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

#ifndef _NOTIFY_H_
#define _NOTIFY_H_
#include "StandardTypes.h"
#define NOTIFY_PRIORITY

typedef uint16_t EventType_t;
typedef struct {
	EventType_t type;
} NotifyEvent;

typedef struct {
	EventType_t type;
	uint16_t data;
} NotifyDataEvent;

typedef struct {
	EventType_t type;
	uint32_t time;
} NotifyTimeEvent;


struct SubscriberNode
{
    void (*callback)(NotifyEvent*);
    NotifyEvent event;
    struct SubscriberNode *next;
#ifdef NOTIFY_PRIORITY
    unsigned char priority;
#endif    
};

void subscribe( struct SubscriberNode **head, struct SubscriberNode *node);
int unSubscribe( struct SubscriberNode **head, struct SubscriberNode *node);
int isSubscribed(struct SubscriberNode **head, struct SubscriberNode *node);
void notifyEvent( struct SubscriberNode **head, NotifyEvent*  pEvent);
#endif /* _NOTIFY_H_ */
