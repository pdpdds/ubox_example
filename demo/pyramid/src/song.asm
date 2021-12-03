; Green, Song part, encoded in the AKM (minimalist) format V0.


Green_Start
Green_StartDisarkGenerateExternalLabel

Green_DisarkPointerRegionStart0
	dw Green_InstrumentIndexes	; Index table for the Instruments.
Green_DisarkForceNonReferenceDuring2_1
	dw 0	; Index table for the Arpeggios.
Green_DisarkForceNonReferenceDuring2_2
	dw 0	; Index table for the Pitches.

; The subsongs references.
	dw Green_Subsong0
	dw Green_Subsong1
	dw Green_Subsong2
Green_DisarkPointerRegionEnd0

; The Instrument indexes.
Green_InstrumentIndexes
Green_DisarkPointerRegionStart3
	dw Green_Instrument0
	dw Green_Instrument1
	dw Green_Instrument2
	dw Green_Instrument3
	dw Green_Instrument4
Green_DisarkPointerRegionEnd3

; The Instrument.
Green_DisarkByteRegionStart4
Green_Instrument0
	db 255	; Speed.

Green_Instrument0Loop	db 0	; Volume: 0.

	db 4	; End the instrument.
Green_DisarkPointerRegionStart5
	dw Green_Instrument0Loop	; Loops.
Green_DisarkPointerRegionEnd5

Green_Instrument1
	db 0	; Speed.

	db 173	; Volume: 11.
	db 232	; Arpeggio: -12.

	db 45	; Volume: 11.

	db 45	; Volume: 11.

	db 45	; Volume: 11.

	db 45	; Volume: 11.

	db 4	; End the instrument.
Green_DisarkPointerRegionStart6
	dw Green_Instrument0Loop	; Loop to silence.
Green_DisarkPointerRegionEnd6

Green_Instrument2
	db 0	; Speed.

	db 173	; Volume: 11.
	db 232	; Arpeggio: -12.

	db 45	; Volume: 11.

	db 173	; Volume: 11.
	db 232	; Arpeggio: -12.

	db 45	; Volume: 11.

	db 4	; End the instrument.
Green_DisarkPointerRegionStart7
	dw Green_Instrument0Loop	; Loop to silence.
Green_DisarkPointerRegionEnd7

Green_Instrument3
	db 0	; Speed.

	db 177	; Volume: 12.
	db 27	; Arpeggio: 13.
	db 9	; Noise: 9.

	db 173	; Volume: 11.
	db 18	; Arpeggio: 9.

	db 169	; Volume: 10.
	db 12	; Arpeggio: 6.

	db 165	; Volume: 9.
	db 9	; Arpeggio: 4.
	db 5	; Noise: 5.

	db 4	; End the instrument.
Green_DisarkPointerRegionStart8
	dw Green_Instrument0Loop	; Loop to silence.
Green_DisarkPointerRegionEnd8

Green_Instrument4
	db 0	; Speed.

	db 113	; Volume: 12.
	dw 2	; Pitch: 2.

	db 45	; Volume: 11.

	db 41	; Volume: 10.

	db 37	; Volume: 9.

	db 33	; Volume: 8.

	db 29	; Volume: 7.

	db 25	; Volume: 6.

	db 21	; Volume: 5.

	db 4	; End the instrument.
Green_DisarkPointerRegionStart9
	dw Green_Instrument0Loop	; Loop to silence.
Green_DisarkPointerRegionEnd9

Green_DisarkByteRegionEnd4
Green_ArpeggioIndexes
Green_DisarkPointerRegionStart10
Green_DisarkPointerRegionEnd10

Green_DisarkByteRegionStart11
Green_DisarkByteRegionEnd11

Green_PitchIndexes
Green_DisarkPointerRegionStart12
Green_DisarkPointerRegionEnd12

Green_DisarkByteRegionStart13
Green_DisarkByteRegionEnd13

; Green, Subsong 0.
; ----------------------------------

Green_Subsong0
Green_Subsong0DisarkPointerRegionStart0
	dw Green_Subsong0_NoteIndexes	; Index table for the notes.
	dw Green_Subsong0_TrackIndexes	; Index table for the Tracks.
Green_Subsong0DisarkPointerRegionEnd0

Green_Subsong0DisarkByteRegionStart1
	db 6	; Initial speed.

	db 2	; Most used instrument.
	db 4	; Second most used instrument.

	db 1	; Most used wait.
	db 3	; Second most used wait.

	db 0	; Default start note in tracks.
	db 0	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 13	; Are there effects? 12 if yes, 13 if not. Don't ask.
Green_Subsong0DisarkByteRegionEnd1

