<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="+32"/>
        <source>Win32 Disk Imager</source>
        <translation>Win32 磁蝶映像檔工具</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Image File</source>
        <translation>映像檔</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Verify</source>
        <translation>驗證</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Device</source>
        <translation>裝置</translation>
    </message>
    <message>
        <location line="+99"/>
        <source>Check GPT</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Image File Hash</source>
        <translation>映像檔雜湊值</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Hash type to generate for image file</source>
        <translation>要為映像檔產生的雜湊類型</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Generate selected hash on file</source>
        <translation>生成檔案所選取的雜湊值類型</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Generate</source>
        <translation>生成</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Copy hash to clipboard</source>
        <translation>複製雜湊值到剪貼簿中</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Copy</source>
        <translation>複製</translation>
    </message>
    <message>
        <location line="-131"/>
        <source>Fix GPT after write</source>
        <translation>寫入後修正 GPT</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>寫入後，將備份 GPT 移到裝置結尾並更新標頭使其相符，讓 Windows 沒有可「修復」的項目。不勾選則改為提醒您移除裝置。</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Show all devices</source>
        <translation>顯示所有裝置</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>同時列出固定式磁碟。內建 PCIe 讀卡機常將記憶卡顯示為不可卸除式裝置，否則便會被隱藏。執行 Windows 的磁碟永遠不會被列出。</translation>
    </message>
    <message>
        <location line="+158"/>
        <source>Progress</source>
        <translation>進度</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Cancel current process.</source>
        <translation>取消目前的進度。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location line="-316"/>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>從「裝置」讀取資料到「映像檔」中</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Read</source>
        <translation>讀取</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>從「映像檔」寫入資料到「裝置」中</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Write</source>
        <translation>寫入資料到「裝置」中</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>比對「裝置」中的資料與「映像檔」</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">驗證選定磁碟的映像檔</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">僅驗證</translation>
    </message>
    <message>
        <location line="+364"/>
        <source>Exit Win32 Disk Imager</source>
        <translation>離開 Win32 磁蝶映像檔工具</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Exit</source>
        <translation>離開</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="+533"/>
        <source>Exit?</source>
        <translation>離開？</translation>
    </message>
    <message>
        <location line="-18"/>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>現在離開將會導致映像檔損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>現在離開將會導致磁碟損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Select a disk image</source>
        <translation>選擇一個磁碟映像檔</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Generating...</source>
        <translation>生成中...</translation>
    </message>
    <message>
        <location line="+181"/>
        <location line="+9"/>
        <source>Cancel?</source>
        <translation>取消？</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>現在離開將會導致目標檔案損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <location line="+30"/>
        <location line="+6"/>
        <location line="+38"/>
        <location line="+90"/>
        <location line="+24"/>
        <location line="+22"/>
        <location line="+243"/>
        <location line="+81"/>
        <source>Write Error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <location line="-504"/>
        <location line="+423"/>
        <location line="+168"/>
        <source>Image file cannot be located on the target device.</source>
        <translation>在目標裝置上找不到映像檔。</translation>
    </message>
    <message>
        <location line="-582"/>
        <source>Confirm overwrite</source>
        <translation>確認覆寫</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">等待工作。</translation>
    </message>
    <message>
        <location line="-295"/>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>現在結束將會取消驗證映像檔。
您確定想要結束嗎？</translation>
    </message>
    <message>
        <location line="+265"/>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>取消驗證。
您確定您想要取消嗎？</translation>
    </message>
    <message>
        <location line="+101"/>
        <location line="+31"/>
        <source>Not enough available space!</source>
        <translation>沒有足夠的可用空間！</translation>
    </message>
    <message>
        <location line="-307"/>
        <location line="+11"/>
        <location line="+245"/>
        <location line="+281"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+14"/>
        <location line="+260"/>
        <location line="+271"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+13"/>
        <source>File Error</source>
        <translation>檔案錯誤</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">寫入成功，但分割表有風險。

%1
%2

%3

