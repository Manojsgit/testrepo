#include <stdio.h>
#include <stdbool.h>
#define BLACK 1
#define WHITE 0
void print(int [][8]);
int  is_valid_row_column(char, int);
void initial_setup_set(int [][8]);
int check_stalemate(int[][8], int*, int*, int*, int*, int);
int  legal_position_check(int [][8], int, int, char);
int  calculate_possible_moves(int [][8], int, int, char, int*, int*);
int process_end_move(int[][8], int *, int *, int, char, int, char, int);
int main()
{

	int board[8][8], current_user = 0, result, end_result;
	char choice, start_col, endpos_col;
	int start_row, endpos_row, position_valid, input_valid = 0, stalemate_result = 0;
	int black_row[] = { 1, 1, 1, 2 };       //possible combination of rows for black
	int black_col[] = { -1, 1, 0, 0 };      //possible combination of columns black
	int white_row[] = { -1, -1, -1, -2 };   //possible combination of rows white
	int white_col[] = { -1, 1, 0, 0 };      //possible combination of columns white

	printf("CHESS GAME begins\n");
	initial_setup_set(board);
	print(board);
	(current_user) ? (printf("black user\n")) : (printf("white user\n"));
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
			input_valid = is_valid_row_column(start_col, start_row);
			if (input_valid == false)
				continue;

			position_valid = legal_position_check(board, current_user, start_row, start_col);

			if (position_valid == false)
				continue;

			if (current_user == WHITE)  // if current user is WHITE means white user and BLACK means black user
				result = calculate_possible_moves(board, current_user, start_row, start_col, white_row, white_col);
			else
				result = calculate_possible_moves(board, current_user, start_row, start_col, black_row, black_col);
			if (result == false) { //false means no possible moves
				printf("please enter a position for different pawn\n");
				continue;
			}else {

				while (1) {
					printf("\nENTER THE END MoVE\n");
					printf("enter q/Q to deselect the pawn\n");
					scanf(" %c", &endpos_col);
					endpos_col = endpos_col & '_'; //converting to uppercase
					if (endpos_col == 'Q') {
						printf("now you can select the different pawn\n");
						break;
					}
					scanf(" %d", &endpos_row);
					input_valid = is_valid_row_column(endpos_col, endpos_row);
					if (input_valid == false)
						continue;
					if (current_user == WHITE)
						end_result = process_end_move(board, white_row, white_col, start_row, start_col, endpos_row, endpos_col,current_user);
					else
						end_result = process_end_move(board, black_row, black_col, start_row, start_col, endpos_row, endpos_col,current_user);

					if (end_result == false)
						continue;
					break;
				}

			}
			if ( endpos_col == 'Q')
				continue;
			stalemate_result = check_stalemate(board, white_row, white_col, black_row, black_col, current_user);
			break;
		}
		if (stalemate_result == true)
			break;
		current_user = current_user + 1 % 2;
		(current_user) ? (printf("black user\n")) : (printf("white user\n"));
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
int legal_position_check(int board[][8], int user, int row_pos, char col_pos)
{
	int ret;

	if (user == 0) {
		//printf("white user\n");
		((board[row_pos - 1][col_pos - 'A']) == '0') ? ( ret = 1) : (ret = 0);
		return ret;
	}else {
		//printf("black user\n");
		((board[row_pos - 1][col_pos - 'A']) == '1') ? ( ret = 1) : (ret = 0);
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
int  calculate_possible_moves(int board[][8], int user, int row, char col, int *p_row, int *p_col)
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
		result[i] = 0;
	printf("\nPOSSIBLE MOVES\n");
	for (i = 0; i < 2; i++) {
		if (board[row + p_row[i] - 1][col - 'A' + p_col[i]] == opp_pawn) {

			result[i] = 1;

			printf(" %c%d ", col + p_col[i], row + p_row[i]);
		}
	}
	if (board[row + p_row[2] - 1][col - 'A' + p_col[2]] != opp_pawn && board[row + p_row[2] - 1][col - 'A' + p_col[2]] != our_pawn && row != 1 && row != 8) {

		result[2] = 1;

		printf("%c%d ", col + p_col[2], row + p_row[2]);
	}
	if (((row == 2 && our_pawn == '1' ) || (row == 7 && our_pawn == '0')) && result[2] == 1 && board[row + p_row[3] - 1][col - 'A' + p_col[3]] != opp_pawn) {

		result[3] = 1;

		printf("%c%d \n", col + p_col[3], row + p_row[3]);
	}

	for (i = 0; i < 4; i++)           //if there is a possible move the value will be 1 or i twill be 0
		if (result[i] == false)
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
 * @param endpos_row is user entered end row position
 * @param endpos_col is user entered end col position
 * @param user is specifying either black or white
 *
 * @return 0 on failure and 1 on success
 */
int process_end_move(int board[][8], int *moves_row, int *moves_col, int start_row, char start_col, int endpos_row, char endpos_col, int user)
{
	int temp_row, temp_col, iterator, number = 1;

	temp_row =  endpos_row - start_row;
	temp_col =  endpos_col - start_col;
	for (iterator = 0; iterator < 4; iterator++) {
		if (moves_row[iterator] == temp_row && moves_col[iterator] == temp_col ) {
			number = 0;
			break;
		}
	}
	if (number) {
		printf("enter only possible moves\n");
		return 0;
	}

	if (user) {
		board[start_row - 1][start_col - 'A'] = '_';
		board[endpos_row - 1][endpos_col - 'A'] = '1';
	}else {
		board[start_row - 1][start_col - 'A'] = '_';
		board[endpos_row - 1][endpos_col - 'A'] = '0';
	}
	print(board);
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
int is_valid_row_column(char col_value, int row_value)
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

/**
 * @brief this function is used to check
 * the stalemate condition
 *
 * @param user is use to identify either white or black user
 *
 * @return 0 on failure and 1 on success
 */
int check_stalemate(int board[][8], int* white_row, int* white_col, int* black_row, int* black_col, int user)
{
	char opp_pawn;
	int i, j, not_movablepawn = 0, *row_comb, *col_comb, result, no_of_pawn_found = 0;

	if (user == 0) {
		opp_pawn = '1';
		row_comb = black_row;
		col_comb = black_col;
	}else {
		opp_pawn = '0';
		row_comb = white_row;
		col_comb = white_col;
	}
	user ? (user = 0) : (user = 1);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j] == opp_pawn) {
				no_of_pawn_found = no_of_pawn_found + 1;
				result = calculate_possible_moves(board, user, i + 1, j + 'A', row_comb, col_comb);
				if (result == false)
					not_movablepawn = not_movablepawn + 1;
			}
		}
	}
	if (no_of_pawn_found == not_movablepawn) {
		printf("STALEMATE DRAW\n");
		return 1;
	}else
		return 0;
}