; The Linker.
Green_Subsong0DisarkByteRegionStart2
; Pattern 0
Green_Subsong0_Loop
	db 170	; State byte.
	db 63	; New height.
	db 128	; New track (0) for channel 1, as a reference (index 0).
	db 128	; New track (0) for channel 2, as a reference (index 0).
	db 128	; New track (0) for channel 3, as a reference (index 0).

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Green_Subsong0DisarkByteRegionEnd2
Green_Subsong0DisarkPointerRegionStart3
	dw Green_Subsong0_Loop

Green_Subsong0DisarkPointerRegionEnd3
; The indexes of the tracks.
Green_Subsong0_TrackIndexes
Green_Subsong0DisarkPointerRegionStart4
	dw Green_Subsong0_Track0	; Track 0, index 0.
Green_Subsong0DisarkPointerRegionEnd4

Green_Subsong0DisarkByteRegionStart5
Green_Subsong0_Track0
	db 205	; New wait (127).
	db 127	;   Escape wait value.

Green_Subsong0DisarkByteRegionEnd5
; The note indexes.
Green_Subsong0_NoteIndexes
Green_Subsong0DisarkByteRegionStart6
Green_Subsong0DisarkByteRegionEnd6

; Green, Subsong 1.
; ----------------------------------

Green_Subsong1
Green_Subsong1DisarkPointerRegionStart0
	dw Green_Subsong1_NoteIndexes	; Index table for the notes.
	dw Green_Subsong1_TrackIndexes	; Index table for the Tracks.
Green_Subsong1DisarkPointerRegionEnd0

Green_Subsong1DisarkByteRegionStart1
	db 6	; Initial speed.

	db 2	; Most used instrument.
	db 4	; Second most used instrument.

	db 1	; Most used wait.
	db 3	; Second most used wait.

	db 57	; Default start note in tracks.
	db 0	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 13	; Are there effects? 12 if yes, 13 if not. Don't ask.
Green_Subsong1DisarkByteRegionEnd1

; The Linker.
Green_Subsong1DisarkByteRegionStart2
; Pattern 0
Green_Subsong1_Loop
	db 170	; State byte.
	db 63	; New height.
	db ((Green_Subsong1_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track0 - ($ + 1)) & 255)
	db ((Green_Subsong1_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track1 - ($ + 1)) & 255)
	db 128	; New track (2) for channel 3, as a reference (index 0).

