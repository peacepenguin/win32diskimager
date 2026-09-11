<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="+29"/>
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
        <location line="+14"/>
        <source>Device</source>
        <translation>裝置</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Hash</source>
        <translation>雜湊值</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Select hash type for verification</source>
        <translation>選取要用來驗證的雜湊類型</translation>
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
        <location line="+6"/>
        <source>Generate</source>
        <translation>生成</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Copy hash to clipboard</source>
        <translation>複製雜湊值到剪貼簿中</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Copy</source>
        <translation>複製</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Read Only Allocated Partitions</source>
        <translation>唯讀的未分配分割區</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Fix GPT after write</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Show all devices</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Progress</source>
        <translation>進度</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Cancel current process.</source>
        <translation>取消目前的進度。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>從「裝置」讀取資料到「映像檔」中</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Read</source>
        <translation>讀取</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>從「映像檔」寫入資料到「裝置」中</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Write</source>
        <translation>寫入資料到「裝置」中</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Verify the image file with the selected drive</source>
        <translation>驗證選定磁碟的映像檔</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Verify Only</source>
        <translation>僅驗證</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Exit Win32 Disk Imager</source>
        <translation>離開 Win32 磁蝶映像檔工具</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Exit</source>
        <translation>離開</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="+195"/>
        <location line="+10"/>
        <location line="+10"/>
        <source>Exit?</source>
        <translation>離開？</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>現在離開將會導致映像檔損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>現在離開將會導致磁碟損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Select a disk image</source>
        <translation>選擇一個磁碟映像檔</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Generating...</source>
        <translation>生成中...</translation>
    </message>
    <message>
        <location line="+42"/>
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
        <location line="+143"/>
        <location line="+212"/>
        <location line="+85"/>
        <source>Write Error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <location line="-446"/>
        <location line="+361"/>
        <location line="+192"/>
        <source>Image file cannot be located on the target device.</source>
        <translation>在目標裝置上找不到映像檔。</translation>
    </message>
    <message>
        <location line="-544"/>
        <source>Confirm overwrite</source>
        <translation>確認覆寫</translation>
    </message>
    <message>
        <location line="-313"/>
        <source>Waiting for a task.</source>
        <translation>等待工作。</translation>
    </message>
    <message>
        <location line="+39"/>
        <location line="+2"/>
        <source>Disk Images (*.img *.IMG)</source>
        <translation>映像檔 (*.img *.IMG)</translation>
    </message>
    <message>
        <location line="+122"/>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>現在結束將會取消驗證映像檔。
您確定想要結束嗎？</translation>
    </message>
    <message>
        <location line="+120"/>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>取消驗證。
您確定您想要取消嗎？</translation>
    </message>
    <message>
        <location line="+143"/>
        <source>Not enough available space!</source>
        <translation>沒有足夠的可用空間！</translation>
    </message>
    <message>
        <location line="-180"/>
        <location line="+372"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+14"/>
        <location line="+473"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+13"/>
        <source>File Error</source>
        <translation>檔案錯誤</translation>
    </message>
    <message>
        <location line="-530"/>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+497"/>
        <source>The selected file does not exist.</source>
        <translation>選取的檔案不存在。</translation>
    </message>
    <message>
        <location line="-492"/>
        <location line="+497"/>
        <source>You do not have permision to read the selected file.</source>
        <translation>您沒有讀取已選取檔案的權限。</translation>
    </message>
    <message>
        <location line="-492"/>
        <location line="+497"/>
        <source>The specified file contains no data.</source>
        <translation>指定的檔案不包含資料。</translation>
    </message>
    <message>
        <location line="-493"/>
        <location line="+189"/>
        <location line="+308"/>
        <source>Done.</source>
        <translation>完成。</translation>
    </message>
    <message>
        <location line="-304"/>
        <location line="+2"/>
        <location line="+281"/>
        <location line="+25"/>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <location line="-497"/>
        <source>Write Successful.</source>
        <translation>寫入成功。</translation>
    </message>
    <message>
        <location line="-393"/>
        <source>Error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Please select a target device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Writing to a physical device can corrupt the device.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+136"/>
        <source>Clearing old partition tables...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Fixing GPT...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Write successful.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Write Successful</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The device has been taken offline and ejected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>the GPT is malformed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Fixing the GPT failed (%1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>write error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Remove the device now</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+590"/>
        <source>[Disk %1]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-555"/>
        <location line="+496"/>
        <source>Please specify an image file to use.</source>
        <translation>請指定所要使用的映像檔。</translation>
    </message>
    <message>
        <location line="-467"/>
        <source>Read Error</source>
        <translation type="unfinished">讀取錯誤</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please select a source device.</source>
        <translation type="unfinished"></translation>
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
        <location line="+73"/>
        <source>Disk is not large enough for the specified image.</source>
        <translation>磁碟空間不足以放置指定的映像檔。</translation>
    </message>
    <message>
        <location line="+75"/>
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
        <source>Verify Error</source>
        <translation>驗證錯誤</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please select a device to verify against.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Size Mismatch!</source>
        <translation>大小不符合！</translation>
    </message>
    <message>
        <location line="+50"/>
        <location line="+38"/>
        <source>Verify Failure</source>
        <translation>驗證失敗</translation>
    </message>
    <message>
        <location line="-38"/>
        <location line="+39"/>
        <source>Verification failed at sector: %1</source>
        <translation>驗證失敗，磁區：%1</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>

The device has been ejected. Remove it now.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-3"/>
        <location line="+29"/>
        <source>Verify Successful.</source>
        <translation>驗證成功。</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../disk.cpp" line="+43"/>
        <location line="+179"/>
        <source>File Error</source>
        <translation>檔案錯誤</translation>
    </message>
    <message>
        <location line="-179"/>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>嘗試取得檔案控制代碼失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+26"/>
        <location line="+128"/>
        <source>Device Error</source>
        <translation>裝置錯誤</translation>
    </message>
    <message>
        <location line="-127"/>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>嘗試取得裝置控制代碼失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
        <location line="+370"/>
        <source>Lock Error</source>
        <translation>鎖定錯誤</translation>
    </message>
    <message>
        <location line="-369"/>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation>嘗試鎖定該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
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
        <location line="+17"/>
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
        <location line="+27"/>
        <source>Read Error</source>
        <translation>讀取錯誤</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>嘗試從該控制代碼讀取資料時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Write Error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>嘗試向該控制代碼寫入資料時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>嘗試取得該裝置容量時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>嘗試取得該檔案大小時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Free Space Error</source>
        <translation>剩餘空間錯誤</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>無法取得磁碟 %1 的剩餘空間。
錯誤 %2：%3
將會跳過剩餘空間檢查。</translation>
    </message>
    <message>
        <location line="+158"/>
        <source>Unknown device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+179"/>
        <source>the primary GPT header size is out of range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>the primary GPT header checksum is invalid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>the GPT partition entry array is not where the header says</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+44"/>
        <source>a partition extends past the end of the device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+68"/>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
