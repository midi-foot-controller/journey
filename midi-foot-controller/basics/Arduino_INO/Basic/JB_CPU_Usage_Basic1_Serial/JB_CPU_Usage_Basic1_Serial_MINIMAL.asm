
JB_CPU_Usage_Basic1_Serial.ino.elf:     file format elf32-avr

Contents of section .data:
 800100 c0c62d00 00000000 f6005f00 8d004301  ..-......._...C.
 800110 be009c00 b0004c6f 6f70733a 20002e20  ......Loops: .. 
 800120 54696d65 3a200020 5b6d735d 000d0a00  Time: . [ms]....

Disassembly of section .data:

00800100 <__data_start>:
  800100:	c0 c6       	rjmp	.+3456   	; 0x800e82 <__bss_end+0xca8>
  800102:	2d 00       	.word	0x002d	; ????

void setup()
{
  Serial.begin(57600);
    
  _startTime_ms = millis();
 5dc:	80 93 30 01 	sts	0x0130, r24	; 0x800130 <__data_end>
 5e0:	90 93 31 01 	sts	0x0131, r25	; 0x800131 <__data_end+0x1>
 5e4:	a0 93 32 01 	sts	0x0132, r26	; 0x800132 <__data_end+0x2>
 5e8:	b0 93 33 01 	sts	0x0133, r27	; 0x800133 <__data_end+0x3>
}

void loop() 
{
  if (0 == _loopCounter--) {

Load bytes to register r24..r27 (low to high)
source is data area with address 0x800100 to 0x8001003 (low to high)
0x002dc6c0 hexdecimal = 3000000 decimal
4 * 2 = 8 cycles

 5ec:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__data_start>
 5f0:	90 91 01 01 	lds	r25, 0x0101	; 0x800101 <__data_start+0x1>
 5f4:	a0 91 02 01 	lds	r26, 0x0102	; 0x800102 <__data_start+0x2>
 5f8:	b0 91 03 01 	lds	r27, 0x0103	; 0x800103 <__data_start+0x3>

move word (two bytes) from register r24, r25 to register r20, r21
move word (two bytes) from register r26, r27 to register r22, r23
2 * 1 cycle = 2 cycles

 5fc:	ac 01       	movw	r20, r24
 5fe:	bd 01       	movw	r22, r26

subtract 1 from the lowest byte in register r20
this is the value of the byte on address 0x800100 (initially 0xc6, lowest byte from 0x002dc6c0)

 600:	41 50       	subi	r20, 0x01	; 1

"carry over" affecting 2nd byte? carry over is "1" in case we subtract 1 from 0x00 which results in 0xFF (overrun)

 602:	51 09       	sbc	r21, r1

"carry over" affecting 3rd and 4th byte?

 604:	61 09       	sbc	r22, r1
 606:	71 09       	sbc	r23, r1

4 * 1 = 4 cycles

Now store the register values back in our memory location
4 * 2 cycles = 8 cycles

 608:	40 93 00 01 	sts	0x0100, r20	; 0x800100 <__data_start>
 60c:	50 93 01 01 	sts	0x0101, r21	; 0x800101 <__data_start+0x1>
 610:	60 93 02 01 	sts	0x0102, r22	; 0x800102 <__data_start+0x2>
 614:	70 93 03 01 	sts	0x0103, r23	; 0x800103 <__data_start+0x3>

perform or operation on registers r24, r25, r26, r27
the or operation (r24 | r25| r26 | r27) calculates a bitwise or, the result can only be zero if all registers are zero
4 * 1 = 4 cycles

 618:	89 2b       	or	r24, r25
 61a:	8a 2b       	or	r24, r26
 61c:	8b 2b       	or	r24, r27

branch (jump) to address if last operation not resulted in zero
1 cycle

 61e:	a9 f5       	brne	.+106    	; 0x68a <main+0x1be>
}


25 cycles in total


