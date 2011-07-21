	.section .compressed_init_func, "a", @progbits
	.globl	init_func_start
init_func_start:
	.incbin	"./src/init_func.bin.lzma"
	.globl	init_func_end
init_func_end: 

	.section .compressed_csdapp, "a", @progbits
	.globl	csdapp_start
csdapp_start:
	.incbin "./src/csdapp.bin.lzma"
	.globl	csdapp_end
csdapp_end:

	.section .compressed_wap, "a", @progbits
	.globl	wap_start
wap_start:
	.incbin	"./src/wap.bin.lzma"
	.globl	wap_end
wap_end: 

 	.section .compressed_jmetoc, "a", @progbits
	.globl	jmetoc_start
jmetoc_start:
	.incbin	"./src/jmetoc.bin.lzma"
	.globl	jmetoc_end
jmetoc_end:

 	.section .compressed_other, "a", @progbits
	.globl	other_start
other_start:
	.incbin	"./src/other.bin.lzma"
	.globl	other_end
other_end:

