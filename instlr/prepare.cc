#include "instlr.hh"

bool instlr::prepare()
{
    string root_disk_ /*!< path to root disk node */
        = conf["roots"]["disk"].as<string>();

    string root_format_ /*!< format type of root disk */
        = conf["roots"]["format"].as<string>();

    /* unmount root_disk */
    if (is_mounted(root_disk_.c_str()))
        umount(root_disk_.c_str());

    int status /*!< status for process */
        = WEXITSTATUS(system(("mkfs -t " + root_format_ + " -F " + root_disk_).c_str()));

    if (status)
    {
        errmesg = "PrepareError: Failed format " + root_disk_;
        return false;
    }

    _work_dir = "/tmp/rlxos_installer";
    fs::create_directories(_work_dir);
    if (is_mounted(_work_dir.c_str(), true))
        umount(_work_dir.c_str());

    status = mount(root_disk_.c_str(), _work_dir.c_str(), root_format_.c_str(), 0, nullptr);
    if (status)
    {
        errmesg = "MountError: " + string(strerror(errno));
        return false;
    }

    return true;
}
