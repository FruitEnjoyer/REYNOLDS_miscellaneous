/**
 * @file LPS22HB_defines.h
 * @brief LPS22HB pressure sensor defines
 * @date 29.01.2026
 * @author Ruslan Valeev
 */

#ifndef LPS22HB_DEFINES_H_
#define LPS22HB_DEFINES_H_

#define LPS22HB_I2CADDR_R  0b1011101
#define LPS22HB_I2CADDR_W  0b1011100

/*----Register addresses----*/               // зачем нужен
#define LPS22HB_REGADDR_INTERRUPT_CFG  0x0B  // R/W включить возможность генерировать прерывания при событиях
#define LPS22HB_REGADDR_THS_P_L        0x0C  // R/W можно самому задать пороговое давление
#define LPS22HB_REGADDR_THS_P_H        0x0D  // R/W можно самому задать пороговое давление
#define LPS22HB_REGADDR_WHO_AM_I       0x0F  // хто я
#define LPS22HB_REGADDR_CTRL_REG1      0x10  // R/W частота измерений, настройка фильтра, тонкости считывания данных, SPI
#define LPS22HB_REGADDR_CTRL_REG2      0x11  // R/W reboot, fifo, инкрементится ли адрес при чтении (включи), I2C, soft reset
#define LPS22HB_REGADDR_CTRL_REG3      0x12  // R/W настройка прерываний и пина DRDY
#define LPS22HB_REGADDR_FIFO_CTRL      0x14  // R/W режим фифо
#define LPS22HB_REGADDR_REF_P_XL       0x15  // R/W опорное давление
#define LPS22HB_REGADDR_REF_P_L        0x16  // R/W опорное давление
#define LPS22HB_REGADDR_REF_P_H        0x17  // R/W опроное давление
#define LPS22HB_REGADDR_RPDS_L         0x18  // R/W смещение давления
#define LPS22HB_REGADDR_RPDS_H         0x19  // R/W смещение давления
#define LPS22HB_REGADDR_RES_CONF       0x1A  // R/W low-power режим
#define LPS22HB_REGADDR_INT_SOURCE     0x25  // R инфа о boot и есть ли прерывания
#define LPS22HB_REGADDR_FIFO_STATUS    0x26  // R инфа о фифо
#define LPS22HB_REGADDR_STATUS         0x27  // R инфа есть ли новые данные
#define LPS22HB_REGADDR_PRESS_OUT_XL   0x28  // читать давление
#define LPS22HB_REGADDR_PRESS_OUT_L    0x29  // читать давление
#define LPS22HB_REGADDR_PRESS_OUT_H    0x2A  // читать давление
#define LPS22HB_REGADDR_TEMP_OUT_L     0x2B  // читать температуру
#define LPS22HB_REGADDR_TEMP_OUT_H     0x2C  // читать температуру
#define LPS22HB_REGADDR_LPFP_RES       0x33  // R для обнуления фильтра


/*----Register defaults----*/
#define LPS22HB_REGDEFAULT_INTERRUPT_CFG  0b00000000
#define LPS22HB_REGDEFAULT_THS_P_L        0b00000000
#define LPS22HB_REGDEFAULT_THS_P_H        0b00000000
#define LPS22HB_REGDEFAULT_WHO_AM_I       0b10110001
#define LPS22HB_REGDEFAULT_CTRL_REG1      0b00000000
#define LPS22HB_REGDEFAULT_CTRL_REG2      0b00010000
#define LPS22HB_REGDEFAULT_CTRL_REG3      0b00000000
#define LPS22HB_REGDEFAULT_FIFO_CTRL      0b00000000
#define LPS22HB_REGDEFAULT_REF_P_XL       0b00000000
#define LPS22HB_REGDEFAULT_REF_P_L        0b00000000
#define LPS22HB_REGDEFAULT_REF_P_H        0b00000000
#define LPS22HB_REGDEFAULT_RPDS_L         0b00000000
#define LPS22HB_REGDEFAULT_RPDS_H         0b00000000
#define LPS22HB_REGDEFAULT_RES_CONF       0b00000000


/*------Register masks------*/
/*    INTERRUPT_CFG    */
#define LPS22HB_INTERRUPTCFG_AUTORIFP  0b10000000
#define LPS22HB_INTERRUPTCFG_RESETARP  0b01000000
#define LPS22HB_INTERRUPTCFG_AUTOZERO  0b00100000
#define LPS22HB_INTERRUPTCFG_RESETAZ   0b00010000
#define LPS22HB_INTERRUPTCFG_DIFFEN    0b00001000
#define LPS22HB_INTERRUPTCFG_LIR       0b00000100
#define LPS22HB_INTERRUPTCFG_PLE       0b00000010
#define LPS22HB_INTERRUPTCFG_PHE       0b00000001

/*    CTRL_REG1    */
#define LPS22HB_CTRLREG1_ODR       0b01110000
#define LPS22HB_CTRLREG1_ENLPFP    0b00001000
#define LPS22HB_CTRLREG1_LPFP_CFG  0b00000100
#define LPS22HB_CTRLREG1_BDU       0b00000010
#define LPS22HB_CTRLREG1_SIM       0b00000001

/*    CTRL_REG2    */
#define LPS22HB_CTRLREG2_BOOT       0b10000000
#define LPS22HB_CTRLREG2_FIFOEN     0b01000000
#define LPS22HB_CTRLREG2_STOPONFTH  0b00100000
#define LPS22HB_CTRLREG2_IFADDINC   0b00010000
#define LPS22HB_CTRLREG2_I2CDIS     0b00001000
#define LPS22HB_CTRLREG2_SWRESET    0b00000100
#define LPS22HB_CTRLREG2_ONESHOT    0b00000001

/*    CTRL_REG3    */
#define LPS22HB_CTRLREG3_INTHL  0b10000000
#define LPS22HB_CTRLREG3_PPOD   0b01000000
#define LPS22HB_CTRLREG3_FFSS5  0b00100000
#define LPS22HB_CTRLREG3_FFTH   0b00010000
#define LPS22HB_CTRLREG3_FOVR   0b00001000
#define LPS22HB_CTRLREG3_DRDY   0b00000100
#define LPS22HB_CTRLREG3_INTS   0b00000011

/*    FIFO_CTRL    */
#define LPS22HB_FIFOCTRL_FMODE  0b11100000
#define LPS22HB_FIFOCTRL_WTM    0b00011111

/*    RES_CONF    */
#define LPS22HB_RESCONF_LCEN  0b00000001

/*    INT_SOURCE    */
#define LPS22HB_INTSOURCE_BOOTSTATUS  0b10000000
#define LPS22HB_INTSOURCE_IA          0b00000100
#define LPS22HB_INTSOURCE_PL          0b00000010
#define LPS22HB_INTSOURCE_PH          0b00000001

/*    FIFO_STATUS    */
#define LPS22HB_FIFOSTATUS_FTH_FIFO  0b10000000
#define LPS22HB_FIFOSTATUS_OVR       0b01000000
#define LPS22HB_FIFOSTATUS_FSS       0b00111111

/*    STATUS    */
#define LPS22HB_STATUS_TOR  0b00100000
#define LPS22HB_STATUS_POR  0b00010000
#define LPS22HB_STATUS_TDA  0b00000010
#define LPS22HB_STATUS_PDA  0b00000001

#endif /* LPS22HB_DEFINES_H_ */
