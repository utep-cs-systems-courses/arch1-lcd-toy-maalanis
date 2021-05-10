	.arch MSP430g2553
	.p2align 1,0

	.data
box_color:	 .word 0 .skip 0

	.text
jt:	 .word default
	.word case1
	.word case2
	.word case3
	.word case4
	.word case5
	.word case6
	.word case7
	.word case8
	.word case9
	.word case10
	.word case11
	.word case12
	.word case13
	.word case14
	.word case15

change_box:
	mv &color_red, r12
	cmp #15, box_color
	jc defaul
	mov &box_color, r13
	add r13, r13
	mov jt(r13), r0
case1:
	mov &color_green, r12
	add #1, box_color
	jmp end
case2:
	mov &color_green, r12
	add #1, box_color
	jmp end
case3:
	mov &color_green, r12
	add #1, box_color
	jmp end
case4:
	mov &color_green, r12
	add #1, box_color
	jmp end
case5:
	mov &color_green, r12
	add #1, box_color
	jmp end
case6:
	mov &color_green, r12
	add #1, box_color
	jmp end
case7:
	mov &color_green, r12
	add #1, box_color
	jmp end
case8:
	mov &color_green, r12
	add #1, box_color
	jmp end
case9:
	mov &color_green, r12
	add #1, box_color
	jmp end
case10:
	mov &color_green, r12
	add #1, box_color
	jmp end
case11:
	mov &color_green, r12
	add #1, box_color
	jmp end
case12:
	mov &color_green, r12
	add #1, box_color
	jmp end
case13:
	mov &color_green, r12
	add #1, box_color
	jmp end
case14:
	mov &color_green, r12
	add #1, box_color
	jmp end
case15:
	mov &color_green, r12
	add #1, box_color
	jmp end
default:
	mov &color_green, r12
	add #1, box_color
	jmp end
end:
	
	         
