;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (MINGW64)
;--------------------------------------------------------
	.module spman
	.optsdcc -mz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _ubox_set_sprite_pat16_flip
	.globl _ubox_set_sprite_pat16
	.globl _ubox_write_vm
	.globl _sp_last_pat
	.globl _sp_pat_map
	.globl _sp_buffer
	.globl _sp_fixed
	.globl _sp_idx
	.globl _sp_last_fixed_sprite
	.globl _sp_last_sprite
	.globl _spman_init
	.globl _spman_alloc_pat
	.globl _spman_sprite_flush
	.globl _spman_alloc_fixed_sprite
	.globl _spman_alloc_sprite
	.globl _spman_update
	.globl _spman_hide_all_sprites
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
_sp_last_sprite::
	.ds 1
_sp_last_fixed_sprite::
	.ds 1
_sp_idx::
	.ds 1
_sp_fixed::
	.ds 128
_sp_buffer::
	.ds 256
_sp_pat_map::
	.ds 64
_sp_last_pat::
	.ds 1
_p:
	.ds 2
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;spman.c:17: void spman_init()
;	---------------------------------
; Function spman_init
; ---------------------------------
_spman_init::
;spman.c:19: sp_last_pat = 0;
	ld	iy, #_sp_last_pat
	ld	0 (iy), #0x00
;spman.c:20: memset(sp_pat_map, SPMAN_PAT_UNUSED, SPMAN_MAX_PATTERNS);
	ld	hl, #_sp_pat_map
	ld	b, #0x20
00103$:
	ld	(hl), #0xff
	inc	hl
	ld	(hl), #0xff
	inc	hl
	djnz	00103$
;spman.c:21: spman_sprite_flush();
;spman.c:22: }
	jp	_spman_sprite_flush
;spman.c:24: uint8_t spman_alloc_pat(uint8_t type, uint8_t *data, uint8_t len, uint8_t flip)
;	---------------------------------
; Function spman_alloc_pat
; ---------------------------------
_spman_alloc_pat::
	dec	sp
;spman.c:28: if (sp_pat_map[type] == SPMAN_PAT_UNUSED)
	ld	bc, #_sp_pat_map+0
	ld	a, c
	ld	hl, #3
	add	hl, sp
	add	a, (hl)
	ld	c, a
	jr	NC,00133$
	inc	b
00133$:
	ld	a, (bc)
	inc	a
	jr	NZ,00106$
;spman.c:30: sp_pat_map[type] = sp_last_pat;
	ld	a,(#_sp_last_pat + 0)
	ld	(bc), a
;spman.c:31: for (i = 0; i < len; ++i)
	ld	hl, #4
	add	hl, sp
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	xor	a, a
	ld	iy, #0
	add	iy, sp
	ld	0 (iy), a
00108$:
	ld	hl, #6
	add	hl, sp
	ld	iy, #0
	add	iy, sp
	ld	a, 0 (iy)
	sub	a, (hl)
	jr	NC,00106$
;spman.c:33: if (flip)
	ld	hl, #7+0
	add	hl, sp
	ld	a, (hl)
	or	a, a
	jr	Z,00102$
;spman.c:34: ubox_set_sprite_pat16_flip(data, sp_last_pat);
	push	bc
	push	de
	ld	a, (_sp_last_pat)
	push	af
	inc	sp
	push	de
	call	_ubox_set_sprite_pat16_flip
	pop	af
	inc	sp
	pop	de
	pop	bc
	jr	00103$
00102$:
;spman.c:36: ubox_set_sprite_pat16(data, sp_last_pat);
	push	bc
	push	de
	ld	a, (_sp_last_pat)
	push	af
	inc	sp
	push	de
	call	_ubox_set_sprite_pat16
	pop	af
	inc	sp
	pop	de
	pop	bc
00103$:
;spman.c:38: data += 32;
	ld	hl, #0x0020
	add	hl, de
	ex	de, hl
;spman.c:39: sp_last_pat++;
	ld	hl, #_sp_last_pat+0
	inc	(hl)
;spman.c:31: for (i = 0; i < len; ++i)
	ld	iy, #0
	add	iy, sp
	inc	0 (iy)
	jr	00108$
00106$:
;spman.c:47: return sp_pat_map[type] * 4;
	ld	a, (bc)
	add	a, a
	add	a, a
	ld	l, a
;spman.c:48: }
	inc	sp
	ret
