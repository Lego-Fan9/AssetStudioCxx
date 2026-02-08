#ifndef SERIALIZED_FILE_FORMAT_VERSION_H
#define SERIALIZED_FILE_FORMAT_VERSION_H

enum SerializedFileFormatVersion_e {
    SerializedFileFormatVersion_Unsupported = 1,
    SerializedFileFormatVersion_Unknown_2 = 2,
    SerializedFileFormatVersion_Unknown_3 = 3,
    SerializedFileFormatVersion_Unknown_5 = 5,                      // 1.2.0 to 2.0.0
    SerializedFileFormatVersion_Unknown_6 = 6,                      // 2.1.0 to 2.6.1
    SerializedFileFormatVersion_Unknown_7 = 7,                      // 3.0.0b
    SerializedFileFormatVersion_Unknown_8 = 8,                      // 3.0.0 to 3.4.2
    SerializedFileFormatVersion_Unknown_9 = 9,                      // 3.5.0 to 4.7.2
    SerializedFileFormatVersion_Unknown_10 = 10,                    // 5.0.0aunk1
    SerializedFileFormatVersion_HasScriptTypeIndex = 11,            // 5.0.0aunk2
    SerializedFileFormatVersion_Unknown_12 = 12,                    // 5.0.0aunk3
    SerializedFileFormatVersion_HasTypeTreeHashes = 13,             // 5.0.0aunk4
    SerializedFileFormatVersion_Unknown_14 = 14,                    //  5.0.0unk
    SerializedFileFormatVersion_SupportsStrippedObject = 15,        // 5.0.1 to 5.4.0
    SerializedFileFormatVersion_RefactoredClassId = 16,             // 5.5.0a
    SerializedFileFormatVersion_RefactorTypeData = 17,              // 5.5.0unk to 2018.4
    SerializedFileFormatVersion_RefactorShareableTypeTreeData = 18, // 2019.1a
    SerializedFileFormatVersion_TypeTreeNodeWithTypeFlags = 19,     // 2019.1unk
    SerializedFileFormatVersion_SupportsRefObject = 20,             // 2019.2
    SerializedFileFormatVersion_StoresTypeDependencies = 21,        // 2019.3 to 2019.4
    SerializedFileFormatVersion_LargeFilesSupport = 22              // 2020.1 to x
};

#endif