#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <conio.h>
#include "libs/miscs.utils.h"
#include "libs/display.utils.h"
#include "libs/game.utils.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <io.h>
#elif __APPLE__
#include <sys/uio.h>
#elif __linux__ || __unix__
#include <sys/io.h>
#else
#error "Unknown compiler"
#endif


/*H**********************************************************************
* FILENAME: app.c
*
* DESCRIPTION :
*       Main file of our app
*
* PUBLIC FUNCTIONS :
*       int main(...)
*
* NOTES :
*       ...command to compile
*       Copyright Groupe 19. 2022. All rights reserved.
*
* AUTHORS : Raphael BIRON, Kilian ALVES, Maram ZITOUNI, Lucas PIERRE, Corentin LECHENE
* START DATE : 06/10/2022
*
*H***********************************************************************/

/*=====================================================*/

int main(int argc, char *argv[]);
int _get_players_names(Board *board);

/*=====================================================*/

int main(int argc, char *argv[]){
	int game_run = 1;
    Config *config = calloc(1, sizeof(Config));
	Board *board = calloc(1, sizeof(Board));

    _setmode(_fileno(stdout), 0x00020000);

	config->rows = 6;
	config->columns = 7;
	config->align_to_win = 4;
	config->player_count = 4;

	if(config->rows < 2 || config->columns < 2 || config->align_to_win < 2 || config->player_count < 2){
		wprintf(L"%sInvalid configuration! (Every field must be at least equal to 2)%s", CONSOLE_COLORS[0], CONSOLE_COLORS[5]);
		return EXIT_FAILURE;
	}

	board = generate_board(config);

    if(_get_players_names(board)){
        display_board(board);
        while(game_run) game_run = run_game(board);
    } else {
		wprintf(L"%sAn error occured!", CONSOLE_COLORS[1]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int _get_players_names(Board *board){
	clear_console();
	wprintf(L"%s---=== PUISSANCE C ===---%s", CONSOLE_COLORS[0], CONSOLE_COLORS[5]);
	wchar_t *first = L"the first";
	wchar_t *next = L"next";
	for(short i = 0; i < board->player_count; i++){
		wchar_t *name = calloc(255, sizeof(wchar_t));

		short correct = 0;
		do {
			correct = 1;
			wprintf(L"\n\n\x2192 Enter %s player name: ", i == 0 ? first : next);
			wscanf(L"%s", name);
			int len = wcslen(name);
			if(len < 2){
				wprintf(L"\n%sIncorrect name! Retry...%s\n", CONSOLE_COLORS[0], CONSOLE_COLORS[5]);
				correct = 0;
			}
			for(short y = 0; y < board->player_count; y++){
				if(wcscmp(board->players[y]->name, name) == 0){
					wprintf(L"\n%sThis name is already taken! Retry...%s\n", CONSOLE_COLORS[0], CONSOLE_COLORS[5]);
					correct = 0;
					break;
				}
			}
		} while(!correct);

		wcscpy(board->players[i]->name, name);
		clear_console();
		fflush(stdin);
		free(name);
		wprintf(L"Welcome onboard %s%s %s!", board->players[i]->color, board->players[i]->name, CONSOLE_COLORS[5]);
	}
	clear_console();
	return 1;
}