請立刻將裝置實體移除，在此之前不要進行任何其他操作，也不要將它重新插入這台電腦。請將它插入目標硬體。</translation>
    </message>
    <message>
        <location line="-578"/>
        <location line="+555"/>
        <source>The selected file does not exist.</source>
        <translation>選取的檔案不存在。</translation>
    </message>
    <message>
        <location line="-835"/>
        <location line="+290"/>
        <location line="+275"/>
        <location line="+280"/>
        <source>The specified file contains no data.</source>
        <translation>指定的檔案不包含資料。</translation>
    </message>
    <message>
        <location line="-551"/>
        <location line="+200"/>
        <location line="+355"/>
        <source>Done.</source>
        <translation>完成。</translation>
    </message>
    <message>
        <location line="-351"/>
        <location line="+2"/>
        <location line="+328"/>
        <location line="+25"/>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <location line="-555"/>
        <source>Write Successful.</source>
        <translation>寫入成功。</translation>
    </message>
    <message>
        <location line="-742"/>
        <location line="+1"/>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation>磁碟映像檔 (*.img *.IMG *.img.gz *.img.xz)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation>壓縮磁碟映像檔 (*.img.gz *.img.xz *.gz *.xz)</translation>
    </message>
    <message>
        <location line="+182"/>
        <location line="+11"/>
        <source>Error</source>
        <translation>錯誤</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation>無法開啟檔案以產生總和檢查碼：
%1</translation>
    </message>
    <message>
        <location line="+201"/>
        <source>Please select a target device.</source>
        <translation>請選擇目標裝置。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Writing to a physical device can corrupt the device.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>寫入實體裝置可能會損壞該裝置。
(目標裝置：%1)
您確定要繼續嗎？</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Device has mounted volumes</source>
        <translation>裝置有已掛載的磁碟區</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 在 Windows 中掛載為 %2。

此裝置上所有分割區中的一切都將被銷毀且無法復原。

請確認 %2 不是您想要保留的磁碟機。

仍要寫入此裝置嗎？</translation>
    </message>
    <message>
        <location line="+21"/>
        <location line="+21"/>
        <location line="+21"/>
        <location line="+55"/>
        <location line="+24"/>
        <location line="+23"/>
        <source>Write failed.</source>
        <translation>寫入失敗。</translation>
    </message>
    <message>
        <location line="-788"/>
        <location line="+464"/>
        <location line="+24"/>
        <location line="+590"/>
        <source>Device Error</source>
        <translation>裝置錯誤</translation>
    </message>
    <message>
        <location line="-1077"/>
        <location line="+488"/>
        <location line="+590"/>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>裝置回報的大小為零。若這是讀卡機，記憶卡可能已被取出。</translation>
    </message>
    <message>
        <location line="-405"/>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>映像檔大於裝置：
  映像檔：至少 %1 個磁區
  可用：%2 個磁區
  磁區大小：%3

映像檔的結尾將不會被寫入，因此裝置中不會包含完整的映像檔。

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>所需空間超過可用空間：
  需要：%1 個磁區
  可用：%2 個磁區
  磁區大小：%3

由於映像檔已壓縮，無法檢查多出的空間中是否含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>所需空間超過可用空間：
  需要：%1 個磁區
  可用：%2 個磁區
  磁區大小：%3

多出的空間中似乎含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>所需空間超過可用空間：
  需要：%1 個磁區
  可用：%2 個磁區
  磁區大小：%3

多出的空間中似乎不含資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Write cancelled.</source>
        <translation>已取消寫入。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Clearing old partition tables...</source>
        <translation>正在清除舊的分割表...</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>無法清除裝置上既有的分割表。</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+24"/>
        <location line="+21"/>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>裝置僅被部分寫入，不再包含可用的映像檔。請在使用前重新寫入映像檔。</translation>
    </message>
    <message>
        <location line="+59"/>
        <source>Fixing GPT...</source>
        <translation>正在修正 GPT...</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image truncated</source>
        <translation>映像檔遭截斷</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">寫入成功。

GPT 已與裝置 (%1) 保持一致，因此 Windows 沒有損壞的分割表需要修復。可以正常移除裝置。</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">寫入成功。

映像檔中不含 GPT，因此沒有分割表需要 Windows 修復。可以正常移除裝置。</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Write successful.</source>
        <translation>寫入成功。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Write Successful</source>
        <translation>寫入成功</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">裝置已離線並退出。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">無法自動將裝置離線。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">無法自動修正 GPT (%1)。</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">GPT 格式不正確</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Fixing the GPT failed (%1).</source>
        <translation>修正 GPT 失敗 (%1)。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>write error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">未啟用「寫入後修正 GPT」選項。</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">此映像檔會受到 Windows GPT 重寫錯誤的影響。

它在第一個分割區之前保留了空間，因此重新掃描時 Windows 會重寫主要分割表，使其指向錯誤的磁區。重寫的結果仍能通過 Windows 自身的檢查，但 Linux 會拒絕它，裝置也無法開機。</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">此映像檔不會受到 Windows GPT 重寫錯誤的影響。

由於備份 GPT 不在裝置結尾，重新掃描時 Windows 仍會重寫分割表，但以這種配置而言，重寫得到的值是正確的。無論如何，現在移除裝置都能讓它與映像檔保持逐位元組相同。</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">無法判斷此映像檔是否受到 Windows GPT 重寫錯誤的影響。請假設它會受影響：重新掃描可能導致分割表被 Linux 拒絕，裝置也無法開機。</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Remove the device now</source>
        <translation>請立即移除裝置</translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+555"/>
        <source>You do not have permission to read the selected file.</source>
        <translation>您沒有讀取所選檔案的權限。</translation>
    </message>
    <message>
        <location line="-486"/>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation>映像檔只能以未壓縮的形式讀出。請選擇不帶 .gz 或 .xz 副檔名的檔案名稱。

壓縮映像檔 (.img.gz、.img.xz) 則可以用於寫入與驗證。</translation>
    </message>
    <message>
        <location line="+39"/>
        <location line="+12"/>
        <location line="+15"/>
        <location line="+9"/>
        <location line="+21"/>
        <location line="+15"/>
        <location line="+12"/>
        <source>Read failed.</source>
        <translation>讀取失敗。</translation>
    </message>
    <message>
        <location line="+72"/>
        <location line="+20"/>
        <location line="+21"/>
        <location line="+66"/>
        <location line="+24"/>
        <source>Verify failed.</source>
        <translation>驗證失敗。</translation>
    </message>
    <message>
        <location line="-102"/>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>映像檔大於裝置：
  映像檔：至少 %1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

只能比對放得下的部分。

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>映像檔大小超過裝置：
  映像檔：%1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

由於映像檔已壓縮，無法檢查多出的空間中是否含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Verify cancelled.</source>
        <translation>已取消驗證。</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Verifying...</source>
        <translation>正在驗證…</translation>
    </message>
    <message>
        <location line="-873"/>
        <location line="+1027"/>
        <source>Partition table damaged</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-1009"/>
        <source>Repair failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The partition table could not be repaired: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+883"/>
        <source>The device could not be read at sector %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+85"/>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Image larger than device</source>
        <translation>映像檔大於裝置</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+145"/>
        <source>[Disk %1]</source>
        <translation>[磁碟 %1]</translation>
    </message>
    <message>
        <location line="-663"/>
        <location line="+554"/>
        <source>Please specify an image file to use.</source>
        <translation>請指定所要使用的映像檔。</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Scanning disks...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-1156"/>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Please select a device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Could not lock the device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Could not open the device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>This device&apos;s partition table is broken:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Partition table repaired.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Partition table is still damaged.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Partition table is valid.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+6"/>
        <location line="+6"/>
        <source>Partition table</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-11"/>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>No GPT on this device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Could not read the partition table.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+227"/>
        <source>Writing...</source>
        <translation>正在寫入…</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>映像檔大於裝置，因此其結尾未被寫入，裝置中沒有完整的映像檔。