; Pattern 1
	db 32	; State byte.
	db ((Green_Subsong1_Track3 - ($ + 2)) & #ff00) / 256	; New track (3) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track3 - ($ + 1)) & 255)

; Pattern 2
	db 40	; State byte.
	db ((Green_Subsong1_Track4 - ($ + 2)) & #ff00) / 256	; New track (4) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track4 - ($ + 1)) & 255)
	db ((Green_Subsong1_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track1 - ($ + 1)) & 255)

; Pattern 3
	db 40	; State byte.
	db ((Green_Subsong1_Track5 - ($ + 2)) & #ff00) / 256	; New track (5) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track5 - ($ + 1)) & 255)
	db ((Green_Subsong1_Track3 - ($ + 2)) & #ff00) / 256	; New track (3) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong1_Track3 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Green_Subsong1DisarkByteRegionEnd2
Green_Subsong1DisarkPointerRegionStart3
	dw Green_Subsong1_Loop

Green_Subsong1DisarkPointerRegionEnd3
; The indexes of the tracks.
Green_Subsong1_TrackIndexes
Green_Subsong1DisarkPointerRegionStart4
	dw Green_Subsong1_Track2	; Track 2, index 0.
Green_Subsong1DisarkPointerRegionEnd4

Green_Subsong1DisarkByteRegionStart5
Green_Subsong1_Track0
	db 222	; Primary instrument (2). New escaped note: 69. New wait (11).
	db 69	;   Escape note value.
	db 11	;   Escape wait value.
	db 95	; Primary instrument (2). Same escaped note: 69. Primary wait (1).
	db 94	; Primary instrument (2). New escaped note: 60. Primary wait (1).
	db 60	;   Escape note value.
	db 30	; Primary instrument (2). New escaped note: 62. 
	db 62	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 69. Primary wait (1).
	db 69	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 62. Primary wait (1).
	db 62	;   Escape note value.
	db 222	; Primary instrument (2). New escaped note: 67. New wait (15).
	db 67	;   Escape note value.
	db 15	;   Escape wait value.
	db 95	; Primary instrument (2). Same escaped note: 67. Primary wait (1).
	db 95	; Primary instrument (2). Same escaped note: 67. Primary wait (1).
	db 95	; Primary instrument (2). Same escaped note: 67. Primary wait (1).
	db 94	; Primary instrument (2). New escaped note: 65. Primary wait (1).
	db 65	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 62. Secondary wait (3).
	db 62	;   Escape note value.
	db 222	; Primary instrument (2). New escaped note: 60. New wait (127).
	db 60	;   Escape note value.
	db 127	;   Escape wait value.

Green_Subsong1_Track1
	db 94	; Primary instrument (2). New escaped note: 38. Primary wait (1).
	db 38	;   Escape note value.
	db 190	; New instrument (1). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (1). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 38. Primary wait (1).
	db 38	;   Escape note value.
	db 142	; New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (1). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 43. Primary wait (1).
	db 43	;   Escape note value.
	db 142	; New escaped note: 31. Secondary wait (3).
	db 31	;   Escape note value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (1). New escaped note: 31. Secondary wait (3).
	db 31	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 43. Primary wait (1).
	db 43	;   Escape note value.
	db 142	; New escaped note: 31. Secondary wait (3).
	db 31	;   Escape note value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 126	; New instrument (1). New escaped note: 31. Primary wait (1).
	db 31	;   Escape note value.
	db 1	;   Escape instrument value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 254	; New instrument (1). New escaped note: 27. New wait (127).
	db 27	;   Escape note value.
	db 1	;   Escape instrument value.
	db 127	;   Escape wait value.

Green_Subsong1_Track2
	db 205	; New wait (127).
	db 127	;   Escape wait value.

Green_Subsong1_Track3
	db 94	; Primary instrument (2). New escaped note: 38. Primary wait (1).
	db 38	;   Escape note value.
	db 190	; New instrument (1). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (1). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 38. Primary wait (1).
	db 38	;   Escape note value.
	db 142	; New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (1). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 24. Primary wait (1).
	db 24	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 43. Primary wait (1).
	db 43	;   Escape note value.
	db 142	; New escaped note: 31. Secondary wait (3).
	db 31	;   Escape note value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (1). New escaped note: 31. Secondary wait (3).
	db 31	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 43. Primary wait (1).
	db 43	;   Escape note value.
	db 142	; New escaped note: 31. Secondary wait (3).
	db 31	;   Escape note value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 126	; New instrument (3). New escaped note: 36. Primary wait (1).
	db 36	;   Escape note value.
	db 3	;   Escape instrument value.
	db 143	; Same escaped note: 36. Secondary wait (3).
	db 207	; Same escaped note: 36. New wait (127).
	db 127	;   Escape wait value.

Green_Subsong1_Track4
	db 111	; Secondary instrument (4). Same escaped note: 57. Primary wait (1).
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 48. Primary wait (1).
	db 48	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 57. Primary wait (1).
	db 57	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 48. Primary wait (1).
	db 48	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 69. Primary wait (1).
	db 69	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 70. Primary wait (1).
	db 70	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 69. Primary wait (1).
	db 69	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 67. Primary wait (1).
	db 67	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 69. Secondary wait (3).
	db 69	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 65. Secondary wait (3).
	db 65	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 53. Primary wait (1).
	db 53	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 57. Primary wait (1).
	db 57	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 53. Primary wait (1).
	db 53	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 48. Primary wait (1).
	db 48	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 62. Secondary wait (3).
	db 62	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 69. Secondary wait (3).
	db 69	;   Escape note value.
	db 95	; Primary instrument (2). Same escaped note: 69. Primary wait (1).
	db 94	; Primary instrument (2). New escaped note: 67. Primary wait (1).
	db 67	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 65. Primary wait (1).
	db 65	;   Escape note value.
	db 222	; Primary instrument (2). New escaped note: 64. New wait (127).
	db 64	;   Escape note value.
	db 127	;   Escape wait value.

Green_Subsong1_Track5
	db 111	; Secondary instrument (4). Same escaped note: 57. Primary wait (1).
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 48. Primary wait (1).
	db 48	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 57. Primary wait (1).
	db 57	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 48. Primary wait (1).
	db 48	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 69. Primary wait (1).
	db 69	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 70. Primary wait (1).
	db 70	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 69. Primary wait (1).
	db 69	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 67. Primary wait (1).
	db 67	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 69. Secondary wait (3).
	db 69	;   Escape note value.
	db 158	; Primary instrument (2). New escaped note: 65. Secondary wait (3).
	db 65	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 53. Primary wait (1).
	db 53	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 57. Primary wait (1).
	db 57	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 53. Primary wait (1).
	db 53	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 50. Primary wait (1).
	db 50	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 48. Primary wait (1).
	db 48	;   Escape note value.
	db 222	; Primary instrument (2). New escaped note: 62. New wait (5).
	db 62	;   Escape note value.
	db 5	;   Escape wait value.
	db 94	; Primary instrument (2). New escaped note: 65. Primary wait (1).
	db 65	;   Escape note value.
	db 94	; Primary instrument (2). New escaped note: 67. Primary wait (1).
	db 67	;   Escape note value.
	db 159	; Primary instrument (2). Same escaped note: 67. Secondary wait (3).
	db 222	; Primary instrument (2). New escaped note: 65. New wait (127).
	db 65	;   Escape note value.
	db 127	;   Escape wait value.

Green_Subsong1DisarkByteRegionEnd5
; The note indexes.
Green_Subsong1_NoteIndexes
Green_Subsong1DisarkByteRegionStart6
Green_Subsong1DisarkByteRegionEnd6

; Green, Subsong 2.
; ----------------------------------

Green_Subsong2
Green_Subsong2DisarkPointerRegionStart0
	dw Green_Subsong2_NoteIndexes	; Index table for the notes.
	dw Green_Subsong2_TrackIndexes	; Index table for the Tracks.
Green_Subsong2DisarkPointerRegionEnd0

Green_Subsong2DisarkByteRegionStart1
	db 6	; Initial speed.

	db 2	; Most used instrument.
	db 4	; Second most used instrument.

	db 1	; Most used wait.
	db 3	; Second most used wait.

	db 31	; Default start note in tracks.
	db 3	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 13	; Are there effects? 12 if yes, 13 if not. Don't ask.
Green_Subsong2DisarkByteRegionEnd1

; The Linker.
Green_Subsong2DisarkByteRegionStart2
; Pattern 0
	db 170	; State byte.
	db 63	; New height.
	db ((Green_Subsong2_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong2_Track0 - ($ + 1)) & 255)
	db ((Green_Subsong2_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Green_Subsong2_Track1 - ($ + 1)) & 255)
	db 128	; New track (2) for channel 3, as a reference (index 0).

; Pattern 1
Green_Subsong2_Loop
	db 40	; State byte.
	db 128	; New track (2) for channel 1, as a reference (index 0).
	db 128	; New track (2) for channel 2, as a reference (index 0).

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Green_Subsong2DisarkByteRegionEnd2
Green_Subsong2DisarkPointerRegionStart3
	dw Green_Subsong2_Loop

Green_Subsong2DisarkPointerRegionEnd3
; The indexes of the tracks.
Green_Subsong2_TrackIndexes
Green_Subsong2DisarkPointerRegionStart4
	dw Green_Subsong2_Track2	; Track 2, index 0.
Green_Subsong2DisarkPointerRegionEnd4

Green_Subsong2DisarkByteRegionStart5
Green_Subsong2_Track0
	db 110	; Secondary instrument (4). New escaped note: 55. Primary wait (1).
	db 55	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 54. Primary wait (1).
	db 54	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 53. Primary wait (1).
	db 53	;   Escape note value.
	db 46	; Secondary instrument (4). New escaped note: 51. 
	db 51	;   Escape note value.
	db 46	; Secondary instrument (4). New escaped note: 49. 
	db 49	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 50. Secondary wait (3).
	db 50	;   Escape note value.
	db 175	; Secondary instrument (4). Same escaped note: 50. Secondary wait (3).
	db 238	; Secondary instrument (4). New escaped note: 55. New wait (7).
	db 55	;   Escape note value.
	db 7	;   Escape wait value.
	db 222	; Primary instrument (2). New escaped note: 43. New wait (127).
	db 43	;   Escape note value.
	db 127	;   Escape wait value.

Green_Subsong2_Track1
	db 79	; Same escaped note: 31. Primary wait (1).
	db 126	; New instrument (1). New escaped note: 30. Primary wait (1).
	db 30	;   Escape note value.
	db 1	;   Escape instrument value.
	db 78	; New escaped note: 29. Primary wait (1).
	db 29	;   Escape note value.
	db 78	; New escaped note: 28. Primary wait (1).
	db 28	;   Escape note value.
	db 190	; New instrument (3). New escaped note: 26. Secondary wait (3).
	db 26	;   Escape note value.
	db 3	;   Escape instrument value.
	db 191	; New instrument (1). Same escaped note: 26. Secondary wait (3).
	db 1	;   Escape instrument value.
	db 254	; New instrument (3). New escaped note: 43. New wait (7).
	db 43	;   Escape note value.
	db 3	;   Escape instrument value.
	db 7	;   Escape wait value.
	db 254	; New instrument (1). New escaped note: 31. New wait (127).
	db 31	;   Escape note value.
	db 1	;   Escape instrument value.
	db 127	;   Escape wait value.

Green_Subsong2_Track2
	db 205	; New wait (127).
	db 127	;   Escape wait value.

Green_Subsong2DisarkByteRegionEnd5
; The note indexes.
Green_Subsong2_NoteIndexes
Green_Subsong2DisarkByteRegionStart6
Green_Subsong2DisarkByteRegionEnd6

