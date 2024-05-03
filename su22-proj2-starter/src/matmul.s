.globl matmul
# P.S.没注意到能调用dot函数
.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

	# Error checks
# 检查长宽
blt zero,a1,comp1
j size_error
comp1:
blt zero,a2,comp2
j size_error
comp2:
blt zero,a4,comp3
j size_error
comp3:
blt zero,a5,comp4
j size_error
comp4:
beq a2,a4,comp5

size_error:
addi a0,zero,38
j exit

comp5:

ebreak
	# Prologue

# 保存寄存器
addi sp,sp,-32
sw  s1,0(sp)
sw  s2,4(sp)
sw  s3,8(sp)
sw  s4,12(sp)
sw  s5,16(sp)
sw  s6,20(sp)
sw  s7,24(sp)
sw  s8,28(sp)

addi s7,zero,4 # s7=4

addi s5,zero,0 # s5=0
addi s8,zero,0 # s8=0

addi s1,a3,0    
# a3-not-changed-s1=a3 #

outer_loop_start:



addi a7,a0,0 # a7=a0

addi s4,zero,0 # s4=0
addi s6,zero,0 # s6=0



inner_loop_start:

lw s2,0(a7)
lw s3,0(s1)

mul s2,s2,s3
add s6,s6,s2

# 更新pointer
addi a7,a7,4
mul s2,a5,s7
add s1,s1,s2

# counter
addi s4,s4,1 #s4+=1
blt s4,a4,inner_loop_start

inner_loop_end:

sw s6,0(a6)
add a6,a6,s7 # a6+=4

addi s5,s5,1 # s5+=1

ebreak

# 恢复s1
addi s2,a4,0 #s2=height
mul s2,s2,a5 #s2=(height)*width
mul s2,s2,s7 #s2*=4
sub s1,s1,s2 #s1-=s2

addi s1,s1,4 # s1+=4(指向下一列)

blt s5,a5,outer_loop_start # 矩阵2没有到底
ebreak
addi s1,a3,0 # s1=a3
mul s2,a2,s7 #s2=width*4
add a0,a0,s2 #a0+=(width*4)
addi s8,s8,1 #s8+=1
addi s5,zero,0
blt s8,a1,outer_loop_start


outer_loop_end:


# 恢复寄存器

lw  s1,0(sp)
lw  s2,4(sp)
lw  s3,8(sp)
lw  s4,12(sp)
lw  s5,16(sp)
lw  s6,20(sp)
lw  s7,24(sp)
lw  s8,28(sp)
addi sp,sp,32

	# Epilogue


	ret
