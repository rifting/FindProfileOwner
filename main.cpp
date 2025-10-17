#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext2fs.h>
#include <iostream>
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image>\n", argv[0]);
        return 1;
    }

    ext2_filsys fs;
    errcode_t err;

    err = ext2fs_open(argv[1], 0, 0, 0, unix_io_manager, &fs);
    if (err) {
        std::cerr << "Error opening filesystem: " << error_message(err) << std::endl;
        return 1;
    }

    ext2_inode inode;
    uint32_t profile_owner_inode = 0;

    std::cout << "Searching filesystem for profile owner..." << std::endl;

    // inode 1 is reserved for bad blocks so skip
    for (int i = 1; i <= fs->super->s_inodes_count; i++) {
        ext2_ino_t ino = i;

        err = ext2fs_read_inode(fs, ino, &inode);
        if (err) {
            std::cerr << "Error reading inode " << ino << ": " << error_message(err) << std::endl;
            continue;
        }

        uint32_t size = inode.i_size;

        // Profile owner file with com.google.android.gms/.kids.account.receiver.ProfileOwnerReceiver
        // will take up exactly 155 bytes
        // (inode.i_mode & 0xF000) == 32768) is just checking to make sure this is a regular file
        if (size == 155 && inode.i_uid == 1000 && (inode.i_mode & 0xF000) == 32768) {
            if (profile_owner_inode == 0) {
                std::cout << "Potential profile owner contained in inode " << ino << std::endl;
                profile_owner_inode = ino;
            } else {
                std::cerr << "Found multiple potential profile owner inodes. This script won't work for you :(" << std::endl;
                return 1;
            }
            // std::cout << "Size: " << size << std::endl;
            // std::cout << "UID: " << inode.i_uid << std::endl;
            // std::cout << "GID: " << inode.i_gid << std::endl;
            // std::cout << "Imode:" << inode.i_mode << std::endl;
        }
    }

    std::cout << "Confirmed profile owner contained in inode " << profile_owner_inode << "!" << std::endl;

    ext2fs_close(fs);
    return 0;
}
