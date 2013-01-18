mus_clear_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $03

@chn0:
@chn0_0:
	.byte $43,$1d,$80,$1d,$80,$1d,$82,$1d,$80,$1d,$80,$1f,$92
@chn0_loop:
@chn0_1:
	.byte $9f
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_0:
	.byte $44,$1d,$80,$1d,$80,$1d,$82,$1d,$80,$1d,$80,$1f,$92
@chn1_loop:
@chn1_1:
	.byte $9f
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_0:
	.byte $41,$1d,$3f,$1d,$3f,$1d,$3f,$81,$1d,$3f,$1d,$3f,$45,$1f,$87,$3f
	.byte $89
@chn2_loop:
@chn2_1:
	.byte $9f
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_0:
	.byte $46,$0f,$80,$0f,$80,$42,$0f,$82,$46,$0f,$80,$0f,$80,$49,$0b,$80
	.byte $42,$0f,$90
@chn3_loop:
@chn3_1:
	.byte $9f
	.byte $fe
	.word @chn3_loop

@chn4:
@chn4_0:
	.byte $9f
@chn4_loop:
@chn4_1:
	.byte $9f
	.byte $fe
	.word @chn4_loop
