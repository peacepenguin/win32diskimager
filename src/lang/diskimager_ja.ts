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
        <translation>イメージファイルのハッシュ</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Hash type to generate for image file</source>
        <translation>イメージファイルに対して生成するハッシュの種類</translation>
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
        <translation>書き込み後に GPT を修正する</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>書き込み後、バックアップ GPT をデバイスの末尾へ移動し、ヘッダーを一致するよう更新して、Windows が「修復」するものを残しません。チェックを外すと、代わりにデバイスを取り外すよう警告します。</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Show all devices</source>
        <translation>すべてのデバイスを表示</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>固定ディスクも一覧に表示します。内蔵 PCIe カードリーダーはカードをリムーバブルでないデバイスとして見せることが多く、その場合は通常表示されません。Windows が起動しているディスクが一覧に出ることはありません。</translation>
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
        <location line="+86"/>
        <location line="+140"/>
        <location line="+38"/>
        <location line="+26"/>
        <location line="+224"/>
        <location line="+74"/>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <location line="-594"/>
        <location line="+520"/>
        <location line="+188"/>
        <source>Image file cannot be located on the target device.</source>
        <translation>イメージファイルをデバイスに配置できません。</translation>
    </message>
    <message>
        <location line="-699"/>
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
        <location line="+159"/>
        <location line="+67"/>
        <source>Not enough available space!</source>
        <translation>空き容量が足りません！</translation>
    </message>
    <message>
        <location line="-263"/>
        <location line="+172"/>
        <location line="+343"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+14"/>
        <location line="+315"/>
        <location line="+270"/>
        <location line="+5"/>
        <location line="+5"/>
        <location line="+13"/>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <location line="-642"/>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation>書き込みは成功しましたが、パーティションテーブルが危険な状態です。

%1
%2

%3

他の操作を行う前に、今すぐデバイスを物理的に取り外してください。このコンピューターに再度挿入せず、目的のハードウェアに挿入してください。</translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+609"/>
        <source>The selected file does not exist.</source>
        <translation>選択したファイルは存在しません。</translation>
    </message>
    <message>
        <location line="-951"/>
        <location line="+352"/>
        <location line="+330"/>
        <location line="+279"/>
        <source>The specified file contains no data.</source>
        <translation>指定されたファイルにはデータが含まれていません。</translation>
    </message>
    <message>
        <location line="-605"/>
        <location line="+201"/>
        <location line="+408"/>
        <source>Done.</source>
        <translation>完了しました。</translation>
    </message>
    <message>
        <location line="-404"/>
        <location line="+2"/>
        <location line="+381"/>
        <location line="+25"/>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <location line="-609"/>
        <source>Write Successful.</source>
        <translation>書き込み成功。</translation>
    </message>
    <message>
        <location line="-742"/>
        <location line="+2"/>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation>ディスクイメージ (*.img *.IMG *.img.gz *.img.xz)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation>圧縮ディスクイメージ (*.img.gz *.img.xz *.gz *.xz)</translation>
    </message>
    <message>
        <location line="+203"/>
        <source>Error</source>
        <translation>エラー</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation>チェックサムを生成するためにファイルを開けませんでした:
%1</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Please select a target device.</source>
        <translation>書き込み先のデバイスを選択してください。</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Writing to a physical device can corrupt the device.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>物理デバイスへの書き込みは、デバイスを破損させる可能性があります。
(書き込み先デバイス: %1)
本当に続行してもよろしいですか？</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Device has mounted volumes</source>
        <translation>デバイスにマウント済みのボリュームがあります</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 は Windows で %2 としてマウントされています。

このデバイス上のすべてのパーティションのデータはすべて破棄され、復元できません。

%2 が残しておきたいドライブでないことを確認してください。

それでもこのデバイスに書き込みますか？</translation>
    </message>
    <message>
        <location line="+26"/>
        <location line="+14"/>
        <location line="+19"/>
        <location line="+16"/>
        <location line="+23"/>
        <location line="+25"/>
        <location line="+95"/>
        <location line="+38"/>
        <location line="+27"/>
        <source>Write failed.</source>
        <translation>書き込みに失敗しました。</translation>
    </message>
    <message>
        <location line="-233"/>
        <location line="+683"/>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <location line="-682"/>
        <location line="+683"/>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>デバイスがサイズ 0 を報告しています。カードリーダーの場合、カードが取り外された可能性があります。</translation>
    </message>
    <message>
        <location line="-625"/>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>イメージがデバイスより大きいです:
  イメージ: 少なくとも %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

イメージの末尾は書き込まれないため、デバイスには完全なイメージが入りません。

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>使用可能な容量を超える容量が必要です:
  必要: %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

イメージが圧縮されているため、超過部分にデータがあるか確認できませんでした

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>使用可能な容量を超える容量が必要です:
  必要: %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

