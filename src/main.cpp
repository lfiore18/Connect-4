#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// y 
//
// 5 | 5  11
// 4 | 4  10
// 3 | 3   9
// 2 | 2   8  ...
// 1 | 1   7  13
// 0 | 0   6  12
//   |--------------------------
//     0 | 1 | 2 | 3 | 4 | 5 | 6     x



// CURRENT PROBLEMS: Win checking in columns is bugged, winning cases on the far right side of the board are not detected.
// Possible issue: loops in "Column" function break too early to feed "GetIndex" right values.


const size_t BOARD_WIDTH  = 7;
const size_t BOARD_HEIGHT = 6;

const size_t NULL_BOARD_INDEX = std::numeric_limits<size_t>::max();

enum class Cell { Empty, Red, Yellow };
using Board = std::array<Cell, BOARD_WIDTH * BOARD_HEIGHT>;

size_t GetIndex(size_t x, size_t y) {
	return (x * BOARD_HEIGHT) + y;
}

Cell CheckForWinnerInColumn(const Board& board) {
	Cell result = Cell::Empty;

	// Search for winners in columns
	// Iterate through every column across the width of the board
	bool found_winner = false;

	for (size_t i = 0; i < BOARD_WIDTH; i++) {

		int counter = 0;
		Cell current_colour = Cell::Empty;

		// Move up the column starting at the bottom cell (j = 0)
		for (size_t j = 0; j < BOARD_HEIGHT; j++) {
			// Our board is stored as a 1D array. Find the index of the cell
			// into our 1D array given the x, y coordinates
			size_t index = GetIndex(i, j);

			//std::cout << index << std::endl;

			// The cell matches the colour that we saw in the cell below,
			// increment the counter by one (once we reach 4 we know we've
			// found a match)
			if (board[index] == current_colour) {
				counter++;
			}
			// The cell is a different colour, and we didn't match 4 (because
			// we would have broken out of this loop already - see below).
			// Reset the counter to 1 and change the currently tracked colour
			// to the new colour.
			else {
				counter = 1;
				current_colour = board[index];
			}

			// This is the 4th cell we found with the same colour, we found a
			// column of 4! We've found a winner, break out of the loop
			// 
			// If the current colour is "Cell::Empty" we disregard the counter
			// as we've found 4 consecutive empty cells
			if (counter == 4 && current_colour != Cell::Empty) {
				found_winner = true;
				result = current_colour;
				break;
			}
		}

		// Found a winner, break out of the outer loop of searching each column
		if (found_winner) {
			break;
		}
	}

	return result;
}

Cell CheckForWinnerInRow(const Board& board) {
	Cell result = Cell::Empty;

	// Search for winners in rows
	// Iterate through every row across the height of the board
	bool found_winner = false;

	for (size_t j = 0; j < BOARD_HEIGHT; j++) {

		int counter = 0;
		Cell current_colour = Cell::Empty;

		for (size_t i = 0; i < BOARD_WIDTH; i++) {
			size_t index = GetIndex(i, j);

			if (board[index] == current_colour) {
				counter++;
			}
			else {
				counter = 1;
				current_colour = board[index];
			}

			if (counter == 4 && current_colour != Cell::Empty) {
				found_winner = true;
				result = current_colour;
				break;
			}
		}

		if (found_winner) {
			break;
		}
	}

	return result;
}

Cell CheckForWinnerDiagonalUpRight(const Board& board) {
	Cell result = Cell::Empty;

	// Starting from Column 0, Row 0
		// Begin looping from bottom left to top right by adding an incremented offset 
		// to the X and Y co-ordinates e.g. for [0, 3] offset = [1, 4]

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 3; j++) {
			
			int counter = 0;
			Cell current_colour = Cell::Empty;

			for (size_t offset = 0; (i + offset < BOARD_WIDTH) && (j + offset < BOARD_HEIGHT); offset++) {
				size_t index = GetIndex(i + offset, j + offset);

				if (board[index] == current_colour) {
					counter++;
				}
				else {
					counter = 1;
					current_colour = board[index];
				}

				if (counter == 4 && current_colour != Cell::Empty) {
					result = current_colour;
					return result;
				}
			}
		}
	}

	return result;
}

