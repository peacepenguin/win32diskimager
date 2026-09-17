<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>MainWindow</name>
    <message>
        <source>Win32 Disk Imager</source>
        <translation type="vanished">Win32 磁盘映像工具</translation>
    </message>
    <message>
        <source>Image File</source>
        <translation>映像文件</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>校验</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation>读取时缩小映像</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation>读取设备的 MBR 或 GPT，将映像缩小到仅包含实际分区。将备份 GPT 移到已用空间的末尾。</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation>读取为 .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation>使用 gz 压缩从设备读取的映像</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation>读取为 .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation>使用 xz 压缩从设备读取的映像</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>退出 WinDiskImager</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">退出 Win Disk Imager</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation>检查 GPT</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win 磁盘映像工具</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>检查当前所选设备的 GPT 是否损坏，若已损坏则提供修复。</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>映像文件哈希</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>为映像文件生成的哈希类型</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>为文件生成所选的哈希值</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>生成</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>复制哈希值到剪贴板</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>复制</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>写入后修复 GPT</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>写入后, 将备份 GPT 移到设备末尾并相应更新头部, 使 Windows 没有可&quot;修复&quot;的内容. 不勾选则改为提示您移除设备.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>显示所有设备</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>同时列出固定磁盘. 内置 PCIe 读卡器常把存储卡显示为不可移动设备, 否则将被隐藏. 运行 Windows 的磁盘永远不会被列出.</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>任务进度</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>取消任务。</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>从“设备”向“映像文件”传送数据</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>读取</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>从“映像文件”向“设备”传送数据</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>写入</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>将“设备”中的数据与“映像文件”比较</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">与选择的驱动盘校验映像文件</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">仅校验</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">退出 Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>退出</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>退出?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>现在退出将造成映像文件不完整.
仍然退出?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>现在退出将造成磁盘文件不完整.
仍然退出?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>选择一个磁盘映像</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>生成中…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>取消?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>现在取消会造成目的文件不完整.
仍然取消?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>写入错误</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>映像文件不能位于目标设备上.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>确认覆盖</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">等待任务完成。.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>现在退出将取消校验映像文件.
仍然退出?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>取消校验.
确定要取消?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>磁盘空间不足！</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>文件错误</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">写入成功, 但分区表存在风险.

%1
%2

%3

请立即物理移除该设备, 不要进行任何其他操作, 也不要将其重新插入本计算机. 请将其插入目标硬件.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>文件不存在.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>该文件为空文件.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>完成.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>写入成功.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation>磁盘映像 (*.img *.IMG *.img.gz *.img.xz)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation>压缩磁盘映像 (*.img.gz *.img.xz *.gz *.xz)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation>无法打开文件以生成校验和:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>请选择目标设备.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>此设备上的所有文件和数据都将被删除。
(目标设备: %1)
确定要继续吗?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>设备上有已挂载的卷</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 在 Windows 中挂载为 %2.

该设备上所有分区中的全部内容都将被销毁且无法恢复.

请确认 %2 不是您想要保留的驱动器.

仍要写入此设备?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>写入失败。</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>设备错误</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>设备报告的容量为零. 如果这是读卡器, 存储卡可能已被取出.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>映像大于设备:
  映像: 至少 %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

映像的末尾将不会被写入, 因此设备中不会包含完整的映像.

仍然继续?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>所需空间超过可用空间:
  需要: %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

由于映像已压缩, 无法检查多出的空间中是否含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>所需空间超过可用空间:
  需要: %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

多出的空间中似乎含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>所需空间超过可用空间:
  需要: %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

多出的空间中似乎不含数据

仍然继续?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>已取消写入。</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>正在清除旧的分区表…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>无法清除设备上已有的分区表.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>设备仅被部分写入，不再包含可用的映像。请在使用前重新写入映像。</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>正在修复 GPT…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>映像被截断</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">写入成功.

GPT 已与设备 (%1) 保持一致, 因此 Windows 没有损坏的分区表需要修复. 可以正常移除设备.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">写入成功.

