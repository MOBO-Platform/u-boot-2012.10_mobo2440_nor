#include <config.h>
#define __REGb(x) (*(volatile unsigned char *)(x))
#define __REGi(x) (*(volatile unsigned int *)(x))

#define NF_BASE 0x4e000000

#define NFCONF __REGi(NF_BASE + 0x0)
#define NFCONT __REGi(NF_BASE + 0x4)
#define NFCMD __REGb(NF_BASE + 0x8)

#define NFADDR __REGb(NF_BASE + 0xC)
#define NFDATA __REGb(NF_BASE + 0x10)
#define NFSTAT __REGb(NF_BASE + 0x20)

#define NAND_CHIP_ENABLE (NFCONT &= ~(1<<1))
#define NAND_CHIP_DISABLE (NFCONT |= (1<<1))
#define NAND_CLEAR_RB (NFSTAT |= (1<<2))
#define NAND_DETECT_RB { while(! (NFSTAT&(1<<0)) );}

#define BUSY 4
inline void wait_idle(void) {
while(!(NFSTAT & BUSY));
NFSTAT |= BUSY;
}

#define NAND_SECTOR_SIZE 512
#define NAND_BLOCK_MASK (NAND_SECTOR_SIZE - 1)

/* low level nand read function */
/*下面 nand_read_ll 的三个参数来自 start.S 里面调用 nand_read_ll 前的 r0、r1、r2*/
int nand_read_ll(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
	/*下面这个 if 保证对 flash 的读操作是从某一页的页头开始的,从直观来看是保证
	start_addr[0:8]位都为 0,为什么呢?因为本 flash 的一页的大小位 512-bytes,也就是从
	0x0 到 0x1ff*/
	if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
	return -1;
	/* invalid alignment */
	}
	NAND_CHIP_ENABLE;
	for(i=start_addr; i < (start_addr + size);) {
	/* READ0 */
	NAND_CLEAR_RB;
	/*到此应该可以明白 s3c2440 nandflash 相关寄存器的确切含义了,就是说 s3c2440
	里面已经集成了对 nand flash 操
	作的相关寄存器,只要你的 nand flash 接线符合 s3c2440 datasheet 的接法,就可
	以随便使用 s3c2440 对于 nandflash 的相关寄存器,例如如果你想像 nand flash 写一个
	命令,那么只要对命令寄存器写入你的命令就可以了,s3c2440 可以自动帮你完成所有的
	时序动作,
	写地址也是一样。
	反过来说如果没有了对 nand flash 的支持,
	那么我们对 nand
	falsh 的操作就会增加好多对 I/O 口的控制,例如对 CLE,ALE 的控制。s3c2440 已经帮我
	们完成了这部分工作了*/
	NFCMD = 0;
	/* Write Address */
	/*下面这个送地址的过程可以说是这段程序里最难懂的一部分了,难就难于为什么送进
	nand flash 的地址忽略了 bit8,纵观整个 for(i) 循环,i 并不是一个随机的地址,而应该是
	每一页的首地址。其实 nand flash 并不是忽略了 bit 8 这个地址,而是 bit 8 早就被定下来
	了,什么时候定下来,就是上面的 NFCMD = 0;语句,本 flash (K9F1208U0B)支持从半页
	开始读取,从而它有两个读的命令,分别是 0x00(从一页的上半页开始读) 和 0x01(从一页
	的下半页开始读),
	当取 0x00 时,bit 8=0,当取 0x01 时 bit 8=1.*/
	NFADDR = i & 0xff;
	NFADDR = (i >> 9) & 0xff;
	NFADDR = (i >> 17) & 0xff;
	NFADDR = (i >> 25) & 0xff;

	NFCMD = 0x30;
	int k=20;
	while(k--);

	NAND_DETECT_RB;
	for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
	*buf = (NFDATA & 0xff); /*每读一次 NANDFLASH 就往 IO 口送下一个 byte,直
	到送完 NAND_SECTOR_SIZE 个为止*/
	buf++;
	}
	}
	NAND_CHIP_DISABLE;
	return 0;
}






