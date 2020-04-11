typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

/** WDOG - Peripheral register structure */
typedef struct WDOG_MemMap {
  uint16_t STCTRLH;                                /**< Watchdog Status and Control Register High, offset: 0x0 */
  uint16_t STCTRLL;                                /**< Watchdog Status and Control Register Low, offset: 0x2 */
  uint16_t TOVALH;                                 /**< Watchdog Time-out Value Register High, offset: 0x4 */
  uint16_t TOVALL;                                 /**< Watchdog Time-out Value Register Low, offset: 0x6 */
  uint16_t WINH;                                   /**< Watchdog Window Register High, offset: 0x8 */
  uint16_t WINL;                                   /**< Watchdog Window Register Low, offset: 0xA */
  uint16_t REFRESH;                                /**< Watchdog Refresh Register, offset: 0xC */
  uint16_t UNLOCK;                                 /**< Watchdog Unlock Register, offset: 0xE */
  uint16_t TMROUTH;                                /**< Watchdog Timer Output Register High, offset: 0x10 */
  uint16_t TMROUTL;                                /**< Watchdog Timer Output Register Low, offset: 0x12 */
  uint16_t RSTCNT;                                 /**< Watchdog Reset Count Register, offset: 0x14 */
  uint16_t PRESC;                                  /**< Watchdog Prescaler Register, offset: 0x16 */
} volatile * WDOG_MemMapPtr;

#define WDOG_BASE_PTR                            ((WDOG_MemMapPtr)0x4003A000u)

/* ----------------------------------------------------------------------------
   -- WDOG - Register accessor macros
   ---------------------------------------------------------------------------- */

/* WDOG - Register accessors */
#define WDOG_STCTRLH_REG(base)                   ((base)->STCTRLH)
#define WDOG_STCTRLL_REG(base)                   ((base)->STCTRLL)
#define WDOG_TOVALH_REG(base)                    ((base)->TOVALH)
#define WDOG_TOVALL_REG(base)                    ((base)->TOVALL)
#define WDOG_WINH_REG(base)                      ((base)->WINH)
#define WDOG_WINL_REG(base)                      ((base)->WINL)
#define WDOG_REFRESH_REG(base)                   ((base)->REFRESH)
#define WDOG_UNLOCK_REG(base)                    ((base)->UNLOCK)
#define WDOG_TMROUTH_REG(base)                   ((base)->TMROUTH)
#define WDOG_TMROUTL_REG(base)                   ((base)->TMROUTL)
#define WDOG_RSTCNT_REG(base)                    ((base)->RSTCNT)
#define WDOG_PRESC_REG(base)                     ((base)->PRESC)

/* WDOG - Register instance definitions */
/* WDOG */
#define WDOG_STCTRLH                             WDOG_STCTRLH_REG(WDOG_BASE_PTR)
#define WDOG_STCTRLL                             WDOG_STCTRLL_REG(WDOG_BASE_PTR)
#define WDOG_TOVALH                              WDOG_TOVALH_REG(WDOG_BASE_PTR)
#define WDOG_TOVALL                              WDOG_TOVALL_REG(WDOG_BASE_PTR)
#define WDOG_WINH                                WDOG_WINH_REG(WDOG_BASE_PTR)
#define WDOG_WINL                                WDOG_WINL_REG(WDOG_BASE_PTR)
#define WDOG_REFRESH                             WDOG_REFRESH_REG(WDOG_BASE_PTR)
#define WDOG_UNLOCK                              WDOG_UNLOCK_REG(WDOG_BASE_PTR)
#define WDOG_TMROUTH                             WDOG_TMROUTH_REG(WDOG_BASE_PTR)
#define WDOG_TMROUTL                             WDOG_TMROUTL_REG(WDOG_BASE_PTR)
#define WDOG_RSTCNT                              WDOG_RSTCNT_REG(WDOG_BASE_PTR)
#define WDOG_PRESC                               WDOG_PRESC_REG(WDOG_BASE_PTR)

