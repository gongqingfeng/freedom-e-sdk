// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

// Structures for registering different interrupt handlers
// for different parts of the application.
typedef void (*function_ptr_t) (void);

void no_interrupt_handler (void) {};

function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];


// Instance data for the PLIC.

plic_instance_t g_plic;


/*Entry Point for PLIC Interrupt Handler*/
void handle_m_ext_interrupt(){
  plic_source int_num  = PLIC_claim_interrupt(&g_plic);
  if ((int_num >=1 ) && (int_num < PLIC_NUM_INTERRUPTS)) {
    g_ext_interrupt_handlers[int_num]();
  }
  else {
    exit(1 + (uintptr_t) int_num);
  }
  PLIC_complete_interrupt(&g_plic, int_num);
}


/*Entry Point for Machine Timer Interrupt Handler*/
void handle_m_time_interrupt(){

  clear_csr(mie, MIP_MTIP);

  // Reset the timer for 3s in the future.
  // This also clears the existing timer interrupt.

  volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
  volatile uint64_t * mtimecmp    = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIMECMP);
  uint64_t now = *mtime;
  uint64_t then = now + 2 * RTC_FREQ;
  *mtimecmp = then;

  // read the current value of the LEDS and invert them.
  uint32_t leds = GPIO_REG(GPIO_OUTPUT_VAL);

  GPIO_REG(GPIO_OUTPUT_VAL) ^= ((0x1 << RED_LED_OFFSET)   |
				(0x1 << GREEN_LED_OFFSET) |
				(0x1 << BLUE_LED_OFFSET));

  // Re-enable the timer interrupt.
  set_csr(mie, MIP_MTIP);

}
int main(int argc, char **argv)
{
  //GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< LED_0_R_OFFSET) | (0x1 << LED_0_G_OFFSET) | (0x1 << LED_0_B_OFFSET) | (0x1 << CK_IO_12_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< LED_0_R_OFFSET) | (0x1 << LED_0_G_OFFSET) | (0x1 << LED_0_B_OFFSET) | (0x1 << CK_IO_12_OFFSET)) ;
  //GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << LED_0_R_OFFSET) ;
  //GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET) | (0x1 << BLUE_LED_OFFSET) | (0x1 << LED_0_R_OFFSET)) ;
  while (1){

  }

  return 0;

}
