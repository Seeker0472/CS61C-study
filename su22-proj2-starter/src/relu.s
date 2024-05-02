.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	# Prologue
ebreak

addi a2,zero,0 # a2=0

# 如果小于1就直接退出
blt zero,a1,loop_start
addi a0,zero,36
j exit

loop_start:

lw a3,0(a0)
bge a3,zero,loop_continue
sw zero,0(a0)


loop_continue:

addi a2,a2,1
addi a0,a0,4
blt a2,a1,loop_start


loop_end:
addi a0,zero,0

	# Epilogue


	ret