/** FTMRA - Peripheral register structure */
typedef struct FTMRA_MemMap {
  uint8_t FSEC;                                    /**< Flash Security Register, offset: 0x0 */
  uint8_t FCLKDIV;                                 /**< Flash Clock Divider Register, offset: 0x1 */
  uint8_t FECCRIX;                                 /**< Flash ECCR Index Register, offset: 0x2 */
  uint8_t FCCOBIX;                                 /**< Flash Common Command Object Index Register, offset: 0x3 */
  uint8_t FERCNFG;                                 /**< Flash Error Configuration Register, offset: 0x4 */
  uint8_t FCNFG;                                   /**< Flash Configuration Register, offset: 0x5 */
  uint8_t FERSTAT;                                 /**< Flash Error Status Register, offset: 0x6 */
  uint8_t FSTAT;                                   /**< Flash Status Register, offset: 0x7 */
  uint8_t DFPROT;                                  /**< D-Flash Protection Register, offset: 0x8 */
  uint8_t FPROT;                                   /**< P-Flash Protection Register, offset: 0x9 */
  uint8_t FCCOBLO;                                 /**< Flash Common Command Object Low Register, offset: 0xA */
  uint8_t FCCOBHI;                                 /**< Flash Common Command Object High Register, offset: 0xB */
  uint8_t RESERVED_0[2];
  uint8_t FECCRLO;                                 /**< Flash ECC Error Results Low Register, offset: 0xE */
  uint8_t FECCRHI;                                 /**< Flash ECC Error Results High Register, offset: 0xF */
  uint8_t RESERVED_1[1];
  uint8_t FOPT;                                    /**< Flash Option Register, offset: 0x11 */
} volatile *FTMRA_MemMapPtr;

/* FTMRA - Peripheral instance base addresses */
/** Peripheral FTMRA base pointer */
#define FTMRA_BASE_PTR                           ((FTMRA_MemMapPtr)0x40039000u)
/* FTMRA - Register accessors */
#define FTMRA_FSEC_REG(base)                     ((base)->FSEC)
#define FTMRA_FCLKDIV_REG(base)                  ((base)->FCLKDIV)
#define FTMRA_FECCRIX_REG(base)                  ((base)->FECCRIX)
#define FTMRA_FCCOBIX_REG(base)                  ((base)->FCCOBIX)
#define FTMRA_FERCNFG_REG(base)                  ((base)->FERCNFG)
#define FTMRA_FCNFG_REG(base)                    ((base)->FCNFG)
#define FTMRA_FERSTAT_REG(base)                  ((base)->FERSTAT)
#define FTMRA_FSTAT_REG(base)                    ((base)->FSTAT)
#define FTMRA_DFPROT_REG(base)                   ((base)->DFPROT)
#define FTMRA_FPROT_REG(base)                    ((base)->FPROT)
#define FTMRA_FCCOBLO_REG(base)                  ((base)->FCCOBLO)
#define FTMRA_FCCOBHI_REG(base)                  ((base)->FCCOBHI)
#define FTMRA_FECCRLO_REG(base)                  ((base)->FECCRLO)
#define FTMRA_FECCRHI_REG(base)                  ((base)->FECCRHI)
#define FTMRA_FOPT_REG(base)                     ((base)->FOPT)


/* FTMRA - Register instance definitions */
/* FTMRA */
#define FTMRA_FSEC                               FTMRA_FSEC_REG(FTMRA_BASE_PTR)
#define FTMRA_FCLKDIV                            FTMRA_FCLKDIV_REG(FTMRA_BASE_PTR)
#define FTMRA_FECCRIX                            FTMRA_FECCRIX_REG(FTMRA_BASE_PTR)
#define FTMRA_FCCOBIX                            FTMRA_FCCOBIX_REG(FTMRA_BASE_PTR)
#define FTMRA_FERCNFG                            FTMRA_FERCNFG_REG(FTMRA_BASE_PTR)
#define FTMRA_FCNFG                              FTMRA_FCNFG_REG(FTMRA_BASE_PTR)
#define FTMRA_FERSTAT                            FTMRA_FERSTAT_REG(FTMRA_BASE_PTR)
#define FTMRA_FSTAT                              FTMRA_FSTAT_REG(FTMRA_BASE_PTR)
#define FTMRA_DFPROT                             FTMRA_DFPROT_REG(FTMRA_BASE_PTR)
#define FTMRA_FPROT                              FTMRA_FPROT_REG(FTMRA_BASE_PTR)
#define FTMRA_FCCOBLO                            FTMRA_FCCOBLO_REG(FTMRA_BASE_PTR)
#define FTMRA_FCCOBHI                            FTMRA_FCCOBHI_REG(FTMRA_BASE_PTR)
#define FTMRA_FECCRLO                            FTMRA_FECCRLO_REG(FTMRA_BASE_PTR)
#define FTMRA_FECCRHI                            FTMRA_FECCRHI_REG(FTMRA_BASE_PTR)
#define FTMRA_FOPT                               FTMRA_FOPT_REG(FTMRA_BASE_PTR)
