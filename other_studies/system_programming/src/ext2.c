#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fs.h>

#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * 1024)
#define BASE_OFFSET 1024 
#define EXT2_NAME_LEN 255

#define EXT2_NDIR_BLOCKS 12
#define EXT2_IND_BLOCK EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS (EXT2_TIND_BLOCK + 1)

struct ext2_super_block
{
    __u32 s_inodes_count;      /* Inodes count */
    __u32 s_blocks_count;      /* Blocks count */
    __u32 s_r_blocks_count;    /* Reserved blocks count */
    __u32 s_free_blocks_count; /* Free blocks count */
    __u32 s_free_inodes_count; /* Free inodes count */
    __u32 s_first_data_block;  /* First Data Block */
    __u32 s_log_block_size;    /* Block size */
    __s32 s_log_frag_size;     /* Fragment size */
    __u32 s_blocks_per_group;  /* # Blocks per group */
    __u32 s_frags_per_group;   /* # Fragments per group */
    __u32 s_inodes_per_group;  /* # Inodes per group */
    __u32 s_mtime;             /* Mount time */
    __u32 s_wtime;             /* Write time */
    __u16 s_mnt_count;         /* Mount count */
    __s16 s_max_mnt_count;     /* Maximal mount count */
    __u16 s_magic;             /* Magic signature */
    __u16 s_state;             /* File system state */
    __u16 s_errors;            /* Behaviour when detecting errors */
    __u16 s_minor_rev_level;   /* minor revision level */
    __u32 s_lastcheck;         /* time of last check */
    __u32 s_checkinterval;     /* max. time between checks */
    __u32 s_creator_os;        /* OS */
    __u32 s_rev_level;         /* Revision level */
    __u16 s_def_resuid;        /* Default uid for reserved blocks */
    __u16 s_def_resgid;        /* Default gid for reserved blocks */
    __u32 s_first_ino;         /* First non-reserved inode */
    __u16 s_inode_size;        /* size of inode structure */
    __u16 s_block_group_nr;    /* block group # of this superblock */
    __u32 s_feature_compat;    /* compatible feature set */
    __u32 s_feature_incompat;  /* incompatible feature set */
    __u32 s_feature_ro_compat; /* readonly-compatible feature set */
    __u32 s_reserved[230];     /* Padding to the end of the block */
};

struct ext2_group_desc
{
    __u32 bg_block_bitmap; /* Blocks bitmap block */
    __u32 bg_inode_bitmap;
    __u32 bg_inode_table;
    __u16 bg_free_blocks_count;
    __u16 bg_free_inodes_count;
    __u16 bg_used_dirs_count;
    __u16 bg_pad;
    __u32 bg_reserved[3];
};

struct ext2_inode
{

    __u16 i_mode;
    __u16 i_uid;
    __u32 i_size;
    __u32 i_atime;
    __u32 i_ctime;
    __u32 i_mtime;
    __u32 i_dtime;
    __u16 i_gid;
    __u16 i_links_count;
    __u32 i_blocks;
    __u32 i_flags;
    union
    {
        struct
        {
            __u32 l_i_reserved1;
        } linux1;
        struct
        {
            __u32 h_i_translator;
        } hurd1;
        struct
        {
            __u32 m_i_reserved1;
        } masix1;
    } osd1;

    __u32 i_block[EXT2_N_BLOCKS];
    __u32 i_version;
    __u32 i_file_acl;
    __u32 i_dir_acl;
    __u32 i_faddr;
    union
    {
        struct
        {
            __u8 l_i_frag;
            __u8 l_i_fsize;
            __u32 l_i_reserved2[2];
        } linux2;
        struct
        {
            __u8 h_i_frag;
            __u8 h_i_fsize;
            __u16 h_i_mode_high;
            __u16 h_i_uid_high;
            __u16 h_i_gid_high;
            __u32 h_i_author;
        } hurd2;
        struct
        {
            __u8 m_i_frag;
            __u8 m_i_fsize;
            __u16 m_pad1;
            __u32 m_i_reserved2[2];
        } masix2;
    } osd2;
};

struct ext2_dir_entry_2
{
    __u32 inode;   /* Inode number */
    __u16 rec_len; /* Directory entry length */
    __u8 name_len; /* Name length */
    __u8 file_type;
    char name[EXT2_NAME_LEN]; /* File name */
};

void printSuperblock(int fd);
void printGroupDescriptor(int fd);
void printfilecontent(int fd);


