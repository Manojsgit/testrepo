#include <stdio.h>
#define ZERO 0
#define ONE 1
#define ASCI 65
void print(int [][8]);
int input_check(char, int);
void initial_setup_set(int [][8]);
int position_check(int [][8], int, int, char);
int  possible_moves(int [][8], int, int, char, int*, int*);
int get_end_move(int[][8], int *, int *, int, char, int);
int main()
{

	int board[8][8], user = 0, result, end_result;
	char choice, start_col;
	int start_row, position_valid, input_valid = 0;
	int black_row[] = { 1, 1, 1, 2 };       //possible combination of rows for black
	int black_col[] = { -1, 1, 0, 0 };      //possible combination of columns black
	int white_row[] = { -1, -1, -1, -2 };   //possible combination of rows white
	int white_col[] = { -1, 1, 0, 0 };      //possible combination of columns white

	printf("CHESS GAME begins\n");
	initial_setup_set(board);
	print(board);
	int user_check = user % 2; // to account either white or black user
	(user_check) ? (printf("black user\n")) : (printf("white user\n"));
	while (1) {
		printf("want to continue the game Y/n\n");
		scanf(" %c", &choice);
		switch (choice) {
		case 'Y':
		case 'y':;
			break;
		case 'N':
		case 'n': return 0;
		default: printf("enter proper option\n");
			continue;
		}
		while (1) {
			printf("enter the starting position\n");
			printf("like A8 A2 B2 H2\n");
			scanf(" %c", &start_col);
			scanf(" %d", &start_row);
			start_col = start_col & '_';
			input_valid = input_check(start_col, start_row);
			if (input_valid == ZERO)
				continue;
			position_valid = position_check(board, user_check, start_row, start_col);

			if (position_valid == ZERO)
				continue;

			if (user_check == ZERO)  // if user_check is ZERO means white user and ONE means black user
				result = possible_moves(board, user_check, start_row, start_col, white_row, white_col);
			else
				result = possible_moves(board, user_check, start_row, start_col, black_row, black_col);
			if (result == ZERO) { //zero means to possible moves
				printf("please enter a position for different pawn\n");
				continue;
			}else {
				if (user_check)
					end_result = get_end_move(board, black_row, black_col, start_row, start_col, user_check);
				else
					end_result = get_end_move(board, white_row, white_col, start_row, start_col, user_check);
				if (end_result == ZERO)
					continue;
			}
			break;
		}
		user = user + 1;
		user_check = user % 2; // to account either white or black user
		(user_check) ? (printf("black user\n")) : (printf("white user\n"));
	}
	return 0;
}



/**
 * @brief this function is used to check entered position is correct or not
 *
 * @param board[][8] is 2d array were chess is played
 * @param user is use to identify either white or black user
 * @param row_pos is the user entered row position
 * @param col_pos is the user entered column positon
 *
 * @return  0 when fails and 1 on success
 *
 */
int position_check(int board[][8], int user, int row_pos, char col_pos)
{
	int ret;

	if (user == 0) {
		//printf("white user\n");
		((board[row_pos - ONE][col_pos - ASCI]) == '0') ? ( ret = 1) : (ret = 0);
		return ret;
	}else {
		//printf("black user\n");
		((board[row_pos - ONE][col_pos - ASCI]) == '1') ? ( ret = 1) : (ret = 0);
		return ret;
	}
}



/**
 * @brief this function is use to make the initial setup of board
 *
 * @param board[][8] is 2d array were chess is played
 */
void initial_setup_set(int board[][8])
{
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (i == 1)
				board[i][j] = '1';
			else if (i == 6)
				board[i][j] = '0';
			else
				board[i][j] = '_';
		}
	}
}



/**
 * @brief this function is used to print the board
 *
 * @param board[][8] is 2d array were chess is played
 */

void print(int board[][8])
{
	int row_value = 1, i, j;
	char column_name = 'A';

	printf("  ");
	for (i = 0; i < 8; i++)
		printf("%c ", column_name + i);
	printf("\n");
	for (i = 0; i < 8; i++) {
		printf("%d ", row_value++);
		for (j = 0; j < 8; j++)
			printf("%c ", board[i][j]);
		printf("\n");
	}
	printf("\n");
	printf("1-BLACK PAWNS 0- WHITE PAWNS\n");
}