由於壓縮映像檔不會記錄其未壓縮大小，直到裝置寫滿時才能發現這一點。</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>寫入成功。

GPT 現已與裝置 (%1) 一致，Windows 沒有需要修復的項目。可正常移除裝置。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>寫入成功。

此映像檔使用 MBR 分割表而非 GPT，因此不會受 Windows GPT 重寫缺陷影響。可正常移除裝置。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>寫入成功。

此映像檔沒有分割表，因此不會受 Windows GPT 重寫缺陷影響。可正常移除裝置。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>The device is offline and ejected.</source>
        <translation>裝置已離線並退出。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The device could NOT be taken offline.</source>
        <translation>無法將裝置離線。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The GPT could not be fixed (%1).</source>
        <translation>無法修復 GPT (%1)。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>malformed GPT</source>
        <translation>GPT 格式錯誤</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>「寫入後修正 GPT」未啟用。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>此映像檔會受影響：它在第一個分割區之前保留了空間，因此重新掃描會使主分割表指向錯誤的磁區。Windows 仍會接受該結果，Linux 不會，裝置將無法開機。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>此映像檔不受影響：重新掃描仍會重寫分割表，但對這種配置寫入的是正確的值。現在移除裝置，無論如何都與映像檔保持一致。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>無法確定此映像檔是否受影響。請按受影響處理：重新掃描可能留下 Linux 拒絕的分割表，裝置將無法開機。</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>寫入成功，但分割表存在風險。

%1 %2

%3

請立即移除裝置，不要再插入本機。直接插入目標硬體。</translation>
    </message>
    <message>
        <location line="+82"/>
        <location line="+15"/>
        <source>Read Error</source>
        <translation>讀取錯誤</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please select a source device.</source>
        <translation>請選擇來源裝置。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Confirm Overwrite</source>
        <translation>確認覆寫</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>您確定要覆寫指定的檔案嗎？</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Disk is not large enough for the specified image.</source>
        <translation>磁碟空間不足以放置指定的映像檔。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Reading...</source>
        <translation>正在讀取…</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Read Canceled.</source>
        <translation>讀取已取消。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Read Successful.</source>
        <translation>讀取成功。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>File Info</source>
        <translation>檔案資訊</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please specify a file to save data to.</source>
        <translation>請指定要儲存資料到哪一個檔案。</translation>
    </message>
    <message>
        <location line="+23"/>
        <location line="+6"/>
        <location line="+13"/>
        <location line="+103"/>
        <location line="+21"/>
        <source>Verify Error</source>
        <translation>驗證錯誤</translation>
    </message>
    <message>
        <location line="-137"/>
        <source>Please select a device to verify against.</source>
        <translation>請選擇要用於驗證的裝置。</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>映像檔大小超過裝置：
  映像檔：%1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

多出的空間中似乎含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>映像檔大小超過裝置：
  映像檔：%1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

多出的空間中似乎不含資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <location line="-24"/>
        <location line="+28"/>
        <source>Size Mismatch!</source>
        <translation>大小不符合！</translation>
    </message>
    <message>
        <location line="+107"/>
        <source>Verify Failure</source>
        <translation>驗證失敗</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Verification failed at sector: %1</source>
        <translation>驗證失敗，磁區：%1</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>映像檔大於裝置，因此只能比對放得下的部分。已比對的部分完全相符，但裝置中沒有完整的映像檔。

由於壓縮映像檔不會記錄其未壓縮大小，直到到達裝置結尾時才能發現這一點。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">驗證成功。

映像檔與裝置僅在 GPT 上有差異，而「寫入後修正 GPT」選項本來就會重寫這個部分。</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

裝置已退出。請立即移除。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