超過部分にはデータが含まれているようです

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>使用可能な容量を超える容量が必要です:
  必要: %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

超過部分にデータは含まれていないようです

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Write cancelled.</source>
        <translation>書き込みを中止しました。</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Clearing old partition tables...</source>
        <translation>古いパーティションテーブルを消去しています...</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>デバイス上の既存のパーティションテーブルを消去できませんでした。</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+38"/>
        <location line="+25"/>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>デバイスへの書き込みが途中で終わったため、使用できるイメージが含まれていません。使用する前にイメージを書き込み直してください。</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>Fixing GPT...</source>
        <translation>GPT を修正しています...</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Image truncated</source>
        <translation>イメージが途中で切れています</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation>書き込みに成功しました。

GPT をデバイス (%1) と整合するようにしたため、Windows が修復すべき壊れたテーブルはありません。デバイスは通常どおり取り外せます。</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation>書き込みに成功しました。

イメージに GPT が含まれていないため、Windows が修復するパーティションテーブルはありません。デバイスは通常どおり取り外せます。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Write successful.</source>
        <translation>書き込みに成功しました。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Write Successful</source>
        <translation>書き込み成功</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The device has been taken offline and ejected.</source>
        <translation>デバイスをオフラインにして取り出しました。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The device could NOT be taken offline automatically.</source>
        <translation>デバイスを自動的にオフラインにできませんでした。</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation>GPT を自動的に修正できませんでした (%1)。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>the GPT is malformed</source>
        <translation>GPT の形式が不正です</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Fixing the GPT failed (%1).</source>
        <translation>GPT の修正に失敗しました (%1)。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>write error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation>「書き込み後に GPT を修正する」オプションが有効になっていません。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation>このイメージは Windows の GPT 書き換え不具合の影響を受けます。

最初のパーティションの前に領域を確保しているため、再スキャン時に Windows がプライマリパーティションテーブルを書き換え、誤ったセクタを指すようになります。その結果は Windows 自身の検査は通りますが、Linux では拒否され、デバイスは起動しません。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation>このイメージは Windows の GPT 書き換え不具合の影響を受けません。

バックアップ GPT がデバイスの末尾にないため、再スキャン時に Windows はテーブルを書き換えますが、この配置では書き換え後も正しい値になります。いずれにせよ、今デバイスを取り外せばイメージとバイト単位で同一のまま保てます。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation>このイメージが Windows の GPT 書き換え不具合の影響を受けるかどうかを判定できませんでした。影響を受けるものとして扱ってください。再スキャンにより、Linux が拒否するパーティションテーブルが残り、デバイスが起動しなくなる可能性があります。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Remove the device now</source>
        <translation>今すぐデバイスを取り外してください</translation>
    </message>
    <message>
        <location line="+16"/>
        <location line="+609"/>
        <source>You do not have permission to read the selected file.</source>
        <translation>選択したファイルを読み取る権限がありません。</translation>
    </message>
    <message>
        <location line="-559"/>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation>イメージは非圧縮でのみ読み出せます。.gz や .xz の拡張子を付けないファイル名を選んでください。

圧縮イメージ (.img.gz、.img.xz) は書き込みと検証には使用できます。</translation>
    </message>
    <message>
        <location line="+41"/>
        <location line="+11"/>
        <location line="+13"/>
        <location line="+26"/>
        <location line="+29"/>
        <location line="+16"/>
        <source>Read failed.</source>
        <translation>読み込みに失敗しました。</translation>
    </message>
    <message>
        <location line="+83"/>
        <location line="+15"/>
        <location line="+19"/>
        <location line="+16"/>
        <location line="+22"/>
        <location line="+25"/>
        <location line="+107"/>
        <location line="+25"/>
        <source>Verify failed.</source>
        <translation>検証に失敗しました。</translation>
    </message>
    <message>
        <location line="-146"/>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>イメージがデバイスより大きいです:
  イメージ: 少なくとも %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

収まる部分のみ比較できます。

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>イメージのサイズがデバイスより大きいです:
  イメージ: %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

イメージが圧縮されているため、超過部分にデータがあるか確認できませんでした

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Verify cancelled.</source>
        <translation>検証を中止しました。</translation>
    </message>
    <message>
        <location line="+145"/>
        <source>Image larger than device</source>
        <translation>イメージがデバイスより大きいです</translation>
    </message>
    <message>
        <location line="+107"/>
        <source>[Disk %1]</source>
        <translation>[ディスク %1]</translation>
    </message>
    <message>
        <location line="-667"/>
        <location line="+608"/>
        <source>Please specify an image file to use.</source>
        <translation>使用するイメージファイルを指定してください。</translation>
    </message>
    <message>
        <location line="-712"/>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>イメージがデバイスより大きいため、末尾は書き込まれず、デバイスには完全なイメージが入っていません。

