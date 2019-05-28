
JB_CPU_Usage_Basic.ino.elf:     file format elf32-avr

Disassembly of section .data:

00800100 <__data_start>:
  
  --> unsigned long 3000000 = 0x002dc6c0

  800100:	c0 c6       	rjmp	.+3456   	; 0x800e82 <__bss_end+0xca1>
  800102:	2d 00       	.word	0x002d	; ????


  800104:	00 40       	sbci	r16, 0x00	; 0
  800106:	14 54       	subi	r17, 0x44	; 68
  800108:	00 40       	sbci	r16, 0x00	; 0
  80010a:	10 50       	subi	r17, 0x00	; 0


void loop() 
{
  if (0 == _loopCounter--) {

  --> load bytes from adress 0x800100, 0x800101, 0x800102, 0x800103 to register 24 (low), 25, 26, 27 (high)
      unsigned long = 4 bytes
      LDS = Load Direct from Data Space

 c26:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__data_start>
 c2a:	90 91 01 01 	lds	r25, 0x0101	; 0x800101 <__data_start+0x1>
 c2e:	a0 91 02 01 	lds	r26, 0x0102	; 0x800102 <__data_start+0x2>
 c32:	b0 91 03 01 	lds	r27, 0x0103	; 0x800103 <__data_start+0x3>

  --> copy register 24, 25 to 20, 21

 c36:	ac 01       	movw	r20, r24

  --> copy register 26, 27 to 22, 23

 c38:	bd 01       	movw	r22, r26

  --> subtract one from low byte and handle carry over to high byte

 c3a:	41 50       	subi	r20, 0x01	; 1
 c3c:	51 09       	sbc	r21, r1
 c3e:	61 09       	sbc	r22, r1
 c40:	71 09       	sbc	r23, r1

  --> write registers back to memory location
      STS = Store Direct to Data Space

 c42:	40 93 00 01 	sts	0x0100, r20	; 0x800100 <__data_start>
 c46:	50 93 01 01 	sts	0x0101, r21	; 0x800101 <__data_start+0x1>
 c4a:	60 93 02 01 	sts	0x0102, r22	; 0x800102 <__data_start+0x2>
 c4e:	70 93 03 01 	sts	0x0103, r23	; 0x800103 <__data_start+0x3>

  --> "or" all register bytes, result is zero if all registers are zero
      result = r24 | r25 | r26 | r27  

 c52:	89 2b       	or	r24, r25
 c54:	8a 2b       	or	r24, r26
 c56:	8b 2b       	or	r24, r27

  --> ??

 c58:	09 f0       	breq	.+2      	; 0xc5c <main+0x104>

  --> ??

 c5a:	3f c0       	rjmp	.+126    	; 0xcda <main+0x182>
}

