/*----------------------------------------------------------------------------*/
/*      Définition des fonctions de bas-niveau permettant de manipuler les    */
/*   interruptions.                                                           */
/*                                                  (C) Manu Chaput 2000-2021 */
/*----------------------------------------------------------------------------*/
#ifndef INTER_BAS_NIVEAU_DEF
#define INTER_BAS_NIVEAU_DEF

void stubHandlerNop(void);
/*
 * Un handler de bas niveau qui ne fait rien
 */

/**
 * Les gestionnaires bas niveau des exceptions
 */
void stubHandlerExDiv0(void);
void stubHandlerExDebug(void);
void stubHandlerExNMI(void);
void stubHandlerExBreakpoint(void);
void stubHandlerExOverflow(void);
void stubHandlerExBoundExceeded(void);
void stubHandlerExDeviceInvalidOpcode(void);
void stubHandlerExDeviceUnavailable(void);
void stubHandlerExDoubleFault(void);
void stubHandlerExCoproOverrun(void);
void stubHandlerExInvalidTSS(void);
void stubHandlerExSegmentNotPresent(void);
void stubHandlerExStackSegmentFault(void);
void stubHandlerExGeneralProtectionFault(void);
void stubHandlerExPageFault(void);
void stubHandlerExReserved(void);
void stubHandlerExFloatingPoint(void);
void stubHandlerExAlignmentCheck(void);
void stubHandlerExFloatingMachineCheck(void);
void stubHandlerExFloatingSIMDFPE(void);
void stubHandlerExFloatingVirtualization(void);
void stubHandlerExControlProtection(void);
void stubHandlerExReserved2(void);
void stubHandlerExReserved3(void);
void stubHandlerExReserved4(void);
void stubHandlerExReserved5(void);
void stubHandlerExReserved6(void);
void stubHandlerExReserved7(void);
void stubHandlerExHypervisionInjection(void);
void stubHandlerExVMMCommunication(void);
void stubHandlerExSecurity(void);
void stubHandlerExReserved8(void);


/**
 * Les gestionnaires bas niveau des IRQ
 */
void stubHandlerIRQ0(void);
void stubHandlerIRQ1(void);
void stubHandlerIRQ2(void);
void stubHandlerIRQ3(void);
void stubHandlerIRQ4(void);
void stubHandlerIRQ5(void);
void stubHandlerIRQ6(void);
void stubHandlerIRQ7(void);
void stubHandlerIRQ8(void);
void stubHandlerIRQ9(void);
void stubHandlerIRQ10(void);
void stubHandlerIRQ11(void);
void stubHandlerIRQ12(void);
void stubHandlerIRQ13(void);
void stubHandlerIRQ14(void);
void stubHandlerIRQ15(void);

void stubHandlerInt66(void);

void initialiserHandlersInterruption(void * table, uint32_t taille);
#endif