struct ext2_group_desc groupDescriptor;
struct ext2_super_block superblock;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <virtual_disk> <file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *virtualDisk = argv[1];
    const char *filePath = argv[2];

    int fd = open(virtualDisk, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening virtual disk");
        exit(EXIT_FAILURE);
    }

    printSuperblock(fd);

    printGroupDescriptor(fd);

    printfilecontent(fd);

    close(fd);

    return 0;
}

void printSuperblock(int fd)
{
    unsigned int block_size = 0;
    lseek(fd, 1024, SEEK_SET);
    read(fd, &superblock, sizeof(struct ext2_super_block));

    printf("this is :%lu\n", sizeof(struct ext2_super_block));
    block_size = 1024 << superblock.s_log_block_size;
    printf("Reading super-block from device :\n"
           "Inodes count            : %u\n"
           "Blocks count            : %u\n"
           "Reserved blocks count   : %u\n"
           "Free blocks count       : %u\n"
           "Free inodes count       : %u\n"
           "First data block        : %u\n"
           "Block size              : %u\n"
           "Blocks per group        : %u\n"
           "Inodes per group        : %u\n"
           "Creator OS              : %u\n"
           "First non-reserved inode: %u\n"
           "Size of inode structure : %hu\n",
           superblock.s_inodes_count,
           superblock.s_blocks_count,
           superblock.s_r_blocks_count, /* reserved blocks count */
           superblock.s_free_blocks_count,
           superblock.s_free_inodes_count,
           superblock.s_first_data_block,
           block_size,
           superblock.s_blocks_per_group,
           superblock.s_inodes_per_group,
           superblock.s_creator_os,
           superblock.s_first_ino, /* first non-reserved inode */
           superblock.s_inode_size);
}
void printGroupDescriptor(int fd)
{
    unsigned int block_size = 0;
    block_size = BASE_OFFSET + 1024 << superblock.s_log_block_size;

    lseek(fd, block_size, SEEK_SET);
    read(fd, &groupDescriptor, sizeof(struct ext2_group_desc));
    
    if (block_size < 1024 || block_size > 4096)
    {
        fprintf(stderr, "Invalid block size: %u\n", block_size);
        exit(EXIT_FAILURE);
    }

    printf("\nblock size : %u\n\n", superblock.s_log_block_size);
    printf("\nGroup Descriptor Information:\n");

    printf("Block bitmap block: %u\n", groupDescriptor.bg_block_bitmap);
    printf("Inode bitmap block: %u\n", groupDescriptor.bg_inode_bitmap);
    printf("Inode table block: %u\n", groupDescriptor.bg_inode_table);
    printf("Free blocks count: %u\n", groupDescriptor.bg_free_blocks_count);
    printf("Free inodes count: %u\n", groupDescriptor.bg_free_inodes_count);
    printf("Used directories count: %u\n", groupDescriptor.bg_used_dirs_count);
    printf("Reserved blocks: %u, %u, %u\n", groupDescriptor.bg_reserved[0], groupDescriptor.bg_reserved[1], groupDescriptor.bg_reserved[2]);
    printf("\n");


}

void printfilecontent(int fd)
{
    for (int i = 10; i <= 10; ++i)
    {
        
        struct ext2_inode inode;
        lseek(fd, 1024 + 1024 * i, SEEK_SET);
        read(fd, &inode, sizeof(struct ext2_inode));
        lseek(fd, 0, SEEK_SET);

        printf("Inode %d:\n", i);
        printf("i_mode: %u\n", inode.i_mode);
        printf("i_uid: %u\n", inode.i_uid);
        printf("i_size: %u\n", inode.i_size);
        printf("i_atime: %u\n", inode.i_atime);
        printf("i_ctime: %u\n", inode.i_ctime);
        printf("i_mtime: %u\n", inode.i_mtime);
        printf("i_dtime: %u\n", inode.i_dtime);
        printf("i_gid: %u\n", inode.i_gid);
        printf("i_links_count: %u\n", inode.i_links_count);
        printf("i_blocks: %u\n", inode.i_blocks);
        printf("i_flags: %u\n", inode.i_flags);

        printf("i_block: [");
        for (int j = 0; j < EXT2_N_BLOCKS; ++j)
        {
            printf(" %u", inode.i_block[j]);
        }

        printf(" ]\n");
        printf("\n");

        
        struct ext2_dir_entry_2 *entry;
        unsigned int size;
        unsigned char block[BLOCK_SIZE];
        
        lseek(fd, inode.i_block[0] * 1024, SEEK_SET);
        int bytes_read=read(fd, block, BLOCK_SIZE); /* read block from disk*/
        if (bytes_read == -1)
        {
            perror("Error reading block");
            close(fd);
            exit(1);
        }
        printf("file content :\n\n");
        fwrite(block, 1, bytes_read, stdout);
    }
}