/**
 * @brief this function is used to give possible moves for
 * selected pawn
 *
 * @param board[][8] is the 2d array were chess is palyed
 * @param user specifies either black or white
 * @param row is user entered row position
 * @param col is user entered col position
 * @param p_row is the possible combinations of row
 * @param p_col is the possible combinations of column
 *
 * @return  0 on failure of no moves
 */
int  possible_moves(int board[][8], int user, int row, char col, int *p_row, int *p_col)
{
	int i, no_moves = 0;
	static int result[4];

	char our_pawn, opp_pawn;

	if (user == 0) {               //white user
		our_pawn = '0';
		opp_pawn = '1';
	}else {                       //black user
		our_pawn = '1';
		opp_pawn = '0';
	}
	for (i = 0; i < 4; i++)
		result[i] = ZERO;
	printf("\nPOSSIBLE MOVES\n");
	for (i = 0; i < 2; i++) {
		if (board[row + p_row[i] - ONE][col - ASCI + p_col[i]] == opp_pawn) {

			result[i] = 1;

			printf(" %c%d ", col + p_col[i], row + p_row[i]);
		}
	}
	if (board[row + p_row[2] - ONE][col - ASCI + p_col[2]] != opp_pawn && board[row + p_row[2] - ONE][col - ASCI + p_col[2]] != our_pawn && row != 1 && row != 8) {

		result[2] = 1;

		printf("%c%d ", col + p_col[2], row + p_row[2]);
	}
	if (((row == 2 && our_pawn == '1' ) || (row == 7 && our_pawn == '0')) && result[2] == 1 && board[row + p_row[3] - ONE][col - ASCI + p_col[3]] != opp_pawn) {

		result[3] = 1;

		printf("%c%d \n", col + p_col[3], row + p_row[3]);
	}

	for (i = 0; i < 4; i++)           //if there is a possible move the value will be 1 or i twill be 0
		if (result[i] == ZERO)
			no_moves = no_moves + 1;

	if (no_moves == 4) {
		printf("no more possible moves for that pawn\n");
		return 0;
	}
	return 1;
}



/**
 * @brief this function is used to get the get end move
 *
 * @param board[][8] is the 2d array wer chess is played
 * @param moves_row is possible row combinations
 * @param moves_col is possible column combinations
 * @param start_row is user entered row position
 * @param start_col is user entered col position
 * @param user is specifying either black or white
 *
 * @return 0 on failure and 1 on success
 */
int get_end_move(int board[][8], int *moves_row, int *moves_col, int start_row, char start_col, int user)
{
	int endpos_row, temp_row, temp_col, iterator, number = 1, input_valid;
	char endpos_col;

	while (1) {
		printf("\nENTER THE END MoVE\n");
		printf("Enter q/Q to deselect the pawn\n");
		scanf(" %c", &endpos_col);
		endpos_col = endpos_col & '_'; //converting to uppercase
		if (endpos_col == 'Q') {
			printf("now you can select the different pawn\n");
			return 0;
		}
		scanf(" %d", &endpos_row);
		input_valid = input_check(endpos_col, endpos_row);
		if (input_valid == ZERO)
			continue;
		temp_row =  endpos_row - start_row;
		temp_col =  endpos_col - start_col;
		printf(" row%d col%d \n", temp_row, temp_col);
		for (iterator = 0; iterator < 4; iterator++) {
			if (moves_row[iterator] == temp_row && moves_col[iterator] == temp_col ) {
				number = 0;
				break;
			}
		}
		if (number) {
			printf("enter only possible moves\n");
			continue;
		}

		if (user) {
			board[start_row - ONE][start_col - ASCI] = '_';
			board[endpos_row - ONE][endpos_col - ASCI] = '1';
		}else {
			board[start_row - ONE][start_col - ASCI] = '_';
			board[endpos_row - ONE][endpos_col - ASCI] = '0';
		}
		print(board);
		break;
	}
	return 1;
}

/**
 * @brief this function is used to check the user entered
 * input coordinates  are matching with the board
 *
 * @param col_value user entered column value
 * @param row_value user entered row value
 *
 * @returns 0 on failure and 1 on success
 */
int input_check(char col_value, int row_value)
{
	switch (col_value) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':;
		break;
	default: printf("enter within the range\n");
		return 0;
	}
	if (row_value < 1 || row_value > 8) {
		printf("enter within range\n");
		return 0;
	}

	return 1;
}