Cell CheckForWinnerDiagonalUpLeft(const Board& board) {
	Cell result = Cell::Empty;


	for (size_t i = BOARD_WIDTH - 1; i > 2; i--) {

		for (size_t j = 0; j < 3; j++) {
			int counter = 0;
			Cell current_colour = Cell::Empty;
			for (size_t offset = 0; (i - offset != NULL_BOARD_INDEX) && (j + offset < BOARD_HEIGHT); offset++) {
				size_t index = GetIndex(i - offset, j + offset);

				if (board[index] == current_colour) {
					counter++;
				}
				else {
					counter = 1;
					current_colour = board[index];
				}

				if (counter == 4 && current_colour != Cell::Empty) {
					result = current_colour;
					return result;
				}
			}
		}
	}

	return result;
}

std::string CellToResultString(Cell cell) {
	switch (cell) {
	case Cell::Red:    return "Red";
	case Cell::Yellow: return "Yellow";
	case Cell::Empty:
	default: return "Draw";
	}
}

std::string PlayConnectFour(std::vector<std::string> pieces_position_list)
{
	// Storage for our board
	Board board{};
	board.fill(Cell::Empty);

	Cell result;

	// Iterate through the pieces list
	for (size_t i = 0; i < pieces_position_list.size(); i++) {
        // Parse input data

		// Get column index
		char column_letter = pieces_position_list[i][0];
		size_t column_index = static_cast<size_t>(column_letter) - 'A';

		char colour_letter = pieces_position_list[i][2];

		// Find Cell
		Cell cell = Cell::Empty;
		
		if (colour_letter == 'R') {
			cell = Cell::Red;
		} else {
			cell = Cell::Yellow;
		}

		// Multiply column index by 6 to find the beginning of column
		// Loop over column to find the first Empty Cell

		size_t index = NULL_BOARD_INDEX;

		for (size_t j = 0; j < BOARD_HEIGHT; j++) {
			size_t current_index = GetIndex(column_index, j);
			if (board[current_index] == Cell::Empty) {
				index = current_index;
				break;
			}
		}

		if (index == NULL_BOARD_INDEX) {
			return "Invalid";
		}

        board[index] = cell;

		std::cout << "\"" << pieces_position_list[i] << "\"," << std::endl;
		
		// Iterate through each column
			// On each column index, loop through each row
				// Check to see current colour occupies cell
				// If it does, increment counter
				// If it's occupied by the opposite colour, reset counter and set current colour to opposite colour
				// If it's occupied by nothing, break
			// If four consecutive pieces are found, set boolean "found winner" to true

		// Convert the winner cell type to the string that the test wants ("Draw", "Red" or "Yellow")

		if (result = CheckForWinnerInColumn(board); result != Cell::Empty) {
			return CellToResultString(result);
		}
		else if (result = CheckForWinnerInRow(board); result != Cell::Empty) {
			return CellToResultString(result);
		}
		else if (result = CheckForWinnerDiagonalUpRight(board); result != Cell::Empty) {
			return CellToResultString(result);
		}
		else if (result = CheckForWinnerDiagonalUpLeft(board); result != Cell::Empty) {
			return CellToResultString(result);
		}
	}
	
	return CellToResultString(Cell::Empty);
}

int main() {
	// 1 Ask user for column to place piece
	// 2 Calculate where piece needs to go
	// 3 Run win checks
	// > If Winner:
    //     - Display winner in terminal, ask to restart
	// > Else:
	//     - Go to step 1

	std::cout << "Welcome to Connect FOUR!!!" << std::endl;

	std::cout << "Please input a column (A-H):"
	std::string user_input;
	std::cin << user_input;

	return 0;
}

