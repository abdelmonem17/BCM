/*
 * BCM_LCfg.h
 *
 * Created: 11/17/2019 5:52:49 PM
 *  Author: AVE-LAP-062
 */ 


#ifndef BCM_LCFG_H_
#define BCM_LCFG_H_
#include "Types.h"


typedef struct
{
	uint32 baudrate;
}BCM_ConfigType;


extern BCM_ConfigType BCM_conf;


#endif /* BCM_LCFG_H_ */