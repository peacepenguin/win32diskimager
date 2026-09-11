<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja_JP" sourcelanguage="en_US">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="+29"/>
        <source>Win32 Disk Imager</source>
        <translation>Win32 Disk Imager</translation>
    </message>
    <message>
        <location line="+25"/>
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
        <location line="+36"/>
        <source>Hash</source>
        <translation>ハッシュ値</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Select hash type for verification</source>
        <translation>照合の方式</translation>
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
        <source>Read Only Allocated Partitions</source>
        <translation>定義されたパーテーションの最後までしか読み取らない</translation>
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
        <location filename="../mainwindow.cpp" line="+185"/>
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
        <location line="+34"/>
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
        <location line="+165"/>
        <location line="+189"/>
        <location line="+101"/>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <location line="-455"/>
        <location line="+354"/>
        <location line="+216"/>
        <source>Image file cannot be located on the target device.</source>
        <translation>イメージファイルをデバイスに配置できません。</translation>
    </message>
    <message>
        <location line="-560"/>
        <source>Confirm overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Writing to a physical device can corrupt the device.
(Target Device: %1 &quot;%2&quot;)
Are you sure you want to continue?</source>
        <translation>物理デバイスに書き込むとデバイスが破損する可能性があります。
（ターゲットデバイス：%1 &quot;%2&quot;）
続行してもよろしいですか？</translation>
    </message>
    <message>
        <location line="-295"/>
        <source>Waiting for a task.</source>
        <translation>タスクを待っています。</translation>
    </message>
    <message>
        <location line="+26"/>
        <location line="+2"/>
        <source>Disk Images (*.img *.IMG)</source>
        <translation>ディスクイメージ (*.img *.IMG)</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>終了すると、照合がキャンセルされます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <location line="+112"/>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>照合をキャンセルします。
本当にキャンセルしてもよろしいですか？</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>Not enough available space!</source>
        <translation>空き容量が足りません！</translation>
    </message>
    <message>
        <location line="+169"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+14"/>
        <location line="+526"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+13"/>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <location line="-573"/>
        <location line="+550"/>
        <source>The selected file does not exist.</source>
        <translation>選択したファイルは存在しません。</translation>
    </message>
    <message>
        <location line="-545"/>
        <location line="+550"/>
        <source>You do not have permision to read the selected file.</source>
        <translation>選択したファイルを読み込むための権限がありません。</translation>
    </message>
    <message>
        <location line="-545"/>
        <location line="+550"/>
        <source>The specified file contains no data.</source>
        <translation>指定されたファイルにはデータが含まれていません。</translation>
    </message>
    <message>
        <location line="-546"/>
        <location line="+213"/>
        <location line="+337"/>
        <source>Done.</source>
        <translation>完了しました。</translation>
    </message>
    <message>
        <location line="-333"/>
        <location line="+2"/>
        <location line="+310"/>
        <location line="+25"/>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <location line="-550"/>
        <source>Write Successful.</source>
        <translation>書き込み成功。</translation>
    </message>
    <message>
        <location line="-164"/>
        <source>Clearing old partition tables...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+72"/>
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
        <location line="+1"/>
        <source>Remove the device now</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Write successful, but the partition table is at risk.

%1
%2

Physically remove the device NOW, before doing anything else.

Do not re-insert it into this computer. If Windows re-reads a partition table whose backup GPT is not at the end of the device (which is normal when the image is smaller than the card), it will silently rewrite it. The result passes Windows&apos; own checks but is rejected by Linux, and the device will not boot.

Insert it into the target hardware instead.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <location line="+549"/>
        <source>Please specify an image file to use.</source>
        <translation>使用するイメージファイルを指定してください。</translation>
    </message>
    <message>
        <location line="-520"/>
        <source>Confirm Overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>ファイルを上書きしてもよろしいですか？</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Disk is not large enough for the specified image.</source>
        <translation>指定されたイメージに対し ディスク容量が十分ではありません。</translation>
    </message>
    <message>
        <location line="+83"/>
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
        <source>Verify Error</source>
        <translation>照合エラー</translation>
    </message>
    <message>
        <location line="+129"/>
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
        <translation>サイズが合いません！</translation>
    </message>
    <message>
        <location line="+54"/>
        <location line="+40"/>
        <source>Verify Failure</source>
        <translation>照合失敗</translation>
    </message>
    <message>
        <location line="-40"/>
        <location line="+41"/>
        <source>Verification failed at sector: %1</source>
        <translation>セクター：%1で照合に失敗しました</translation>
    </message>
    <message>
        <location line="+43"/>
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
        <location filename="../disk.cpp" line="+43"/>
        <location line="+215"/>
        <location line="+90"/>
        <location line="+20"/>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <location line="-325"/>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>ファイルのハンドルを取得しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+43"/>
        <location line="+147"/>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <location line="-146"/>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>デバイス上でハンドルを取得しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="-28"/>
        <location line="+46"/>
        <location line="+356"/>
        <source>Volume Error</source>
        <translation>ボリュームエラー</translation>
    </message>
    <message>
        <location line="-401"/>
        <source>An error occurred when attempting to get information on volume.
Error %1: %2</source>
        <translation>ボリュームに関する情報を取得中にエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>An error occurred when attempting to get a handle on the volume.
Error %1: %2</source>
        <translation>ボリューム上でハンドルを取得しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+17"/>
        <location line="+436"/>
        <source>Lock Error</source>
        <translation>ロックエラー</translation>
    </message>
    <message>
        <location line="-435"/>
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
        <location line="+66"/>
        <source>An error occurred while getting the device number.
This usually means something is currently accessing the device;please close all applications and try again.

Error %1: %2</source>
        <translation>デバイス番号の取得中にエラーが発生しました。
これは、通常、デバイスに現在アクセスしていることを意味します。すべてのアプリケーションを閉じて、もう一度やり直してください。

エラー%1：%2</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>An error occurred while querying the properties.
This usually means something is currently accessing the device; please close all applications and try again.

Error %1: %2</source>
        <translation>プロパティの確認中にエラーが発生しました。
これは通常、何かが現在デバイスにアクセスしていることを意味します。 すべてのアプリケーションを閉じてもう一度やり直してください。

エラー%1：%2</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>An error occurred when attempting to get a handle on %3.
Error %1: %2</source>
        <translation>%3のハンドルを取得しようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <location line="+98"/>
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