無法自動將裝置離線。</translation>
    </message>
    <message>
        <location line="-20"/>
        <location line="+46"/>
        <source>Verify Successful.</source>
        <translation>驗證成功。</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../disk.cpp" line="+64"/>
        <location line="+171"/>
        <source>File Error</source>
        <translation>檔案錯誤</translation>
    </message>
    <message>
        <location line="-170"/>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>嘗試取得檔案控制代碼失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+115"/>
        <source>Device Error</source>
        <translation>裝置錯誤</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>嘗試取得裝置控制代碼失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+168"/>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+245"/>
        <source>Lock Error</source>
        <translation>鎖定錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">嘗試鎖定該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="-400"/>
        <source>Unlock Error</source>
        <translation>解除鎖定失敗</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>嘗試解除鎖定該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Dismount Error</source>
        <translation>卸載失敗</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>嘗試卸載該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
        <location line="+8"/>
        <source>Read Error</source>
        <translation>讀取錯誤</translation>
    </message>
    <message>
        <location line="-7"/>
        <location line="+8"/>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>嘗試從該控制代碼讀取資料時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+23"/>
        <location line="+8"/>
        <location line="+10"/>
        <source>Write Error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <location line="-17"/>
        <location line="+8"/>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>嘗試向該控制代碼寫入資料時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>嘗試取得該裝置容量時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>嘗試取得該檔案大小時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Free Space Error</source>
        <translation>剩餘空間錯誤</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">無法取得磁碟 %1 的剩餘空間。
錯誤 %2：%3
將會跳過剩餘空間檢查。</translation>
    </message>
    <message>
        <location line="+199"/>
        <source>Unknown device</source>
        <translation>不明的裝置</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>無法鎖定磁碟區 %1：它仍在使用中。
請關閉所有正在使用該裝置的程式後再試一次。
錯誤 %2</translation>
    </message>
    <message>
        <location line="+214"/>
        <source>the primary GPT header size is out of range</source>
        <translation>主要 GPT 標頭的大小超出範圍</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>the primary GPT header checksum is invalid</source>
        <translation>主要 GPT 標頭的總和檢查碼無效</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT 分割區項目陣列不在標頭所指的位置</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>the GPT entry array does not fit on the device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+28"/>
        <source>a partition extends past the end of the device</source>
        <translation>有分割區超出了裝置的結尾</translation>
    </message>
    <message>
        <location line="+131"/>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>備份 GPT 已移至 LBA %1；最後可用的 LBA 現在是 %2；位於 LBA %3 的過時副本已清除</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>備份 GPT 已移至 LBA %1；最後可用的 LBA 現在是 %2</translation>
    </message>
    <message>
        <location line="+148"/>
        <source>the device geometry is not usable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>the primary GPT header is not readable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>the GPT entry array geometry is not usable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>the device is too small to hold an entry array</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>the partition entries could not be read</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>the repaired header could not be written</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../imagesource.cpp" line="+113"/>
        <source>The device reports a sector size of zero.</source>
        <translation>裝置回報的磁區大小為零。</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The image file could not be opened (error %1).</source>
        <translation>無法開啟映像檔 (錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>無法讀取映像檔的大小 (錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+65"/>
        <location line="+7"/>
        <location line="+183"/>
        <location line="+198"/>
        <source>The image file could not be read (error %1).</source>
        <translation>無法讀取映像檔 (錯誤 %1)。</translation>
    </message>
    <message>
        <location line="-411"/>
        <source>The image file could not be rewound (error %1).</source>
        <translation>無法將映像檔倒回開頭 (錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+178"/>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>無法啟動 gzip 解壓縮程式 (zlib 錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>無法啟動 xz 解壓縮程式 (lzma 錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+73"/>
        <location line="+84"/>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>映像檔在壓縮資料的中間結束。它已被截斷或損毀。</translation>
    </message>
    <message>
        <location line="-37"/>
        <source>The gzip image could not be decompressed.</source>
        <translation>無法解壓縮此 gzip 映像檔。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>此 gzip 映像檔已損毀 (zlib 錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>此 xz 映像檔已損毀 (lzma 錯誤 %1)。</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>A compressed image can only be read forwards.</source>
        <translation>壓縮映像檔只能向前讀取。</translation>
    </message>
</context>
</TS>
