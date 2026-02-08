#ifndef ARCHIVE_FLAGS_H
#define ARCHIVE_FLAGS_H

enum ArchiveFlags_e {
    ArchiveFlags_CompressionTypeMask = 0x3f,
    ArchiveFlags_BlocksAndDirectoryInfoCombined = 0x40,
    ArchiveFlags_BlocksInfoAtTheEnd = 0x80,
    ArchiveFlags_OldWebPluginCompatibility = 0x100,
    ArchiveFlags_BlockInfoNeedPaddingAtStart = 0x200
};

#endif