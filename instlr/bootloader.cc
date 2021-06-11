#include "instlr.hh"
#include <fstream>

bool instlr::bootloader_uefi()
{
    string _target /*!< Efi Target */
        = "x86_64-efi";

    if (conf["boot"]["target"])
        _target = conf["boot"]["target"].as<string>();

    string efi_node /*!< Path to efi device */
        = conf["boot"]["path"].as<string>();

    string efi_path_ = _work_dir + "/boot/efi";
    if (!is_mounted(efi_path_.c_str()))
        fs::create_directories(efi_path_);

    int status = rlx::utils::exec::command("mount -t auto " + efi_node + " " + efi_path_);
    if (status != 0)
    {
        errmesg = "BootError: Failed to mount efi device " + string(strerror(errno));
        return false;
    }

    string cmd = "grub-install --root-directory=" + _work_dir + " --target=" + _target + " --bootloader-id=rlxos  --recheck";
    if (WEXITSTATUS(system(cmd.c_str())) != 0)
    {
        errmesg = "BootError: Failed to install legacy grub bootloader";
        return false;
    }
    return true;
}

bool instlr::bootloader_legacy()
{
    string disk_node /*!< Path to efi device */
        = conf["boot"]["path"].as<string>();

    string cmd = "grub-install --root-directory=" + _work_dir + " " + disk_node + " --recheck";
    if (WEXITSTATUS(system(cmd.c_str())) != 0)
    {
        errmesg = "BootError: Failed to install legacy grub bootloader";
        return false;
    }
    return true;
}

bool instlr::bootloader()
{
    string boot_mode /*!< Bootloader mode */
        = conf["boot"]["mode"].as<string>();

    string root_disk /*!< root disk */
        = conf["roots"]["disk"].as<string>();

    string _system_ver /*!< system version */
        = conf["system"]["version"].as<string>();

    if (boot_mode == "uefi")
    {
        if (!bootloader_uefi())
            return false;
    }
    else if (boot_mode == "legacy")
    {
        if (!bootloader_legacy())
            return false;
    }
    else
    {
        errmesg = "BootError: Unknow boot mode - " + boot_mode;
        return false;
    }

    string grub_cfg = "insmod part_gpt\ninsmod part_msdos\ninsmod all_video\ntimeout=5\ndefault=rlxos\nmenuentry 'rlxos' {\n\tinsmod gzio\n\tinsmod ext2\n\tlinux /boot/vmlinuz root=" + root_disk + " system=" + _system_ver + "\n" + "\tinitrd /boot/initrd\n}";

    /* Using grub_cfg from config */
    if (conf["boot"]["grub"])
        grub_cfg = conf["boot"]["grub"].as<string>();

    std::ofstream file(_work_dir + "/boot/grub/grub.cfg");

    file << grub_cfg << std::endl;
    file.close();

    string _kernel_dir = "/run/iso/boot";

    if (!fs::copy_file(_kernel_dir + "/vmlinuz", _work_dir + "/boot/vmlinuz"))
    {
        errmesg = "BootError: Failed to copy kernel into system";
        return false;
    }

    if (!fs::copy_file(_kernel_dir + "/initrd", _work_dir + "/boot/initrd"))
    {
        errmesg = "BootError: Failed to copy initrd into system";
        return false;
    }

    return true;
}