映像中不含 GPT, 因此没有分区表需要 Windows 修复. 可以正常移除设备.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>写入成功.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>写入成功</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">设备已脱机并弹出.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">无法自动将设备脱机.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">无法自动修复 GPT (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">GPT 格式有误</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>修复 GPT 失败 (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>写入错误</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">未启用&quot;写入后修复 GPT&quot;选项.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">此映像会受 Windows GPT 重写缺陷的影响.

它在第一个分区之前保留了空间, 因此重新扫描时 Windows 会重写主分区表, 使其指向错误的扇区. 重写结果仍能通过 Windows 自身的检查, 但 Linux 会拒绝它, 设备也无法引导.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">此映像不会受 Windows GPT 重写缺陷的影响.

由于备份 GPT 不在设备末尾, 重新扫描时 Windows 仍会重写分区表, 但对于这种布局, 重写得到的值是正确的. 无论如何, 现在移除设备可使其与映像保持逐字节一致.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">无法确定此映像是否受 Windows GPT 重写缺陷的影响. 请按受影响处理: 重新扫描后, 分区表可能会被 Linux 拒绝, 设备也可能无法引导.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>请立即移除设备</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>您没有读取所选文件的权限.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">映像只能以未压缩的形式读出. 请选择不带 .gz 或 .xz 扩展名的文件名.

压缩映像 (.img.gz, .img.xz) 可以用于写入和校验.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>读取失败。</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>校验失败。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>映像大于设备:
  映像: 至少 %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

只能校验能够容纳的部分.

仍然继续?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>映像大小超过设备:
  映像: %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

由于映像已压缩, 无法检查多出的空间中是否含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>已取消校验。</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>正在校验…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>分区表已损坏</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>修复失败</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>无法修复分区表：%1</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>无法读取设备的扇区 %1。</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>设备中的映像正确无误，但其分区表已损坏：</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>映像大于设备</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>设备中的映像正确无误，但其分区表仍然损坏。请勾选“写入后修复 GPT”后重新写入映像，或再次执行校验并接受修复。</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>校验成功。

设备的分区表原本已损坏，现已修复。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>校验成功。

映像与设备仅在 GPT 上存在差异，且设备上的 GPT 有效。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>校验成功。

映像与设备仅在 GPT 上存在差异。</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[磁盘 %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>请指定要使用的映像文件.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>正在扫描磁盘…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation>无法读取整个文件以生成校验和：
%1</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1，主 GPT 头部所指向的扇区中并没有分区表项。

未启用“写入后修复 GPT”就写入存储卡时，Windows 重新扫描后便会留下这种状态。数据并未丢失，但设备将无法启动，而且大多数工具会拒绝该分区表。

现在修复分区表吗？</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>请选择设备。</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>无法锁定设备。</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>无法打开设备。</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>该设备的分区表已损坏：</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>分区表已修复。</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>分区表仍然损坏。</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>分区表有效。</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>分区表</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>该设备上的 GPT 有效：头部与其指向的分区表项一致。</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>该设备上没有 GPT。</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>该设备没有 GPT，因此不会出现本项检查所针对的损坏。</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>无法读取分区表。</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>无法读取分区表，或者其损坏方式不属于本功能所能修复的类型。</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>正在写入…</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>映像大于设备, 因此其末尾未被写入, 设备中没有完整的映像.

由于压缩映像不记录其未压缩大小, 只有在设备写满后才能发现这一点.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>写入成功。

GPT 现已与设备 (%1) 一致，Windows 没有需要修复的内容。可正常移除设备。</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>写入成功。

该映像使用 MBR 分区表而非 GPT，因此不会受 Windows GPT 重写缺陷影响。可正常移除设备。</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>写入成功。

该映像没有分区表，因此不会受 Windows GPT 重写缺陷影响。可正常移除设备。</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>设备已脱机并弹出。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>无法将设备脱机。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>无法修复 GPT (%1)。</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>GPT 格式错误</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>“写入后修复 GPT”未启用。</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>该映像会受影响：它在第一个分区之前预留了空间，因此重新扫描会使主分区表指向错误的扇区。Windows 仍会接受该结果，Linux 不会，设备将无法启动。</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>该映像不受影响：重新扫描仍会重写分区表，但对这种布局写入的是正确的值。现在移除设备，无论如何都与映像保持一致。</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>无法确定该映像是否受影响。请按受影响处理：重新扫描可能留下 Linux 拒绝的分区表，设备将无法启动。</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>写入成功，但分区表存在风险。

%1 %2

%3

请立即移除设备，不要再插入本机。直接插入目标硬件。</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>读取错误</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation>映像只能以未压缩方式读回。请选择不带 .gz 或 .xz 扩展名的文件名，或勾选&quot;读取为 .img.gz&quot;或&quot;读取为 .img.xz&quot;。</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>请选择源设备.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>确认覆盖</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>确认覆盖已存在的文件?</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>磁盘容量不足以容纳指定的映像.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>正在读取…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>读取已取消.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>读取成功.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>文件信息</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>请选择要保存的文件路径.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>校验错误</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>请选择用于校验的设备.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>映像大小超过设备:
  映像: %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

多出的空间中似乎含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>映像大小超过设备:
  映像: %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

多出的空间中似乎不含数据

仍然继续?</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>大小不匹配!</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>校验失败</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>校验失败的扇区: %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>映像大于设备, 因此只能校验能够容纳的部分. 已校验的部分全部一致, 但设备中没有完整的映像.

由于压缩映像不记录其未压缩大小, 只有到达设备末尾时才能发现这一点.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">校验成功.

映像与设备仅在 GPT 上存在差异, 而&quot;写入后修复 GPT&quot;选项本就会重写这一部分.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

设备已弹出. 请立即移除.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

无法自动将设备脱机.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>校验成功.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>文件错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>获取文件句柄失败.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>设备错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>获取设备句柄失败.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>无法获取存放 %1 的卷的剩余空间.
错误 %2: %3
已忽略剩余空间检查.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>锁定错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">锁该卷时发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>解锁错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>解锁该卷时失败.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>卸载错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>卸载卷时发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>读取错误</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>扇区数量过大。</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>无法为读取缓冲区分配内存。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>从该句柄中读取数据发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>写入错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>向该句柄写入数据时发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>设备仅接受了 %2 个字节中的 %1 个. 设备上的映像并不完整.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>无法获取设备的磁盘几何信息.
错误 %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>无法获取文件大小.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>剩余空间错误</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">无法得到驱动器剩余空间 %1.
错误 %2: %3
已忽略剩余空间检查.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>未知设备</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>无法锁定卷 %1: 它仍在使用中.
请关闭所有正在使用该设备的程序后重试.
错误 %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>主 GPT 头部的大小超出范围</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>主 GPT 头部的校验和无效</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT 分区表项数组不在头部所指示的位置</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>GPT 分区表项数组无法容纳于该设备</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>GPT 分区表项数组的校验和无效</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>有分区超出了设备的末尾</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>备份 GPT 已移至 LBA %1; 最后可用 LBA 现为 %2; 位于 LBA %3 的过时副本已清除</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>备份 GPT 已移至 LBA %1; 最后可用 LBA 现为 %2</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>MBR 中没有可供缩小的分区</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>重新打包后的布局已超出 32 位 MBR 表项的范围</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>设备已经这么紧凑了，没有可缩小的空间</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA 不适用于重新打包</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>GPT 中没有可供缩小的分区</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>某个分区表项描述了一个不可能的范围</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>设备的磁盘几何信息不可用</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>无法读取主 GPT 头部</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>GPT 分区表项数组的几何信息不可用</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>设备太小, 无法容纳分区表项数组</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>无法读取分区表项</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>分区表项不在 LBA 2, 因此这不是本功能所能修复的损坏</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>无法写入修复后的头部</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA 已重新指向 LBA 2, 头部校验和也已重新计算</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>设备报告的扇区大小为零.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>无法打开映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>无法读取映像文件的大小 (错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>无法读取映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>无法将映像文件回退到开头 (错误 %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>无法启动 gzip 解压程序 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>无法启动 xz 解压程序 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>映像文件在压缩数据中间结束. 它已被截断或损坏.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>无法解压该 gzip 映像.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>该 gzip 映像已损坏 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>该 xz 映像已损坏 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>压缩映像只能向前读取.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>无法创建映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>无法启动 gzip 压缩程序 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>无法启动 xz 压缩程序 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>gzip 压缩失败 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>xz 压缩失败 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>无法写入映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>无法刷新映像文件缓冲区 (错误 %1).</translation>
    </message>
</context>
</TS>
