/*-------------------------------------------------------------------------*\
 *               File: std_hal.h
 *                Ver: 1.0
 *             Author: Chi-Chun Cheng
 * Functions Provided: Provides the Hardware Abstract Layer for
 *                       the "C Chip Library" Standard header.
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_STANDARD_HAL_HEADER_INCLUDED__
#define	__C_CHIP_LIB_STANDARD_HAL_HEADER_INCLUDED__

#include <ttx_port.h>    /* Provides: TTX_RegRead() and TTX_RegWrite() */

/* HAL macro definitions --------------------------------------------------*/

#define _CCL_VALUEOF(type,x)     ((type)(x))	/* Value of x */

/*******************************************************/
/* generic macros for field manipulation               */
/*******************************************************/

#define _CCL_PER_FSHIFT(PER,REG,FIELD) \
	_##PER##_##REG##_##FIELD##_SHIFT

#define _CCL_PER_FMASK(PER,REG,FIELD) \
	_##PER##_##REG##_##FIELD##_MASK

#define _CCL_PER_FSYM(PER,REG,FIELD,SYM)\
	PER##_##REG##_##FIELD##_##SYM
/*
#define _CCL_PER_FDEFAULT(PER,REG,FIELD) \
	((##PER##_##REG##_##FIELD##_DEFAULT << _##PER##_##REG##_##FIELD##_SHIFT) \
	& _##PER##_##REG##_##FIELD##_MASK)
*/
#define _CCL_PER_FDEFAULT(PER,REG,FIELD) \
	((PER##_##REG##_OF(PER##_##REG##_##FIELD##_DEFAULT)<< _##PER##_##REG##_##FIELD##_SHIFT) \
	& _##PER##_##REG##_##FIELD##_MASK)

#define _CCL_PER_FMK(PER,REG,FIELD,x) (\
	(_CCL_VALUEOF(_##PER##_##REG##_TYPE,x)<<_CCL_PER_FSHIFT(PER,REG,FIELD))\
	&_CCL_PER_FMASK(PER,REG,FIELD)\
	)

/*
#define _CCL_PER_FMKS(PER,REG,FIELD,SYM) (\
	(_CCL_PER_FSYM(##PER,REG,FIELD,SYM)<<_CCL_PER_FSHIFT(##PER,REG,FIELD))\
	&_CCL_PER_FMASK(##PER,REG,FIELD)\
	)
*/
#define _CCL_PER_FMKS(PER,REG,FIELD,SYM) (\
	(PER##_##REG##_OF(_CCL_PER_FSYM(PER,REG,FIELD,SYM))<<_CCL_PER_FSHIFT(PER,REG,FIELD))\
	&_CCL_PER_FMASK(PER,REG,FIELD)\
	)

#define _CCL_PER_FEXTRACT(PER,REG,FIELD,reg) (_##PER##_##REG##_TYPE)(\
	((_CCL_VALUEOF(_##PER##_##REG##_TYPE,reg)&_CCL_PER_FMASK(PER,REG,FIELD))\
	>>_CCL_PER_FSHIFT(PER,REG,FIELD))\
	)

#define _CCL_PER_FINSERT(PER,REG,FIELD,reg,field) (_##PER##_##REG##_TYPE)(\
	((PER##_##REG##_OF(reg)&~PER##_##REG##_OF(_CCL_PER_FMASK(PER,REG,FIELD)))|\
	((PER##_##REG##_OF(field)<<_CCL_PER_FSHIFT(PER,REG,FIELD))\
	&_CCL_PER_FMASK(PER,REG,FIELD)))\
	)

/*******************************************************/
/* macros for memmory mapped registers                 */
/*******************************************************/

#define _CCL_PER_RAOI(addr,PER,REG,_and,_or,_inv)\
    (*(volatile _##PER##_##REG##_TYPE*)(addr))=(\
      ((((*(volatile _##PER##_##REG##_TYPE*)(addr))\
      &((_##PER##_##REG##_TYPE)(_and)))\
      |((_##PER##_##REG##_TYPE)(_or)))\
      ^((_##PER##_##REG##_TYPE)(_inv)))\
    )

#define _CCL_PER_RGET(addr,PER,REG,datshft,datwidth)   \
    ((_CCL_VALUEOF(_##PER##_##REG##_TYPE,_##PER##_REG_READ(addr))>>datshft)&datwidth)         /* For memory mapped registers use: (((*(volatile uint32_t*)(addr))>>datshft)&datwidth) */

#define _CCL_PER_RSET(addr,PER,REG,x,datshft,datwidth) \
    _##PER##_REG_WRITE(addr, (_CCL_VALUEOF(_##PER##_##REG##_TYPE,(x))&(datwidth))<<datshft) /* For memory mapped registers use: (*(volatile uint32_t*)(addr))=((uint32_t)((x)&(datwidth))<<datshft) */

#define _CCL_PER_RSYM(PER,REG,SYM) \
	PER##_##REG##_##SYM

#define _CCL_PER_RSETS(addr,PER,REG,SYM,datshft,datwidth) \
	_CCL_PER_RSET(addr,PER,REG,_CCL_PER_RSYM(PER,REG,SYM),datshft,datwidth)

#define _CCL_PER_FGET(addr,PER,REG,FIELD,datshft,datwidth) \
	_CCL_PER_FEXTRACT(PER,REG,FIELD,_CCL_PER_RGET(addr,PER,REG,datshft,datwidth))

#define _CCL_PER_FSET(addr,PER,REG,FIELD,field,datshft,datwidth) \
	_CCL_PER_RSET(addr,PER,REG,\
	_CCL_PER_FINSERT(PER,REG,FIELD,_CCL_PER_RGET(addr,PER,REG,datshft,datwidth),field),datshft,datwidth)

#define _CCL_PER_FSETS(addr,PER,REG,FIELD,SYM,datshft,datwidth) \
	_CCL_PER_RSET(addr,PER,REG,_CCL_PER_FINSERT(PER,REG,FIELD,_CCL_PER_RGET(addr,PER,REG,datshft,datwidth),\
	_CCL_PER_FSYM(PER,REG,FIELD,SYM)),datshft,datwidth)

/*******************************************************/
/* macros for CPU control registers                    */
/*******************************************************/

#define _CCL_PER_CRGET(PER,REG) \
	REG

#define _CCL_PER_CRSET(PER,REG,reg) \
	REG=((uint32_t)(reg))

#define _CCL_PER_CFGET(PER,REG,FIELD) \
	_CCL_PER_FEXTRACT(PER,REG,FIELD,_CCL_PER_CRGET(PER,REG))

#define _CCL_PER_CFSET(PER,REG,FIELD,field)\
	_CCL_PER_CRSET(PER,REG,\
	_CCL_PER_FINSERT(PER,REG,FIELD,_PER_CRGET(PER,REG),field))

#define _CCL_PER_CFSETS(PER,REG,FIELD,SYM)\
	_CCL_PER_CRSET(PER,REG,\
	_CCL_PER_FINSERT(PER,REG,FIELD,_CCL_PER_CRGET(PER,REG),\
	_CCL_PER_FSYM(PER,REG,FIELD,SYM)))


#endif	/* !__C_CHIP_LIB_STANDARD_HAL_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * end of std_hal.h                                                        *
\*-------------------------------------------------------------------------*/