;spman.c:50: void spman_sprite_flush()
;	---------------------------------
; Function spman_sprite_flush
; ---------------------------------
_spman_sprite_flush::
;spman.c:52: sp_last_fixed_sprite = 0;
	ld	hl,#_sp_last_fixed_sprite + 0
	ld	(hl), #0x00
;spman.c:53: sp_last_sprite = 0;
	ld	hl,#_sp_last_sprite + 0
	ld	(hl), #0x00
;spman.c:54: }
	ret
;spman.c:56: void spman_alloc_fixed_sprite(struct sprite_attr *sp)
;	---------------------------------
; Function spman_alloc_fixed_sprite
; ---------------------------------
_spman_alloc_fixed_sprite::
	push	ix
	ld	ix,#0
	add	ix,sp
;spman.c:65: memcpy(&sp_fixed[sp_last_fixed_sprite++], sp, 4);
	ld	bc, #_sp_fixed+0
	ld	iy, #_sp_last_fixed_sprite
	ld	e, 0 (iy)
	inc	0 (iy)
	ld	d, #0x00
	ld	l, e
	ld	h, d
	add	hl, hl
	add	hl, hl
	add	hl, bc
	ex	de, hl
	ld	l, 4 (ix)
	ld	h, 5 (ix)
	ld	bc, #0x0004
	ldir
;spman.c:66: }
	pop	ix
	ret
;spman.c:68: void spman_alloc_sprite(struct sprite_attr *sp)
;	---------------------------------
; Function spman_alloc_sprite
; ---------------------------------
_spman_alloc_sprite::
	push	ix
	ld	ix,#0
	add	ix,sp
;spman.c:77: memcpy(&sp_buffer[sp_last_sprite++], sp, 4);
	ld	bc, #_sp_buffer+0
	ld	iy, #_sp_last_sprite
	ld	e, 0 (iy)
	inc	0 (iy)
	ld	d, #0x00
	ld	l, e
	ld	h, d
	add	hl, hl
	add	hl, hl
	add	hl, bc
	ex	de, hl
	ld	l, 4 (ix)
	ld	h, 5 (ix)
	ld	bc, #0x0004
	ldir
;spman.c:78: }
	pop	ix
	ret
;spman.c:84: void spman_update()
;	---------------------------------
; Function spman_update
; ---------------------------------
_spman_update::
;spman.c:86: p = (uint8_t*) 0x1b00;
	ld	hl, #0x1b00
	ld	(_p), hl
;spman.c:88: if (sp_last_sprite)
	ld	iy, #_sp_last_sprite
	ld	a, 0 (iy)
	or	a, a
	jr	Z,00106$
;spman.c:90: memcpy(&sp_buffer[sp_last_sprite], sp_buffer, sp_last_sprite * 4);
	ld	bc, #_sp_buffer+0
	ld	l, 0 (iy)
	ld	h, #0x00
	add	hl, hl
	add	hl, hl
	ex	de,hl
	ld	l, c
	ld	h, b
	add	hl, de
	push	de
	push	bc
	push	hl
	call	___memcpy
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
;spman.c:92: if (sp_last_sprite > 2)
	ld	a, #0x02
	ld	iy, #_sp_last_sprite
	sub	a, 0 (iy)
	jr	NC,00102$
