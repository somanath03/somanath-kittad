#define SETBIT(WORD,SBP) WORD |= (1 << SBP)
#define CLRBIT(WORD,BP) WORD&=~(1<<BP)
#define TOGBIT(WORD,BP) WORD^=(1<<BP)

#define READBIT(WORD,BP) ((WORD>>BP) & 1)

#define WRITEBIT(WORD,SBP,BIT) WORD = ((WORD &~(1 << SBP)) | (BIT << SBP))
#define WRITENIBBLE(WORD,SBP,NIBBLE) WORD = ((WORD &~(0xf << SBP)) | (NIBBLE << SBP))
#define WRITEBYTE(WORD,SBP,BYTE_D) WORD = ((WORD &~(0xff << SBP)) | (BYTE_D << SBP))
#define WRITEHWORD(WORD,SBP,HWORD) WORD = ((WORD &~(0xffff << SBP)) | (HWORD << SBP))
