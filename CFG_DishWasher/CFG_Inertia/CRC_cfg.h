/* if not defined CRC_HW will be used CRC_SW */
#define CRC_HW

#if !defined(CRC_HW)
/* if not defined CRC_SW_TABLE_DRIVEN will be used loop driven computation. 
table driven computation: faster than loop driven computation but with waste of flash (about 500 bytes more) 
*/
//#define CRC_SW_TABLE_DRIVEN
#endif