圧縮イメージは非圧縮サイズを記録しないため、デバイスがいっぱいになるまでこれを検出できませんでした。</translation>
    </message>
    <message>
        <location line="+134"/>
        <location line="+15"/>
        <source>Read Error</source>
        <translation>読み込みエラー</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please select a source device.</source>
        <translation>読み込み元のデバイスを選択してください。</translation>
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
        <location line="+62"/>
        <source>Disk is not large enough for the specified image.</source>
        <translation>指定されたイメージに対し ディスク容量が十分ではありません。</translation>
    </message>
    <message>
        <location line="+82"/>
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
        <location line="+61"/>
        <location line="+154"/>
        <source>Verify Error</source>
        <translation>照合エラー</translation>
    </message>
    <message>
        <location line="-215"/>
        <source>Please select a device to verify against.</source>
        <translation>照合するデバイスを選択してください。</translation>
    </message>
    <message>
        <location line="+162"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>イメージのサイズがデバイスより大きいです:
  イメージ: %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

超過部分にはデータが含まれているようです

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>イメージのサイズがデバイスより大きいです:
  イメージ: %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

超過部分にデータは含まれていないようです

それでも続行しますか？</translation>
    </message>
    <message>
        <location line="-59"/>
        <location line="+63"/>
        <source>Size Mismatch!</source>
        <translation>サイズが合いません！</translation>
    </message>
    <message>
        <location line="+69"/>
        <location line="+38"/>
        <source>Verify Failure</source>
        <translation>照合失敗</translation>
    </message>
    <message>
        <location line="-38"/>
        <location line="+39"/>
        <source>Verification failed at sector: %1</source>
        <translation>セクター：%1で照合に失敗しました</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>イメージがデバイスより大きいため、収まる部分のみ比較できました。比較した範囲はすべて一致しましたが、デバイスには完全なイメージが入っていません。

圧縮イメージは非圧縮サイズを記録しないため、デバイスの末尾に達するまでこれを検出できませんでした。</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation>検証に成功しました。

イメージとデバイスの違いは GPT のみで、これは「書き込み後に GPT を修正する」オプションが意図的に書き換えたものです。</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

デバイスを取り出しました。今すぐ取り外してください。</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

デバイスを自動的にオフラインにできませんでした。</translation>
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
        <translation>不明なデバイス</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>ボリューム %1 をロックできませんでした: まだ使用中です。
デバイスを使用しているプログラムをすべて閉じて、もう一度お試しください。
エラー %2</translation>
    </message>
    <message>
        <location line="+179"/>
        <source>the primary GPT header size is out of range</source>
        <translation>プライマリ GPT ヘッダーのサイズが範囲外です</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>the primary GPT header checksum is invalid</source>
        <translation>プライマリ GPT ヘッダーのチェックサムが不正です</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT パーティションエントリ配列がヘッダーの示す位置にありません</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>a partition extends past the end of the device</source>
        <translation>パーティションがデバイスの末尾を超えて広がっています</translation>
    </message>
    <message>
        <location line="+68"/>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>バックアップ GPT を LBA %1 に移動しました。最終使用可能 LBA は %2 になりました</translation>
    </message>
    <message>
        <location filename="../imagesource.cpp" line="+104"/>
        <source>The device reports a sector size of zero.</source>
        <translation>デバイスがセクタサイズ 0 を報告しています。</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The image file could not be opened (error %1).</source>
        <translation>イメージファイルを開けませんでした (エラー %1)。</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>イメージファイルのサイズを読み取れませんでした (エラー %1)。</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+71"/>
        <location line="+7"/>
        <location line="+190"/>
        <location line="+24"/>
        <location line="+157"/>
        <source>The image file could not be read (error %1).</source>
        <translation>イメージファイルを読み取れませんでした (エラー %1)。</translation>
    </message>
    <message>
        <location line="-403"/>
        <source>The image file could not be rewound (error %1).</source>
        <translation>イメージファイルを先頭に戻せませんでした (エラー %1)。</translation>
    </message>
    <message>
        <location line="+181"/>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>gzip 展開処理を開始できませんでした (zlib エラー %1)。</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>xz 展開処理を開始できませんでした (lzma エラー %1)。</translation>
    </message>
    <message>
        <location line="+62"/>
        <location line="+84"/>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>イメージファイルが圧縮データの途中で終わっています。切り詰められているか破損しています。</translation>
    </message>
    <message>
        <location line="-37"/>
        <source>The gzip image could not be decompressed.</source>
        <translation>gzip イメージを展開できませんでした。</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>gzip イメージが破損しています (zlib エラー %1)。</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>xz イメージが破損しています (lzma エラー %1)。</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>A compressed image can only be read forwards.</source>
        <translation>圧縮イメージは前方向にしか読み取れません。</translation>
    </message>
</context>
</TS>
