	.export _music_level
	.export _music_game
	.export _music_clear
	.export _music_lose
	.export _music_gameover
	.export _music_welldone

	.include "mus_instruments.s"
_music_level: 		.include "mus_level.s"
_music_game: 		.include "mus_game.s"
_music_clear: 		.include "mus_clear.s"
_music_lose: 		.include "mus_lose.s"
_music_gameover: 	.include "mus_gameover.s"
_music_welldone: 	.include "mus_welldone.s"