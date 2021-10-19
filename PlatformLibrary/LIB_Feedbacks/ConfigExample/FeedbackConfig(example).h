#ifndef __FEEDBACKPLTFCFG_H
#define __FEEDBACKPLTFCFG_H


#define FEEDBACK_ENABLE TRUE
#define PLATFORM2_FEEDBACK
/**
Numbers of Supported Feedbacks 
*/
#define N_FEEDBACK  9
/**
Number of readings in order to establish feedback's state
*/
#define FEEDBACK_SAMPLES                16
/** 
Zeo Crossing Voltage Not Available 
*/
/** 
In presence of a frequency, must be less than FEEDBACK_SAMPLES and FDB_NO_ZC_LIMIT_FOR_LEVEL 
*/
#define FDB_NO_ZC_LIMIT_FOR_FREQ        2
/** 
In presence of level high or low, must be less than FEEDBACK_SAMPLES and greater than FDB_NO_ZC_LIMIT_FOR_FREQ 
*/
#define FDB_NO_ZC_LIMIT_FOR_LEVEL       14

/** 
Zero Crossing Voltage Available 
*/
/** 
In presence of a frequency, must be less than FEEDBACK_SAMPLES  
*/
#define FDB_ZC_LIMIT_FOR_FREQ           14
/** 
In presence of a level, must be less than FEEDBACK_SAMPLES  
*/
#define FDB_ZC_LIMIT_FOR_LEVEL          14

#endif /* __FEEDBACKPLTFCFG_H */
