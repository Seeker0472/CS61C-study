.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

	# Prologue
ebreak

# a7 存储结果
addi a7,zero,4
mul a3,a3,a7
mul a4,a4,a7
addi a7,zero,0

# 数组的长度小于1
blt zero,a2,next1
addi a0,zero,36
j exit
next1:
# 第一个数组的步长小于1
blt zero,a3,next2
addi a0,zero,37
j exit
next2:
# 第二个数组的步长小于1
blt zero,a4,loop_start
addi a0,zero,37
j exit

loop_start:
ebreak

# 更新a7
lw a5,0(a0)
lw a6,0(a1)
mul a5,a5,a6
add a7,a7,a5

add a0,a0,a3
add a1,a1,a4

addi a2,a2,-1

blt zero,a2,loop_start


loop_end:
addi a0,a7,0


	# Epilogue


	ret
