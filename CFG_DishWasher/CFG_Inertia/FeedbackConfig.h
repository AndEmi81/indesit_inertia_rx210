#ifndef __FEEDBACKPLTFCFG_H
#define __FEEDBACKPLTFCFG_H
#include "autoconf.h"



/**
Numbers of Supported Feedbacks 
*/
#define N_FEEDBACK  					CONFIG_N_FEEDBACK
/**
Number of readings in order to establish feedback's state
*/
#define FEEDBACK_SAMPLES   				CONFIG_FEEDBACK_SAMPLES
/** 
Zero Crossing Voltage Not Available 
*/
/** 
In presence of a frequency, must be less than FEEDBACK_SAMPLES and FDB_NO_ZC_LIMIT_FOR_LEVEL 
*/
#define FDB_NO_ZC_LIMIT_FOR_FREQ        CONFIG_FDB_NO_ZC_LIMIT_FOR_FREQ
/** 
In presence of level high or low, must be less than FEEDBACK_SAMPLES and greater than FDB_NO_ZC_LIMIT_FOR_FREQ 
*/
#define FDB_NO_ZC_LIMIT_FOR_LEVEL       CONFIG_FDB_NO_ZC_LIMIT_FOR_LEVEL
/** 
Zero Crossing Voltage Available 
*/
/** 
In presence of a frequency, must be less than FEEDBACK_SAMPLES  
*/
#define FDB_ZC_LIMIT_FOR_FREQ           CONFIG_FDB_ZC_LIMIT_FOR_FREQ
/** 
In presence of a level, must be less than FEEDBACK_SAMPLES  
*/
#define FDB_ZC_LIMIT_FOR_LEVEL          CONFIG_FDB_ZC_LIMIT_FOR_LEVEL

#endif /* __FEEDBACKPLTFCFG_H */
