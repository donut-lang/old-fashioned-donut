mus_lose_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $03

@chn0:
@chn0_0:
	.byte $43,$18,$86,$15,$82,$13,$92
@chn0_loop:
@chn0_1:
	.byte $9f
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_0:
	.byte $44,$18,$86,$15,$82,$13,$92
@chn1_loop:
@chn1_1:
	.byte $9f
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_0:
	.byte $45,$18,$82,$3f,$82,$41,$15,$82,$45,$14,$8a,$3f,$86
@chn2_loop:
@chn2_1:
	.byte $9f
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_0:
	.byte $49,$0b,$82,$46,$0f,$80,$0f,$80,$0f,$82,$49,$0b,$92
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
