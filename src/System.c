#include "System.h"


//Copyright 2015 Palouse RoboSub Club

/*
  This file is part of Sublibinal.

    Sublibinal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sublibinal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sublibinal.  If not, see <http://www.gnu.org/licenses/>.
 */


void disable_Interrupts()
{
    asm volatile ("di");
}

void enable_Interrupts()
{
    INTCON |= 1<<12;
    asm volatile ("nop"); //this nop is crucial. It resolves a race condition between a configuration of the CPU register INTCON
                          //and an IRQ. The NOP allows the CPU to configure for multi vector mode before it allows IRQs to enter.
    asm volatile("ei");
}

