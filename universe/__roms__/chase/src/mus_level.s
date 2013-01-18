mus_level_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $03

@chn0:
@chn0_0:
	.byte $47,$18,$80,$18,$80,$1a,$8a
@chn0_loop:
@chn0_1:
	.byte $8f
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_0:
	.byte $48,$18,$80,$18,$80,$1a,$8a
@chn1_loop:
@chn1_1:
	.byte $8f
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_0:
	.byte $41,$18,$80,$18,$80,$49,$18,$41,$1a,$81,$3f,$86
@chn2_loop:
@chn2_1:
	.byte $8f
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_0:
	.byte $46,$0f,$80,$0f,$80,$49,$0b,$80,$42,$0f,$88
@chn3_loop:
@chn3_1:
	.byte $8f
	.byte $fe
	.word @chn3_loop

@chn4:
@chn4_0:
	.byte $8f
@chn4_loop:
@chn4_1:
	.byte $8f
	.byte $fe
	.word @chn4_loop
