
#ifdef __WT5700__

void WT5700_init(void);
UINT8 WT5700_readkey(void);

#else


extern void WT5700_init(void);
extern void WT5700_power_off_init(void);


extern UINT8 WT5700_readkey(void);
#endif

