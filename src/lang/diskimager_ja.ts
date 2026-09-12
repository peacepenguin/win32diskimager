<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja_JP" sourcelanguage="en_US">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="+34"/>
        <source>Win32 Disk Imager</source>
        <translation>Win32 Disk Imager</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Image File</source>
        <translation>イメージファイル</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Image File Hash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Hash type to generate for image file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>None</source>
        <translation>無し</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Generate selected hash on file</source>
        <translation>ファイルから作成</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Generate</source>
        <translation>作成</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Copy hash to clipboard</source>
        <translation>ハッシュをクリップボードからコピー</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Copy</source>
        <translation>コピー</translation>
    </message>
    <message>
        <location line="+55"/>
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
        <translation>進捗の状況</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Cancel current process.</source>
        <translation>作業をキャンセルしました。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>デバイスからファイルを作成</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Read</source>
        <translation>読込み</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>ファイルからデバイスに書込み</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Write</source>
        <translation>書込み</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Verify the image file with the selected drive</source>
        <translation>イメージとドライブの照合</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Verify Only</source>
        <translation>照合のみ</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Exit Win32 Disk Imager</source>
        <translation>Win32 Disk Imagerを終了</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Exit</source>
        <translation>終了</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="+217"/>
        <location line="+10"/>
        <location line="+10"/>
        <source>Exit?</source>
        <translation>終了しますか？</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>終了すると、破損したイメージファイルが作成されます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>終了すると、破損したディスクが作成されます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Select a disk image</source>
        <translation>イメージを選択</translation>
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
        <translation>キャンセル？</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>いまキャンセルすると、宛先が破損します。
本当にキャンセルしてもよろしいです？</translation>
    </message>
    <message>
        <location line="+30"/>
        <location line="+6"/>
        <location line="+80"/>
        <location line="+132"/>
        <location line="+34"/>
        <location line="+238"/>
        <location line="+68"/>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <location line="-558"/>
        <location line="+490"/>
        <location line="+176"/>
        <source>Image file cannot be located on the target device.</source>
        <translation>イメージファイルをデバイスに配置できません。</translation>
    </message>
    <message>
        <location line="-657"/>
        <source>Confirm overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <location line="-316"/>
        <source>Waiting for a task.</source>
        <translation>タスクを待っています。</translation>
    </message>
    <message>
        <location line="+166"/>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>終了すると、照合がキャンセルされます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <location line="+120"/>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>照合をキャンセルします。
本当にキャンセルしてもよろしいですか？</translation>
    </message>
    <message>
        <location line="+149"/>
        <location line="+65"/>
        <source>Not enough available space!</source>
        <translation>空き容量が足りません！</translation>
    </message>
    <message>
        <location line="-251"/>
        <location line="+164"/>
        <location line="+321"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+14"/>
        <location line="+295"/>
        <location line="+260"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+13"/>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <location line="-612"/>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+579"/>
        <source>The selected file does not exist.</source>
        <translation>選択したファイルは存在しません。</translation>
    </message>
    <message>
        <location line="-899"/>
        <location line="+330"/>
        <location line="+310"/>
        <location line="+269"/>
        <source>The specified file contains no data.</source>
        <translation>指定されたファイルにはデータが含まれていません。</translation>
    </message>
    <message>
        <location line="-575"/>
        <location line="+189"/>
        <location line="+390"/>
        <source>Done.</source>
        <translation>完了しました。</translation>
    </message>
    <message>
        <location line="-386"/>
        <location line="+2"/>
        <location line="+363"/>
        <location line="+25"/>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <location line="-579"/>
        <source>Write Successful.</source>
        <translation>書き込み成功。</translation>
    </message>
    <message>
        <location line="-712"/>
        <location line="+2"/>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+203"/>
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
        <location line="+13"/>
        <source>Device has mounted volumes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+46"/>
        <location line="+641"/>
        <source>Device Error</source>
        <translation type="unfinished">デバイスエラー</translation>
    </message>
    <message>
        <location line="-640"/>
        <location line="+641"/>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-589"/>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+59"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Clearing old partition tables...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+110"/>
        <source>Fixing GPT...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image truncated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
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
        <location line="+16"/>
        <location line="+579"/>
        <source>You do not have permission to read the selected file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-529"/>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+280"/>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+164"/>
        <source>Image larger than device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+107"/>
        <source>[Disk %1]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-637"/>
        <location line="+578"/>
        <source>Please specify an image file to use.</source>
        <translation>使用するイメージファイルを指定してください。</translation>
    </message>
    <message>
        <location line="-682"/>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+134"/>
        <location line="+15"/>
        <source>Read Error</source>
        <translation type="unfinished">読込みエラー</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please select a source device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Confirm Overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>ファイルを上書きしてもよろしいですか？</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>Disk is not large enough for the specified image.</source>
        <translation>指定されたイメージに対し ディスク容量が十分ではありません。</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>Read Canceled.</source>
        <translation>読み込みがキャンセルされました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Read Successful.</source>
        <translation>読み込み成功。</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>File Info</source>
        <translation>ファイル情報</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please specify a file to save data to.</source>
        <translation>データを保存するファイルを指定してください。</translation>
    </message>
    <message>
        <location line="+23"/>
        <location line="+6"/>
        <location line="+55"/>
        <location line="+146"/>
        <source>Verify Error</source>
        <translation>照合エラー</translation>
    </message>
    <message>
        <location line="-201"/>
        <source>Please select a device to verify against.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+150"/>
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
        <location line="-57"/>
        <location line="+61"/>
        <source>Size Mismatch!</source>
        <translation>サイズが合いません！</translation>
    </message>
    <message>
        <location line="+65"/>
        <location line="+36"/>
        <source>Verify Failure</source>
        <translation>照合失敗</translation>
    </message>
    <message>
        <location line="-36"/>
        <location line="+37"/>
        <source>Verification failed at sector: %1</source>
        <translation>セクター：%1で照合に失敗しました</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
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
        <translation>照合に成功しました。</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../disk.cpp" line="+48"/>
        <location line="+179"/>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <location line="-179"/>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>ファイルのハンドルを取得しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+26"/>
        <location line="+128"/>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <location line="-127"/>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>デバイス上でハンドルを取得しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
        <location line="+370"/>
        <source>Lock Error</source>
        <translation>ロックエラー</translation>
    </message>
    <message>
        <location line="-369"/>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation>ボリュームをロックしようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Unlock Error</source>
        <translation>アンロックエラー</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>ボリュームのロックを解除しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Dismount Error</source>
        <translation>マウント解除エラー</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>ボリュームをマウント解除しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Read Error</source>
        <translation>読込みエラー</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>ハンドルからデータを読み取ろうとしているときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Write Error</source>
        <translation>書込みエラー</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>データの書き込み中にエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>デバイスのジオメトリを取得中にエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>ファイルサイズを取得中にエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Free Space Error</source>
        <translation>空き領域エラー</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>ドライブ%1の空きスペースを取得できませんでした。
エラー%2：%3
空き容量の確認はスキップされます。</translation>
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
    <message>
        <location filename="../imagesource.cpp" line="+104"/>
        <source>The device reports a sector size of zero.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The image file could not be opened (error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The size of the image file could not be read (error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+71"/>
        <location line="+7"/>
        <location line="+190"/>
        <location line="+24"/>
        <location line="+157"/>
        <source>The image file could not be read (error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-403"/>
        <source>The image file could not be rewound (error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+181"/>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+62"/>
        <location line="+84"/>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-37"/>
        <source>The gzip image could not be decompressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>A compressed image can only be read forwards.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
