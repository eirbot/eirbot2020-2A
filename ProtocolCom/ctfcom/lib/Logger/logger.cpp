#include "logger.h"

// This will take the system's default block device
BlockDevice *bd = BlockDevice::get_default_instance();


// Uncomment the following two lines and comment the previous two to use FAT file system.
// #include "FATFileSystem.h"
// FATFileSystem fs("fs");

