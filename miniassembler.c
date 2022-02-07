/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Charles Coppiters't Wallant + Stephane Pienaar             */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <assert.h>
#include <stddef.h>

/*--------------------------------------------------------------------*/
/* Modify *puiDest in place,
   setting uiNumBits starting at uiDestStartBit (where 0 indicates
   the least significant bit) with bits taken from uiSrc,
   starting at uiSrcStartBit.
   uiSrcStartBit indicates the rightmost bit in the field.
   setField sets the appropriate bits in *puiDest to 1.
   setField never unsets any bits in *puiDest.                        */
static void setField(unsigned int uiSrc, unsigned int uiSrcStartBit,
                     unsigned int *puiDest, unsigned int uiDestStartBit,
                     unsigned int uiNumBits)
{
   unsigned int i = 0;
   unsigned int refinedSrc = 0;

   /* Making an in int with in for for the bits required to be copied 
      in the src string. Building the int from right to left.  */
   while (i < uiNumBits) {
      refinedSrc = refinedSrc << 1;
      refinedSrc = refinedSrc | 1;
      i++;
   }

   /* shifting left until uiSrcStartBit*/
   refinedSrc = refinedSrc << uiSrcStartBit;

   /* and'ing the result with uiSrc so that refinedSrc only includes the
      necessary bits to be inserted into puiDest */
   refinedSrc = uiSrc & refinedSrc;

   /* shift back to the right */
   refinedSrc = refinedSrc >> uiSrcStartBit;
   
   /* shifting so that the bits to be inserted aligns with puiDest */
   refinedSrc = refinedSrc << uiDestStartBit;
   
   /* or'ing to insert bits into refinedSrc*/
   *puiDest = *puiDest | refinedSrc;


}

/*--------------------------------------------------------------------*/

/* Return the machine language equivalent of "mov reg, immed" where    
   reg is a W register. 
 
   Parameters:                                 
      uiReg: the number of reg.  0 <= uiReg <= 31.                    
      iImmed: the immed value. -32768 <= iImmed <= 32767      */

unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   unsigned int uiInstr;

   assert(uiReg <= 31);
   assert(iImmed >= -32768 && iImmed <= 32767);
   
   /* Base Instruction Code */
   /* for w register: 0101 0010 100[0 0000 0000 0000 000][0 0000] */
   uiInstr = 0x52800000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* Vaue to be inserted in instruction */
   setField(iImmed, 0, &uiInstr, 5, 16);

   return uiInstr;

}

/*--------------------------------------------------------------------*/

/* Return the machine language equivalent of "adr reg, addr".

   Parameters:
      uiReg: the number of reg. 0 <= uiReg <= 31.
      ulAddr: the address denoted by addr.
      ulAddrOfThisInstr: the address of the adr instruction itself
      (must be a multiple of 4).                   */

unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
                               unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x10000000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* displacement to be split into immlo and immhi and inserted */
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   setField(uiDisp, 0, &uiInstr, 29, 2);
   setField(uiDisp, 2, &uiInstr, 5, 19);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

/* Return the machine language equivalent of "strb fromreg,[toreg]",
   where fromreg is a W register and toreg is a X register.
   Parameters:
      uiFromReg: the number of fromreg. 0 <= uiFromReg <= 31.
      uiToReg: the number of toreg.     0 <= uiToReg <= 31.           */

unsigned int MiniAssembler_strb(unsigned int uiFromReg,
                                unsigned int uiToReg)
{
   unsigned int uiInstr;

   assert(uiFromReg <= 31);
   assert(uiToReg <= 31);
   
   /* Base Instruction Code */
   /* for w register: 0011 1001 00[0 0000 0000 00][00 000][0 0000] */
   uiInstr = 0x39000000;

   /* destination register (really source) */
   setField(uiFromReg, 0, &uiInstr, 0, 5);

   /* source register (really destination) */
   setField(uiToReg, 0, &uiInstr, 5, 5);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

/* Return the machine language equivalent of "b addr".
   Parameters:
      ulAddr: the address denoted by addr, that is, the address to
         which the branch should occur (must be a multiple of 4).
      ulAddrOfThisInstr: the address of the b instruction itself
      (must be a multiple of 4).                                   */

unsigned int MiniAssembler_b(unsigned long ulAddr,
                             unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   assert(ulAddr % 4 == 0);
   assert(ulAddrOfThisInstr % 4 == 0);

   /* Base Instruction Code */
   /* 0001 01[00 0000 0000 0000 0000 0000 0000] */
   uiInstr = 0x14000000;

   /* Defining offset and adding to instruction */
   uiDisp = (unsigned int) (ulAddr - ulAddrOfThisInstr);
   setField(uiDisp, 2, &uiInstr, 0, 26);

   return uiInstr;
}
/*--------------------------------------------------------------------*/

/* Return the machine language equivalent of "bl addr".
   Parameters:
      ulAddr: the address denoted by addr, that is, the address to
         which the branch should occur (must be a multiple of 4).
      ulAddrOfThisInstr: the address of the b instruction itself
      (must be a multiple of 4).                                   */

unsigned int MiniAssembler_bl(unsigned long ulAddr,
                             unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   assert(ulAddr % 4 == 0);
   assert(ulAddrOfThisInstr % 4 == 0);

   /* Base Instruction Code */
   /* 1001 01[00 0000 0000 0000 0000 0000 0000] */
   uiInstr = 0x94000000;

   /* Defining offset and adding to instruction */
   uiDisp = (unsigned int) (ulAddr - ulAddrOfThisInstr);
   setField(uiDisp, 2, &uiInstr, 0, 26);

   return uiInstr;
}
