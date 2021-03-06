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

// Later functions
void displayBoard( void* board, int height, int lBytes, int lActual );


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
   *  byte's, plus one for buffer assumption.  The height, however, is simply the number of rows.  But because we want a buffer, we add two, so we get two extra rows.
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
   *  height  = preHeight + 2
   *  length  = preLength/CELLS_IN_BYTE + 1 + (if preLength%CELLS_IN_BYTE == 1: 1  else: 0)
   *  Char [height][length];
  */
  

  // TODO:  Make cols and rows accept inputs if they are not null and are less than COLMAX and ROWMAX, respectively.  If the inputs are null, use the defaults like below.
  int heightActual = HDEFAULT;
  int lengthActual = LDEFAULT;
  int lAdd = 0;

  // Basic work, as explained above.  Height is heightActual + 2
  // Length is length size / CELLS_IN_BYTE + 1, and another + 1 if and only if size % CELLS_IN_BYTE == 1. 
  int height = heightActual + 2;
  if(lengthActual%CELLS_IN_BYTE == 1){
    lAdd = 1;
  }
  int length = lengthActual/CELLS_IN_BYTE + 1 + lAdd;
  int totalSize = height*length;
  
  // Need to use some kind of memory allocation.  I use Calloc as we're assuming that the array starts filled with nothing but 0's.
  char *gameBoard = (char *) calloc( totalSize, 2 );

  // Initialization just to be safe
  /*for( int h = 0; h < heightActual; h++ ){
      for( int l = 0; l < length; l++){
        gameBoard[ h*length + l] = 0b00000000;
      }
    }*/

  printf("Test Info:\n");
  printf("Number of bytes needed for the height: %d\n", height);
  printf("Number of bytes needed for the length: %d\n", length);
  displayBoard( gameBoard, heightActual, length, lengthActual );
  free(gameBoard);
  return( 0 );
}

// This is the displayBoard function.  It requires the pointer to the board, the number of bytes in the height and row, and the actual size of the array we're using.
// This is for testing purposes to ensure that the board works and that I don't crash and destroy everything.
// I could put everything needed into global variables, but I'd rather do this to ensure that I understand everything properly, and make it more portable.
void displayBoard( void* board, int height, int lBytes, int lActual ){
  
  /*
   * ASSUMPTIONS
   ***
   ***  The board has an extra row on the top and bottom
   ***  The board is given from the actual starting position, not the starting position of the first 'actual' row
   ***  The board starts on the bit we want to read
   *
   * We shall start by assuring ourselves that the size of a char is 1.  As that's what we'll be using for the arithmatic here.  If it isn't, then we are in trouble,
   * so just return as is.
   *
   * For appearances sake, we then put in a row of - 's, which will be of the length of the actual length.  I'll also put in an empty space, so it covers only the actual
   * length of the array.
   *
   * Up next comes some tricky bits.  I need to check every two bits for the length of the array, ignoring the first two, and print out what the result is.  As this is
   * done in C and not in Assembly, this is going to be somewhat tricky to do Properly... so I'm going to do it in a cheaty way, and just do it for all of them.  This will
   * be done for every unit of height.
   *
   * So, for the full height, I then:
   *   For every unit of length, I then:
   *     For CELLS_IN_BYTE, I then:
   *       Check the the first bit
   *       If that bit is 0, put out an O.  If that bit is 1, put out an X
   *       Move the 'check bit' forward by two.
   * 
   */

  // The initial test
  if( sizeof(char) != 1 ){
    printf("FAILURE!  Char is not one byte!  It is %d bytes!  This test function will not work!\n", sizeof(char));
    return;
  }
  // The extra newline is to make it prettier!
  printf("Commencing test display\n\n");

  // The row of -'s for appearance
  printf(" ");
  for(int l = 0; l < lActual; l ++ ) {
    printf("-");
  }
  printf("\n");

  // This is the annoyingly complicated 'check the bits' section.  First I turn the board into a char pointer for
  // the pointer arithmatic.
  char* boardActual = (char *) board;

  // I then push boardActual forward by the size of one row, to get to the actual start
  boardActual = boardActual + lBytes;

  
  // NOTE:  MANUAL TESTING GOES HERE
  // This turns on only what is currently seen
  boardActual[0] = 0x55;

  // This turns on only what is not seen
  boardActual[1] = 0xAA;

  // This turns on both
  boardActual[lBytes * 19] = 0xFF;
  boardActual[lBytes - 1] = 0xFF;

  // I go for hActual for the full height, and lBytes for the arithmatic
  for( int h = 0; h < height; h ++ ){
    for( int l = 0; l < lBytes; l ++ ){
      // I need the current byte for this.  For that, I need to move forward by:
      // current height * bytes in length
      // Plus the current lByte we are on
      char curByte = *(boardActual + h*lBytes + l);
      //printf("Byte %d\n", h*lBytes + l);
      for( int curCell = 0; curCell < CELLS_IN_BYTE; curCell ++ ){
	// For each cell, I make the curBit variable.  Then I shift it by the cell number, times two.  This is what
	// I need to check against.
	char curBit = 0b1000000;
	curBit = curBit >> (curCell * 2);
	// Then I need to check if curBit & curByte = curBit.  If so, there is a 1 in the location I'm looking for.
	// If there is a 1 there, then I output an X.  Otherwise, I output a O
	if( (curBit & curByte) == curBit ){
	  printf("X");
	} else {
	  printf("O");
	}
	
      }
      //printf("TEST");
    }
    // Now that the full row is done, time to go down a line.
    printf("\n");
  }

  
  
}
