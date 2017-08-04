//
//
//
#ifndef _PCB_CONF_DEF_H
#define _PCB_CONF_DEF_H

// Crystal frequency
#define _XTAL_10M				(10000000)
#define _XTAL_27M				(27000000)

//GPIO definition
// Input/Output
#define	GPIO_DIRECTION_INPUT_PIN			0
#define	GPIO_DIRECTION_OUTPUT_PIN		1

// Low/High Active
#define	GPIO_ACTICE_LOW					0
#define	GPIO_ACTIVE_HIGH					1

// De-bounce clock base.
#define	GPIO_DEBOUNCE_CLOCK_30ms			7
#define	GPIO_DEBOUNCE_CLOCK_20ms			6
#define	GPIO_DEBOUNCE_CLOCK_10ms			5
#define	GPIO_DEBOUNCE_CLOCK_1ms				4
#define	GPIO_DEBOUNCE_CLOCK_100us			3
#define	GPIO_DEBOUNCE_CLOCK_10us			2
#define	GPIO_DEBOUNCE_CLOCK_1us				1
#define	GPIO_DEBOUNCE_CLOCK_37ns			0

typedef enum
{
    GPIO0 = 0,
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    GPIO8,
    GPIO9,
    GPIO10,
    GPIO11,
    GPIO12,
    GPIO13,
    GPIO14,
    GPIO15,
    GPIO16,
    GPIO17,
    GPIO18,
    GPIO19,
    GPIO20,
    GPIO21,
    GPIO22,
    GPIO23,
    GPIO24,
    GPIO25,
    GPIO26,
    GPIO27,
    GPIO28,
    GPIO29,
    GPIO30,
    GPIO31,
    GPIO32,
    GPIO33,
    GPIO34,
    GPIO35,
    GPIO36,
    GPIO37,
    GPIO38,
    AGPO0,
    AGPO1,
    AGPO2,
    AGPO3,
    AGPO4,
    AGPO5,
    AGPO6,
    AGPO7,
    AGPO8,
    AGPO9,
    AGPO10,
    AGPO11,
    AGPO12,
    AGPO13,
    AGPO14,
    AGPO15,
    AGPO16,
    AGPO17,
    AGPO18,
    AGPO19,
    AGPO20,
    AGPO21,
    AGPO22,
    AGPO23,
    AGPO24,
    AGPO25,
    AGPO26,
    AGPO27,
    GPIO_TOTAL
} GPIONUM;

#define GPIO_A1			GPIO0
#define GPIO_A2			GPIO1
#define GPIO_A3			GPIO2
#define GPIO_A4			GPIO3
#define GPIO_A5			GPIO4
#define GPIO_A6			GPIO5
#define GPIO_A7			GPIO6
#define GPIO_B0			GPIO7
#define GPIO_B1			GPIO8
#define GPIO_B2			GPIO9
#define GPIO_B3			GPIO10
#define GPIO_C0			GPIO11
#define GPIO_C1			GPIO12
#define GPIO_C2			GPIO13
#define GPIO_C3			GPIO14
#define GPIO_C4			GPIO15
#define GPIO_C5			GPIO16
#define GPIO_D0			GPIO17
#define GPIO_D1			GPIO18
#define GPIO_G0			GPIO19
#define GPIO_G1			GPIO20
#define GPIO_G2			GPIO21
#define GPIO_G3			GPIO22
#define GPIO_G4			GPIO23
#define GPIO_G5			GPIO24
#define GPIO_G6			GPIO25
#define GPIO_G7			GPIO26
#define GPIO_H0			GPIO27
#define GPIO_H1			GPIO28
#define GPIO_H2			GPIO29
#define GPIO_H3			GPIO30
#define GPIO_H4			GPIO31
#define GPIO_H5			GPIO32
#define GPIO_H6			GPIO33
#define GPIO_H7			GPIO34
#define GPIO_I4			GPIO35
#define GPIO_I5			GPIO36
#define GPIO_I6			GPIO37
#define GPIO_I7			GPIO38
#define AGPO_LSADC0	AGPO0
#define AGPO_LSADC1	AGPO1
#define AGPO_LSADC2	AGPO2
#define AGPO_LSADC3	AGPO3
#define AGPO_AIO_2L		AGPO4
#define AGPO_AIO_2R	       AGPO5
#define AGPO_VIN00P		AGPO6
#define AGPO_VIN10P		AGPO7
#define AGPO_VIN01P		AGPO8
#define AGPO_VIN11P		AGPO9
#define AGPO_VIN0N		AGPO10
#define AGPO_VIN02P		AGPO11
#define AGPO_VIN12P		AGPO12
#define AGPO_VIN1N		AGPO13
#define AGPO_VIN03P		AGPO14
#define AGPO_RGBSW0	AGPO15
#define AGPO_AIN_2L		AGPO16
#define AGPO_RGBSW1	AGPO17
#define AGPO_AIN_2R	AGPO18
#define AGPO_AVOUT2	AGPO19
#define AGPO_B2N		AGPO20
#define AGPO_B2P		AGPO21
#define AGPO_G2N		AGPO22
#define AGPO_G2P		AGPO23
#define AGPO_R2N		AGPO24
#define AGPO_R2P		AGPO25
#define AGPO_AIN_5L		AGPO26
#define AGPO_AIN_5R	AGPO27

enum PWM_NO
{
    PWM_0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_TOTAL
};

#define AUDIO_AMP_MUTE		1//-0
#define AUDIO_AMP_UNMUTE		0//-1

#define LED_ON 					0
#define LED_OFF 					1//Tim 0325

#define PVR_SEL			 		1
#define AV_SEL 					0

#define TUNERPOWER_ENABLE 	1
#define TUNERPOWER_DISABLE 	0

#define PVR_ENABLE 			1
#define PVR_DISABLE 		0

#endif // #ifndef _PCB_GPIO_DEF_H