;spman.c:93: sp_idx += 2;
	ld	a,(#_sp_idx + 0)
	add	a, #0x02
	ld	(_sp_idx+0), a
00102$:
;spman.c:95: if (sp_idx >= sp_last_sprite)
	ld	hl, #_sp_last_sprite
	ld	iy, #_sp_idx
	ld	a, 0 (iy)
	sub	a, (hl)
	jr	C,00107$
;spman.c:96: sp_idx -= sp_last_sprite;
	ld	hl, #_sp_last_sprite
	push	de
	push	iy
	pop	de
	ld	a, (de)
	sub	a, (hl)
	ld	(de), a
	pop	de
	jr	00107$
00106$:
;spman.c:99: sp_idx = 0;
	ld	hl,#_sp_idx + 0
	ld	(hl), #0x00
00107$:
;spman.c:101: memcpy(&sp_buffer[sp_idx + sp_last_sprite], &hide, 4);
	ld	bc, #_sp_buffer+0
	ld	hl,#_sp_idx + 0
	ld	e, (hl)
	ld	d, #0x00
	ld	iy, #_sp_last_sprite
	ld	l, 0 (iy)
	ld	h, #0x00
	add	hl, de
	add	hl, hl
	add	hl, hl
	add	hl, bc
	ex	de, hl
	push	bc
	ld	hl, #_hide
	ld	bc, #0x0004
	ldir
	pop	bc
;spman.c:103: ubox_wait_vsync();
;	
	halt 
;spman.c:104: if (sp_last_fixed_sprite)
	ld	iy, #_sp_last_fixed_sprite
	ld	a, 0 (iy)
	or	a, a
	jr	Z,00112$
;spman.c:106: ubox_write_vm(p, sp_last_fixed_sprite * 4, (uint8_t *)sp_fixed);
	ld	l, 0 (iy)
	ld	h, #0x00
	add	hl, hl
	add	hl, hl
	push	bc
	ld	de, #_sp_fixed
	push	de
	push	hl
	ld	hl, (_p)
	push	hl
	call	_ubox_write_vm
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
	pop	bc
;spman.c:107: p += sp_last_fixed_sprite * 4;
	ld	iy, #_sp_last_fixed_sprite
	ld	l, 0 (iy)
	ld	h, #0x00
	add	hl, hl
	add	hl, hl
	ex	de,hl
	ld	hl, #_p
	ld	a, (hl)
	add	a, e
	ld	(hl), a
	inc	hl
	ld	a, (hl)
	adc	a, d
	ld	(hl), a
00112$:
;spman.c:109: ubox_write_vm(p, 4 + sp_last_sprite * 4, (uint8_t *)&sp_buffer[sp_idx]);
	ld	iy, #_sp_idx
	ld	l, 0 (iy)
	ld	h, #0x00
	add	hl, hl
	add	hl, hl
	add	hl, bc
	ex	de, hl
	ld	iy, #_sp_last_sprite
	ld	l, 0 (iy)
	ld	h, #0x00
	add	hl, hl
	add	hl, hl
	ld	bc, #0x0004
	add	hl, bc
	push	de
	push	hl
	ld	hl, (_p)
	push	hl
	call	_ubox_write_vm
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
;spman.c:111: spman_sprite_flush();
;spman.c:112: }
	jp	_spman_sprite_flush
_hide:
	.db #0xd0	; 208
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
;spman.c:114: void spman_hide_all_sprites()
;	---------------------------------
; Function spman_hide_all_sprites
; ---------------------------------
_spman_hide_all_sprites::
;spman.c:116: ubox_wait_vsync();
;	
	halt 
;spman.c:117: ubox_write_vm((uint8_t *)0x1b00, 4, (uint8_t *)hide);
	ld	bc, #_hide+0
	push	bc
	ld	hl, #0x0004
	push	hl
	ld	hl, #0x1b00
	push	hl
	call	_ubox_write_vm
	ld	hl, #6
	add	hl, sp
	ld	sp, hl
;spman.c:118: }
	ret
	.area _CODE
	.area _INITIALIZER
	.area _CABS (ABS)
