#ifndef __PORT_H
#define __PORT_H

#define store_x          sd
#define load_x           ld

#define portWORD_SIZE         (8)
#define portSTACK_FRAME_SIZE  (32*portWORD_SIZE)

 #define portSTACK_RESERVED_IDX                (31)
 #define portSTACK_SSTATUS_IDX                 (30)
 #define portSTACK_XCRITICALNESTING_IDX        (29)
 #define portSTACK_X31_IDX                     (28)
 #define portSTACK_X30_IDX                     (27)
 #define portSTACK_X29_IDX                     (26)
 #define portSTACK_X28_IDX                     (25)
 #define portSTACK_X27_IDX                     (24)
 #define portSTACK_X26_IDX                     (23)
 #define portSTACK_X25_IDX                     (22)
 #define portSTACK_X24_IDX                     (21)
 #define portSTACK_X23_IDX                     (20)
 #define portSTACK_X22_IDX                     (19)
 #define portSTACK_X21_IDX                     (18)
 #define portSTACK_X20_IDX                     (17)
 #define portSTACK_X19_IDX                     (16)
 #define portSTACK_X18_IDX                     (15)
 #define portSTACK_X17_IDX                     (14)
 #define portSTACK_X16_IDX                     (13)
 #define portSTACK_X15_IDX                     (12)
 #define portSTACK_X14_IDX                     (11)
 #define portSTACK_X13_IDX                     (10)
 #define portSTACK_X12_IDX                     ( 9)
 #define portSTACK_X11_IDX                     ( 8)
 #define portSTACK_X10_IDX                     ( 7)
 #define portSTACK_X9_IDX                      ( 6)
 #define portSTACK_X8_IDX                      ( 5)
 #define portSTACK_X7_IDX                      ( 4)
 #define portSTACK_X6_IDX                      ( 3)
 #define portSTACK_X5_IDX                      ( 2)
 #define portSTACK_X1_IDX                      ( 1)
 #define portSTACK_PXCODE_IDX                  ( 0)

 #define portSTACK_PVPARAMETERS_IDX            (portSTACK_X10_IDX)
 #define portSTACK_PORTTASK_RETURN_ADDRESS_IDX (portSTACK_X1_IDX)

#endif
