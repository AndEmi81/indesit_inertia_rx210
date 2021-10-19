/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/



/**
\par	  Lib gestione sottoscrizione e notfica eventi
\n	    Il file contiene le funzioni per gestire la sottoscrizione alla ricezione di un evento e per 
\n      la notifica dell'evento.
\n
\file		notify.c
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

#define NULL (0)
#include "notify.h"


void notifyEvent( struct SubscriberNode **head, NotifyEvent*  pEvent)
{
   if( *head == NULL)
    {
        return;
    }
    else
    {
        struct SubscriberNode *p = *head;

        do
        {
        	if( p->event.type == pEvent->type){
        		if( p->callback != NULL)
        			p->callback( pEvent );
        	}
            p = p->next;
        }
        while(p != NULL);
    }
}
#ifdef NOTIFY_PRIORITY
void subscribe( struct SubscriberNode **head, struct SubscriberNode *node)
{
    node->next = NULL;

    if( *head == NULL)
    {
        *head = node;
    }
    else
    {
        struct SubscriberNode *p = *head;

        if( node->priority > p->priority)
        {/* inserimento in testa*/
            node->next = *head;
            *head = node;
            return;
        }
        while(p->next != NULL
              && node->priority <= p->priority
              )
        {
            p = p->next;
        }
        p->next = node;
    }
}

#else
// INSERISCE IN CODA
void subscribe( struct SubscriberNode **head, struct SubscriberNode *node)
{
    node->next = NULL;
    
    if( *head == NULL)
    {
        *head = node;
    }
    else
    {
        struct SubscriberNode *p = *head;

        while(p->next != NULL)
        {
            p = p->next;
        }
        p->next = node;
    }
}
#endif /* NOTIFY_PRIORITY */


int unSubscribe( struct SubscriberNode **head, struct SubscriberNode *node)
{
    if( *head == NULL)
    {
        return 1;
    }
    else
    {
        struct SubscriberNode *p = *head;

        if( *head == node){ /* rimozione in testa*/
            *head = p->next;
            return 1;
        }else{
            while( p->next != node  )
            {
                if( p->next == NULL )
                    return 0;

                p = p->next;
            }

            p->next = p->next->next;
            return 1;
        }
    }
    /*lint -e{527} Warning 527:*/
    return 0;

}

int isSubscribed(struct SubscriberNode **head, struct SubscriberNode *node)
{
    if( *head != NULL)
    {
        struct SubscriberNode *p = *head;
        do
        {
            if( p == node)
                return 1;

            p = p->next;
        }
        while(p != NULL);
    }
    return 0;
}
