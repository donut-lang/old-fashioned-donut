mus_game_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $04

@chn0:
@chn0_loop:
@chn0_0:
	.byte $bf
@chn0_1:
	.byte $bf
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_loop:
@chn1_0:
	.byte $bf
@chn1_1:
	.byte $bf
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_loop:
@chn2_0:
	.byte $41,$13,$3f,$81,$13,$3f,$12,$80,$49,$18,$3f,$85,$41,$11,$3f,$81
	.byte $11,$80,$10,$80,$49,$18,$3f,$85,$41,$13,$3f,$81,$42,$13,$3f,$12
	.byte $80,$49,$18,$3f,$81,$41,$12,$3f,$81,$11,$3f,$81,$11,$80,$49,$18
	.byte $41,$10,$81,$3f,$84
@chn2_1:
	.byte $13,$3f,$81,$13,$3f,$12,$80,$49,$18,$3f,$81,$41,$12,$3f,$81,$11
	.byte $3f,$81,$11,$80,$10,$80,$49,$18,$3f,$85,$41,$0e,$3f,$81,$0e,$3f
	.byte $10,$80,$49,$18,$3f,$81,$41,$11,$80,$49,$18,$41,$13,$83,$3f,$8a
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_loop:
@chn3_0:
	.byte $46,$0f,$80,$0f,$80,$42,$0f,$80,$46,$0f,$80,$49,$0b,$82,$42,$0f
	.byte $80,$46,$0f,$82,$0f,$80,$42,$0f,$80,$46,$0f,$80,$49,$0b,$82,$46
	.byte $0f,$82,$0f,$80,$0f,$80,$42,$0f,$80,$46,$0f,$80,$49,$0b,$82,$42
	.byte $0f,$80,$46,$0f,$82,$0f,$80,$42,$0f,$80,$49,$0b,$84,$46,$0f,$80
	.byte $0f,$80
@chn3_1:
	.byte $0f,$80,$0f,$80,$42,$0f,$80,$46,$0f,$80,$49,$0b,$82,$42,$0f,$80
	.byte $46,$0f,$82,$0f,$80,$42,$0f,$80,$46,$0f,$80,$49,$0b,$82,$46,$0f
	.byte $82,$0f,$80,$0f,$80,$42,$0f,$80,$46,$0f,$80,$49,$0b,$82,$0b,$80
	.byte $0b,$84,$46,$0f,$80,$0f,$80,$42,$0f,$82,$46,$0f,$80,$0f,$80
	.byte $fe
	.word @chn3_loop

@chn4:
@chn4_loop:
@chn4_0:
	.byte $bf
@chn4_1:
	.byte $bf
	.byte $fe
	.word @chn4_loop