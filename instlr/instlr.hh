#ifndef __INSTLR__
#define __INSTLR__

#include <cli/cli.hh>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <filesystem>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mount.h>
#include <errno.h>

using std::string;
namespace fs = std::filesystem;

/** @brief Install class provides ABI help
 *  in installation process
 */
struct instlr
{

    /** Initialize the installer object
     * @param conf YAML node
     */
    instlr(YAML::Node const &conf)
        : conf(conf)
    {
    }

    /** Verify the provided configurations
     * @return true if pass and false if fail
     */
    bool verify();

    /** Prepare the system to perform installation
     * @return true if pass
     */
    bool prepare();

    /** Install system image into disk
     */
    bool install();

    /** Configure bootloader into system
     */
    bool bootloader();

    /** Get error mesg
     */
    string const error() const
    {
        return errmesg;
    }

private:
    YAML::Node conf; /*!< Contain configurations required for performing installation */
    string errmesg;  /*!< error mesg */

    string _work_dir; /*!< Temporary working directory */

    bool bootloader_uefi();

    bool bootloader_legacy();

    /** check if directory is mount point or not
     */
    bool is_mounted(char const *path, bool is_dir = true)
    {
        char buffer[1024] = {0};
        if (is_dir)
            sprintf(buffer, "mount | grep \"^%s \"", path);
        else
            sprintf(buffer, "mountpoint %s", path);
            
        return WEXITSTATUS(system(buffer)) == 0;
    }
};

#endif
