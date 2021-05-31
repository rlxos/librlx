#include "instlr.hh"

bool instlr::install()
{
    /*if (!is_mounted(_work_dir.c_str()))
    {
        errmesg = "InstallError: Working directory is not a mount point";
        return false;
    }*/

    string img_path /*!< System Image Path */
        = conf["system"]["image"].as<string>();

    string version /*!< System Version */
        = conf["system"]["version"].as<string>();

    string rlxos_dir_ = _work_dir + "/rlxos/system/";

    if (!fs::exists(img_path))
    {
        errmesg = "InstallError: '" + img_path + "' System Image not exist";
        return false;
    }

    if (!fs::create_directories(rlxos_dir_))
    {
        errmesg = "InstallError: Failed to create directory '" + rlxos_dir_ + "'";
        return false;
    }

    if (!fs::copy_file(img_path, rlxos_dir_ + "/" + version))
    {
        errmesg = "InstallError: Failed to install system image";
        return false;
    }

    return true;
}
