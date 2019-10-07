SIFS PROJECT

关于一个文件系统的所有信息都存在一个单一文件中，本文件是由 mkvolume 通过 fwrite 生成的，本 Project 所有的操作都是需要对这个文件进行读写，所以一旦理解文件，剩余要做的事务便是通过修改文件实现不同的功能。 同时该文件可以上传至 Chris 提供的 sifs-viewer 分析。

SIFS 文件一共是 2+nblocks 个结构体组成的，在文件的头部是以下架构体
    SIFS_VOLUME_HEADER	header = {
            .blocksize	= blocksize, //该文件系统中每个 block 的大小
            .nblocks	= nblocks, //该文件系统共有多少个 block
        };
该文件一旦生成后上述相关参数则无法改变

其次是一个等同于 nblocks 大小的 char 数组
    SIFS_BIT	bitmap[nblocks];
储存了该文件中每一个 block 的类型，其实是使用一个字符来代表
#define SIFS_UNUSED		'u'
#define SIFS_DIR		'd'
#define SIFS_FILE		'f'
#define SIFS_DATABLOCK		'b'
文件剩下的部分储存的就是所有的 block 每个 blocksize 大小 一共 nblocks 个。

block 介绍
每个 block 的本质其实也是 char 数组而已，数组的大小是 blocksize 
但是如上所示，有四种不同的 block
SIFS_UNUSED		'u' 没有使用的 block 中所有的值都是 0
SIFS_DATABLOCK	'b' 储存文件内容的 block 每个值都是文件的内容 到'\0'即意味 EOF(End Of File)
SIFS_DIR		'd' 存储目录信息的 block （文件中的第一个 block 永远是根目录 block) 该 block 的内容由 'SIFS_DIRBLOCK' 结构体决定，记录文件夹的信息

    typedef struct {
        char		name[SIFS_MAX_NAME_LENGTH];
        time_t		modtime;	// time last modified <- time()

        uint32_t		nentries;
        struct {
        SIFS_BLOCKID	blockID;	// of the entry's subdirectory or file
        uint32_t	fileindex;	// into a SIFS_FILEBLOCK's filenames[]
        } entries[SIFS_MAX_ENTRIES];
    } SIFS_DIRBLOCK;

其中 entries 结构体记录了文件夹中所有的文件从哪个 Block 开始，以及这个文件的文件名的序号（为什么是序号将会在文件结构体中揭晓）
SIFS_FILE		'f' 存储文件信息，其中的内容是以下结构体
    typedef struct {
        time_t		modtime;	// time first file added <- time()
        size_t		length;		// length of files' contents in bytes

        unsigned char	md5[MD5_BYTELEN];
        SIFS_BLOCKID	firstblockID;

        uint32_t		nfiles;		// n files with identical contents
        char		filenames[SIFS_MAX_ENTRIES][SIFS_MAX_NAME_LENGTH];
    } SIFS_FILEBLOCK;
其中值得注意的是 文件名存在最后一个数组中，我们设计的 SIFS 是一个可以有重复文件但该文件事实上只物理存在一份，所有其他的文件“副本”都只是另一个文件名而已，所以在文件夹结构体中 entries 我们有记录本文件夹存放文件的 Block ID 以及 文件名的序号。

这就是 SIFS 文件的所有内容了，所以我们要做的任务并没有技术上的难度，只要正确的修改以上结构体即可，一个示范是提供的 mkvolume.c 文件，初始文件的生成也可以通过本文件进行了解。如果调用写好的 function 可以通过编译并运行 sifs_mkvolume 程序，使用 make 进行编译。