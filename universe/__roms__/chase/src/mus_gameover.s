mus_gameover_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $06

@chn0:
@chn0_0:
	.byte $43,$11,$11,$81,$10,$10,$81,$0f,$0f,$81,$0e,$80,$0c,$84,$18,$8a
@chn0_loop:
@chn0_1:
	.byte $9f
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_0:
	.byte $44,$11,$11,$81,$10,$10,$81,$0f,$0f,$81,$0e,$80,$0c,$84,$18,$82
	.byte $18,$86
@chn1_loop:
@chn1_1:
	.byte $9f
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_0:
	.byte $41,$11,$80,$3f,$80,$10,$80,$3f,$80,$0f,$80,$3f,$80,$0e,$80,$0c
	.byte $82,$3f,$80,$45,$18,$84,$3f,$84
@chn2_loop:
@chn2_1:
	.byte $9f
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_0:
	.byte $42,$0f,$80,$46,$0f,$80,$42,$0e,$80,$46,$0f,$80,$42,$0d,$80,$46
	.byte $0f,$80,$42,$0d,$80,$0c,$84,$49,$0b,$80,$42,$0f,$88
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
