
@{{BLOCK(ball_status_bar)

@=======================================================================
@
@	ball_status_bar, 128x8@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap lz77 compressed
@	Total size: 32 + 124 = 156
@
@	Time-stamp: 2025-08-22, 19:01:18
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global ball_status_barBitmap		@ 124 unsigned chars
	.hidden ball_status_barBitmap
ball_status_barBitmap:
	.word 0x00020010,0x0000000F,0xF0030076,0xF001F001,0x0C209E01,0x13F07776,0x01F001F0,0xC0773E60
	.word 0x0B3001F0,0x78787878,0x88008888,0x86868688,0x00666686,0x65656666,0x55556565,0xF0555538
	.word 0x1001F03E,0x8787870E,0x3F108742,0x68686868,0x1F563F10,0xD0565656,0xF001F03F,0xF001F001
	.word 0x01F0FF01,0x01F001F0,0x01F001F0,0x01F001F0,0xF0E001F0,0x1001F001,0x00000001

	.section .rodata
	.align	2
	.global ball_status_barPal		@ 32 unsigned chars
	.hidden ball_status_barPal
ball_status_barPal:
	.hword 0x0000,0x2108,0x6FFF,0x5B5B,0x3AF8,0x424F,0x31AA,0x00E4
	.hword 0x2147,0x039F,0x4A7F,0x7F28,0x7C00,0x03E0,0x001F,0x7E4D

@}}BLOCK(ball_status_bar)
