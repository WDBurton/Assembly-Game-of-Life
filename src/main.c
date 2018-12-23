#include <stdio.h>
#include <stdlib.h>

//Definitions

// Number of cells in a byte, for calloc.  It is assumed to be 4.  Try to keep the number a perfect fit.
#define CELLS_IN_BYTE 4

// Max size for height and length
// NOTE:  Using Height and Length instead of Column and Rows for my ease of visualizing the array.  The height is the number of rows, the length is the number of columns
#define HMAX 256
#define LMAX 256

// Default size for height and length
#define HDEFAULT 20
#define LDEFAULT 20


int main( int argc, char *argv[] )
{

  printf("You should enter in the height, then the length, of the Game of Life you wish to play.  For now, we just use %d and %d, respectively, for testing purposes.\n", HDEFAULT, LDEFAULT);

  /*
   *  The array here is pretty interesting.  As the goal is to make it all work in assembly, what we need is an array of bits... but C works in bytes at the smallest.  Which is
   *  eight bits, each.
   * 
   *  Now, to make things more interesting, we don't want just a single array of bits.  Because then we'd be editing the structure as we alter it.  So what we actually want
   *  is two bit arrays.  And because I apparently hate myself, I want to make that into one, to master array iteration in Assembly.  Yaaay meeeee... so, every entry requires
   *  two bits.  I'm going to call this a 'cell'.  A byte is 8 bits, a cell is 2 bits, therefor the CELLS_IN_BYTE are 4.
   *
   *  We're not done yet though!  To make the assembly run faster, we want a minimum of possible checks possible.  Therefor, we need to alter the number of chars for it to make
   *  it work a bit smoother.  As a remender:  One 'cell' is two bits, and a byte holds CELLS_IN_BYTE cells.  We shall assume that the length start at 'startRows/CELLS_IN_BYTE'
   *  byte's, plus one for buffer assumption.  The height, however, is done in single bits units.  So we take startHeight/8, plus one for buffer assumption.
   *
   *  The length needs to be one cells longer to the left and the right.  If length%SIZE is 1, that means that the added buffer only has empty space equal to 1 cell.  That is not enough.
   *  Therefor, we add 1.
   *
   *  We don't need to adjust the height.  Either it fits perfectly, in which case the default added 1 is enough, or it doesn't.  In which case there is a space of at least one cell
   *  left over by the added one, which is enough of a buffer.
   *
   *  In summary:
   *  Each 'cell' is two bits.  One for if the cell is alive, one for if the cell Will be alive.
   *  Two cells are the size of one char.
   *  We will add two cells to the the length.  We assume a buffer of one.  If length%size == 1, add 1.  This is an important edge case.
   *  We will add one cell to the height.  However, given the assumed buffer of 1, there is no way this will not be fit, so we do not need to adjust it at all.
   *
   *  tldr:  The array will be:
   *  height  = preHeight/8 + 1
   *  length  = preLength/CELLS_IN_BYTE + 1 + (if preLength%CELLS_IN_BYTE == 1: 1  else: 0)
   *  Char [height][length];
  */
  

  // TODO:  Make cols and rows accept inputs if they are not null and are less than COLMAX and ROWMAX, respectively.  If the inputs are null, use the defaults like below.
  int height = HDEFAULT;
  int length = LDEFAULT;
  int lAdd = 0;

  // Basic work, as explained above.  Height is height size / 8 + 1.
  // Length is length size / CELLS_IN_BYTE + 1, and another + 1 if and only if size % CELLS_IN_BYTE == 1. 
  height = height/8 + 1;
  if(length%CELLS_IN_BYTE == 1){
    lAdd = 1;
  }
  length = length/8 + 1 + lAdd;
  
  // Need to use some kind of memory allocation.  I use Calloc as we're assuming that the array starts filled with nothing but 0's.
  char *gameBoard = (char *) calloc( height * length, 1 );
  
  
  /*printf("Welp.  Back to the silly solution.  Silly solution HO!\n");
  printf("This is a test for how many lines I can see with ease\n");
  
  for(int i = 0; i <= 40; i ++){
    printf("%d---1----2----3----4----5----6----7----8----9---10---11---12---13---14---15---16---17---18---19---20\n", i);
    }*/

  printf("Test Info:\n");
  printf("Number of bytes needed for the height: %d\n", height);
  printf("Number of bytes needed for the length: %d\n", length);
  free(gameBoard);
  return( 0 );
}
