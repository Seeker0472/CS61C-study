.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
ebreak
addi a2,zero,0 # a2=0
addi a3,zero,0 # a3=0
lw a4,0(a0) # a4=arr[a0]

# 如果小于1就直接退出
blt zero,a1,loop_start
addi a0,zero,36
j exit

loop_start:
lw a5,0(a0)
blt a5,a4,loop_continue
add a3,a2,zero
lw a4,0(a0)


loop_continue:
addi a2,a2, 1
addi a0,a0, 4
blt a2,a1,loop_start


loop_end:
addi a0,a3,0
	# Epilogue

	ret
