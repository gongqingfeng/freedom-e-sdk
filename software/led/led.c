#include <stdio.h>
#include "platform.h"
#include <unistd.h>

static unsigned long get_cpu_freq()
{
  return 65000000;
}

static void uart_init(size_t baud_rate)
{
  GPIO_REG(GPIO_IOF_SEL) &= ~IOF0_UART0_MASK;
  GPIO_REG(GPIO_IOF_EN) |= IOF0_UART0_MASK;
  UART0_REG(UART_REG_DIV) = (get_cpu_freq() / 2) / baud_rate - 1;
  UART0_REG(UART_REG_TXCTRL) |= UART_TXEN;
}



const char * instructions_msg = " \
\n\
                SandSilicon\n\
\n\
         5555555555555555555555555\n\
        5555                   5555\n\
       5555                     5555\n\
      5555                       5555\n\
     5555       5555555555555555555555\n\
    5555       555555555555555555555555\n\
   5555                             5555\n\
  5555                               5555\n\
 5555                                 5555\n\
5555555555555555555555555555          55555\n\
 55555           555555555           55555\n\
   55555           55555           55555\n\
     55555           5           55555\n\
       55555                   55555\n\
         55555               55555\n\
           55555           55555\n\
             55555       55555\n\
               55555   55555\n\
                 555555555\n\
                   55555\n\
                     5\n\
\n\
SandSilicon Ebaz4205 FPGA Board 'led' program.\n\
 ";

void print_instructions() {
  write (STDOUT_FILENO, instructions_msg, strlen(instructions_msg));
}

int main()
{
  uart_init(115200);
  print_instructions();
  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< LED_0_R_OFFSET) | (0x1 << LED_0_G_OFFSET) | (0x1 << LED_0_B_OFFSET) | (0x1 << CK_IO_12_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< LED_0_R_OFFSET) | (0x1 << LED_0_B_OFFSET) | (0x1 << CK_IO_12_OFFSET)) ;
  //GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << LED_0_R_OFFSET) ;
  //GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET) | (0x1 << BLUE_LED_OFFSET) | (0x1 << LED_0_R_OFFSET)) ;
  while(1);
  return 0;
}
