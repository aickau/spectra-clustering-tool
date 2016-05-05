#include <stdio.h>

#include "xparameters.h"
#include "xil_cache.h"
#include "xintc.h"
#include "intc_header.h"
#include "xemaclite.h"
#include "xemaclite_example.h"
#include "emaclite_header.h"
#include "emaclite_intr_header.h"
#include "xtmrctr.h"
#include "tmrctr_header.h"
#include "tmrctr_intr_header.h"

// ==================
#include "xafaprocess_hw.h"
#include "interrupts.h"
#include "afa_types.h"


void AfaInit()
{
}

int
main()
{
    static XIntc intc;
    static XEmacLite axi_ethernetlite_0_EmacLite;
    static XTmrCtr axi_timer_0_Timer;
    Xil_ICacheEnable();
//    Xil_DCacheEnable();
   
    xil_printf( "---Entering main--- ======================================================\n\r" );

    InterruptInit();
    XAfaprocess_hw_Init();
    InterruptEnable();

    AfaTestApp();
#if 0
    {
      int status;

      print("\r\n Running IntcSelfTestExample() for microblaze_0_axi_intc...\r\n");

      status = IntcSelfTestExample(XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);

      if (status == 0) {
         print("IntcSelfTestExample PASSED\r\n");
      }
      else {
         print("IntcSelfTestExample FAILED\r\n");
      }
   }
   {
       int Status;

       Status = IntcInterruptSetup(&intc, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
       if (Status == 0) {
          print("Intc Interrupt Setup PASSED\r\n");
       }
       else {
         print("Intc Interrupt Setup FAILED\r\n");
      }
   }
   {
      int status;

      print("\r\nRunning EmacLitePolledExample() for axi_ethernetlite_0...\r\n");
      status = EmacLitePolledExample(XPAR_AXI_ETHERNETLITE_0_DEVICE_ID);
      if (status == 0) {
         print("EmacLite Polled Example PASSED\r\n");
      }
      else {
         print("EmacLite Polled Example FAILED\r\n");
      }
   }
   {
      int Status;

      print("\r\n Running Interrupt Test  for axi_ethernetlite_0...\r\n");

      Status = EmacLiteIntrExample(&intc, &axi_ethernetlite_0_EmacLite, \
                               XPAR_AXI_ETHERNETLITE_0_DEVICE_ID, \
                               XPAR_MICROBLAZE_0_AXI_INTC_AXI_ETHERNETLITE_0_IP2INTC_IRPT_INTR);

      if (Status == 0) {
         print("EmacLite Interrupt Test PASSED\r\n");
      }
      else {
         print("EmacLite Interrupt Test FAILED\r\n");
      }
   }
   {
      int status;
      
      print("\r\n Running TmrCtrSelfTestExample() for axi_timer_0...\r\n");
      
      status = TmrCtrSelfTestExample(XPAR_AXI_TIMER_0_DEVICE_ID, 0x0);
      
      if (status == 0) {
         print("TmrCtrSelfTestExample PASSED\r\n");
      }
      else {
         print("TmrCtrSelfTestExample FAILED\r\n");
      }
   }
   {
      int Status;

      print("\r\n Running Interrupt Test  for axi_timer_0...\r\n");
      
      Status = TmrCtrIntrExample(&intc, &axi_timer_0_Timer, \
                                 XPAR_AXI_TIMER_0_DEVICE_ID, \
                                 XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR, 0);
	
      if (Status == 0) {
         print("Timer Interrupt Test PASSED\r\n");
      } 
      else {
         print("Timer Interrupt Test FAILED\r\n");
      }

   }
   
   /*
    * Peripheral SelfTest will not be run for axi_uartlite_0
    * because it has been selected as the STDOUT device
    */
#endif

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}