// int main()
// {
// 	std::vector<std::vector<std::string>> test_cases = {
// 		{
// 			"A_Red",
// 			"A_Red",
// 			"A_Red",
// 			"A_Red",
// 			"A_Red",
// 			"A_Red",
// 			"A_Red",
// 		},
// 		{
//             "A_Red",
//             "B_Yellow",
//             "A_Red",
//             "B_Yellow",
//             "A_Red",
//             "B_Yellow",
//             "G_Red",
//             "B_Yellow"
// 		},
// 		{
// 			"A_Red",
// 			"B_Red",
// 			"C_Red",
// 			"D_Red"
// 		},
// 		{
// 			"A_Red",
// 			"B_Red",
// 			"B_Red",
// 			"C_Red",
// 			"C_Red",
// 			"C_Red",
// 			"D_Yellow",
// 			"D_Yellow",
// 			"D_Yellow",
// 			"D_Red"
// 		},
// 		{
// 			"D_Red",
// 			"E_Red",
// 			"E_Red",
// 			"F_Red",
// 			"F_Red",
// 			"F_Red",
// 			"G_Yellow",
// 			"G_Yellow",
// 			"G_Yellow",
// 			"G_Red"
// 		},
// 		{
// 			"A_Red",
// 			"A_Red",
// 			"A_Red",
// 			"A_Yellow",
// 			"B_Yellow",
// 			"B_Yellow",
// 			"B_Yellow",
// 			"C_Yellow",
// 			"C_Yellow",
// 			"D_Yellow",
// 		},
// 		{
// 			"A_Red",
// 			"B_Yellow",
// 			"A_Red",
// 			"B_Yellow",
// 			"A_Red",
// 			"B_Yellow",
// 			"G_Red",
// 			"B_Yellow"
// 		},
// 		{
// 			"C_Yellow",
// 			"E_Red",
// 			"G_Yellow",
// 			"B_Red",
// 			"D_Yellow",
// 			"B_Red",
// 			"B_Yellow",
// 			"G_Red",
// 			"C_Yellow",
// 			"C_Red",
// 			"D_Yellow",
// 			"F_Red",
// 			"E_Yellow",
// 			"A_Red",
// 			"A_Yellow",
// 			"G_Red",
// 			"A_Yellow",
// 			"F_Red",
// 			"F_Yellow",
// 			"D_Red",
// 			"B_Yellow",
// 			"E_Red",
// 			"D_Yellow",
// 			"A_Red",
// 			"G_Yellow",
// 			"D_Red",
// 			"D_Yellow",
// 			"C_Red"
// 		},
// 		{
// 			"A_Yellow",
// 			"B_Red",
// 			"B_Yellow",
// 			"C_Red",
// 			"G_Yellow",
// 			"C_Red",
// 			"C_Yellow",
// 			"D_Red",
// 			"G_Yellow",
// 			"D_Red",
// 			"G_Yellow",
// 			"D_Red",
// 			"F_Yellow",
// 			"E_Red",
// 			"D_Yellow"
// 		},
// 		{
// 			"F_Yellow",
// 			"G_Red",
// 			"D_Yellow",
// 			"C_Red",
// 			"A_Yellow",
// 			"A_Red",
// 			"E_Yellow",
// 			"D_Red",
// 			"D_Yellow",
// 			"F_Red",
// 			"B_Yellow",
// 			"E_Red",
// 			"C_Yellow",
// 			"D_Red",
// 			"F_Yellow",
// 			"D_Red",
// 			"D_Yellow",
// 			"F_Red",
// 			"G_Yellow",
// 			"C_Red",
// 			"F_Yellow",
// 			"E_Red",
// 			"A_Yellow",
// 			"A_Red",
// 			"C_Yellow",
// 			"B_Red",
// 			"E_Yellow",
// 			"C_Red",
// 			"E_Yellow",
// 			"G_Red",
// 			"A_Yellow",
// 			"A_Red",
// 			"G_Yellow",
// 			"C_Red",
// 			"B_Yellow",
// 			"E_Red",
// 			"F_Yellow",
// 			"G_Red",
// 			"G_Yellow",
// 			"B_Red",
// 			"B_Yellow",
// 			"B_Red"
// 		},
// 		{
// 			"C_Yellow",
// 			"B_Red",
// 			"B_Yellow",
// 			"E_Red",
// 			"D_Yellow",
// 			"G_Red",
// 			"B_Yellow",
// 			"G_Red",
// 			"E_Yellow",
// 			"A_Red",
// 			"G_Yellow",
// 			"C_Red",
// 			"A_Yellow",
// 			"A_Red",
// 			"D_Yellow",
// 			"B_Red",
// 			"G_Yellow",
// 			"A_Red",
// 			"F_Yellow",
// 			"B_Red",
// 			"D_Yellow",
// 			"A_Red",
// 			"F_Yellow",
// 			"F_Red",
// 			"B_Yellow",
// 			"F_Red",
// 			"F_Yellow",
// 			"G_Red",
// 			"A_Yellow",
// 			"F_Red",
// 			"C_Yellow",
// 			"C_Red",
// 			"G_Yellow",
// 			"C_Red",
// 			"D_Yellow",
// 			"D_Red",
// 			"E_Yellow",
// 			"D_Red",
// 			"E_Yellow",
// 			"C_Red",
// 			"E_Yellow",
// 			"E_Red"
// 		},
// 		{
// 			"B_Yellow",
// 			"G_Red",
// 			"G_Yellow",
// 			"C_Red",
// 			"C_Yellow",
// 			"G_Red",
// 			"F_Yellow",
// 			"A_Red",
// 			"D_Yellow",
// 			"D_Red",
// 			"A_Yellow",
// 			"B_Red",
// 			"E_Yellow",
// 			"F_Red",
// 			"F_Yellow",
// 			"G_Red",
// 			"F_Yellow",
// 			"G_Red",
// 			"C_Yellow",
// 			"B_Red",
// 			"E_Yellow",
// 			"C_Red",
// 			"C_Yellow",
// 			"E_Red",
// 			"E_Yellow",
// 			"E_Red",
// 			"E_Yellow",
// 			"G_Red",
// 			"C_Yellow"
// 		},
// 		{
// 			"G_Red",
// 			"G_Red",
// 			"G_Red",
// 			"G_Red"
// 		},
// 		{
// 			"E_Yellow",
// 			"C_Red",
// 			"A_Yellow",
// 			"G_Red",
// 			"A_Yellow",
// 			"B_Red",
// 			"E_Yellow",
// 			"C_Red",
// 			"G_Yellow",
// 			"A_Red",
// 			"A_Yellow",
// 			"B_Red",
// 			"B_Yellow",
// 			"F_Red",
// 			"D_Yellow",
// 			"G_Red",
// 			"C_Yellow",
// 			"G_Red",
// 			"A_Yellow",
// 			"C_Red",
// 			"G_Yellow",
// 			"F_Red",
// 			"B_Yellow"
// 		}
// 	};
// 
// 	for (size_t i = 0; i < test_cases.size(); ++i) {
// 		const auto& test_case = test_cases[i];
// 		std::string result = PlayConnectFour(test_case);
// 
// 		std::cout << "Case " << i << ": " << result << std::endl;
// 	}
// 
// 	// 1 Ask user for column to place piece
// 	// 2 Calculate where piece needs to go
// 	// 3 Run win checks
// 	// > If Winner:
//     //     - Display winner in terminal, ask to restart
// 	// > Else:
// 	//     - Go to step 1
// 
// 	return 0;
// }

// |
// |
// | Y
// | R   Y
// | R   Y
// | R   Y                  Y 
// |--------------------------
//   A | B | C | D | E | F | G

// 5 | 5  11
// 4 | 4  10
// 3 | 3   9
// 2 | 2   8  ...
// 1 | 1   7  13
// 0 | 0   6  12
//   |--------------------------
//     0 | 1 | 2 | 3 | 4 | 5 | 6


//Connect Four
//Take a look at wiki description of Connect Four game :
//
//Wiki Connect Four
//
//The grid is 6 row by 7 columns, those being named from A to G.
//
//You will receive a list of strings showing the order of the pieces which dropped in columns :
//
//std::vector<std::string> pieces_position_list
//{
//  "A_Red",
//  "B_Yellow",
//  "A_Red",
//  "B_Yellow",
//  "A_Red",
//  "B_Yellow",
//  "G_Red",
//  "B_Yellow"
//}
//The list may contain up to 42 moves and shows the order the players are playing.
//
//The first player who connects four items of the same color is the winner.
//
//You should return "Yellow", "Red" or "Draw" accordingly.