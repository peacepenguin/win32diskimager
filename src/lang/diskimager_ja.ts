<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja_JP" sourcelanguage="en_US">
<context>
    <name>MainWindow</name>
    <message>
        <source>Win32 Disk Imager</source>
        <translation type="vanished">Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Image File</source>
        <translation>イメージファイル</translation>
    </message>
    <message>
        <source>...</source>
        <translation>…</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>照合</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation>読み取り時にイメージを縮小</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation>デバイスの MBR または GPT を読み取り、実際のパーティションに合わせてイメージを縮小します。バックアップ GPT は使用済み領域の末尾へ移動します。</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation>.img.gz として読み取る</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation>デバイスから読み取ったイメージを gz で圧縮します</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation>.img.xz として読み取る</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation>デバイスから読み取ったイメージを xz で圧縮します</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation>読み取るパーティションを選択</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation>読み取りの前に、デバイスのパーティションを一覧表示し、含めるものを選択できます。除外したものはイメージから取り除かれます。これは未パーティション領域と同様で、「読み取り時にイメージを縮小」がチェックされているかどうかにかかわらず、常にイメージを縮小します。</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>WinDiskImagerを終了</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Win Disk Imagerを終了</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation>GPT を確認</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>選択中のデバイスの GPT が壊れていないかを確認し、壊れていれば修復を提案します。</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>イメージファイルのハッシュ</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>イメージファイルに対して生成するハッシュの種類</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無し</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>選択したハッシュをファイルから生成</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>作成</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>ハッシュをクリップボードにコピー</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>コピー</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>書き込み後に GPT を修正する</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>書き込み後、バックアップ GPT をデバイスの末尾へ移動し、ヘッダーを一致するよう更新して、Windows が「修復」するものを残しません。チェックを外すと、代わりにデバイスを取り外すよう警告します。</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>すべてのデバイスを表示</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>固定ディスクも一覧に表示します。内蔵 PCIe カードリーダーはカードをリムーバブルでないデバイスとして見せることが多く、その場合は通常表示されません。Windows が起動しているディスクが一覧に出ることはありません。</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>進捗状況</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>現在の処理をキャンセルします。</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>デバイスからファイルを作成</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>読込み</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>ファイルからデバイスに書込み</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>書込み</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>デバイスとファイルを照合</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">イメージとドライブの照合</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">照合のみ</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Win32 Disk Imagerを終了</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>終了</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>終了しますか？</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>終了すると、破損したイメージファイルが作成されます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>終了すると、破損したディスクが作成されます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>イメージを選択</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>生成中…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>キャンセルしますか？</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>いまキャンセルすると、宛先が破損します。
本当にキャンセルしてもよろしいですか？</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>イメージファイルをデバイスに配置できません。</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">タスクを待っています。</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>終了すると、照合がキャンセルされます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>照合をキャンセルします。
本当にキャンセルしてもよろしいですか？</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>空き容量が足りません！</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">書き込みは成功しましたが、パーティションテーブルが危険な状態です。

%1
%2

%3

他の操作を行う前に、今すぐデバイスを物理的に取り外してください。このコンピューターに再度挿入せず、目的のハードウェアに挿入してください。</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>選択したファイルは存在しません。</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>指定されたファイルにはデータが含まれていません。</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>完了しました。</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>完了</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>書き込み成功。</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation>ディスクイメージ (*.img *.IMG *.img.gz *.img.xz)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation>圧縮ディスクイメージ (*.img.gz *.img.xz *.gz *.xz)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>エラー</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation>チェックサムを生成するためにファイルを開けませんでした:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>書き込み先のデバイスを選択してください。</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>このデバイス上のすべてのファイルとデータが削除されます。
(書き込み先デバイス: %1)
本当に続行してもよろしいですか？</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>デバイスにマウント済みのボリュームがあります</translation>
    </message>
    <message>
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
        <source>Write failed.</source>
        <translation>書き込みに失敗しました。</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>デバイスがサイズ 0 を報告しています。カードリーダーの場合、カードが取り外された可能性があります。</translation>
    </message>
    <message>
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
        <source>Write cancelled.</source>
        <translation>書き込みを中止しました。</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>古いパーティションテーブルを消去しています…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>デバイス上の既存のパーティションテーブルを消去できませんでした。</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>デバイスへの書き込みが途中で終わったため、使用できるイメージが含まれていません。使用する前にイメージを書き込み直してください。</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>GPT を修正しています…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>イメージが途中で切れています</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">書き込みに成功しました。

GPT をデバイス (%1) と整合するようにしたため、Windows が修復すべき壊れたテーブルはありません。デバイスは通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">書き込みに成功しました。

イメージに GPT が含まれていないため、Windows が修復するパーティションテーブルはありません。デバイスは通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>書き込みに成功しました。</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>書き込み成功</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">デバイスをオフラインにして取り出しました。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">デバイスを自動的にオフラインにできませんでした。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">GPT を自動的に修正できませんでした (%1)。</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">GPT の形式が不正です</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>GPT の修正に失敗しました (%1)。</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">「書き込み後に GPT を修正する」オプションが有効になっていません。</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">このイメージは Windows の GPT 書き換え不具合の影響を受けます。

最初のパーティションの前に領域を確保しているため、再スキャン時に Windows がプライマリパーティションテーブルを書き換え、誤ったセクタを指すようになります。その結果は Windows 自身の検査は通りますが、Linux では拒否され、デバイスは起動しません。</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">このイメージは Windows の GPT 書き換え不具合の影響を受けません。

バックアップ GPT がデバイスの末尾にないため、再スキャン時に Windows はテーブルを書き換えますが、この配置では書き換え後も正しい値になります。いずれにせよ、今デバイスを取り外せばイメージとバイト単位で同一のまま保てます。</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">このイメージが Windows の GPT 書き換え不具合の影響を受けるかどうかを判定できませんでした。影響を受けるものとして扱ってください。再スキャンにより、Linux が拒否するパーティションテーブルが残り、デバイスが起動しなくなる可能性があります。</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>今すぐデバイスを取り外してください</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>選択したファイルを読み取る権限がありません。</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">イメージは非圧縮でのみ読み出せます。.gz や .xz の拡張子を付けないファイル名を選んでください。

圧縮イメージ (.img.gz、.img.xz) は書き込みと検証には使用できます。</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>読み込みに失敗しました。</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>検証に失敗しました。</translation>
    </message>
    <message>
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
        <source>Verify cancelled.</source>
        <translation>検証を中止しました。</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>検証中…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>パーティションテーブルの破損</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>修復に失敗しました</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>パーティションテーブルを修復できませんでした: %1</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>セクタ %1 でデバイスを読み取れませんでした。</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>デバイスにはイメージが正しく書き込まれていますが、パーティションテーブルが壊れています:</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>イメージがデバイスより大きいです</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>デバイスにはイメージが正しく書き込まれていますが、パーティションテーブルはまだ壊れています。「書き込み後に GPT を修正する」をオンにしてイメージを書き込み直すか、もう一度検証を実行して修復を実行してください。</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>検証に成功しました。

デバイスのパーティションテーブルは破損していましたが、修復しました。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>検証に成功しました。

イメージとデバイスの違いは GPT のみで、デバイス上の GPT は正常です。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>検証に成功しました。

イメージとデバイスの違いは GPT のみです。</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[ディスク %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>使用するイメージファイルを指定してください。</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>ディスクを検索しています…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation>チェックサムを生成するためにファイル全体を読み取れませんでした:
%1</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1。プライマリ GPT ヘッダーが、パーティションエントリの存在しないセクタを指しています。

これは、「書き込み後に GPT を修正する」を使わずに書き込んだカードを Windows が再スキャンしたときに残る状態です。データは失われていませんが、デバイスは起動せず、多くのツールがこのテーブルを受け付けません。

今すぐパーティションテーブルを修復しますか？</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>デバイスを選択してください。</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>デバイスをロックできませんでした。</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>デバイスを開けませんでした。</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>このデバイスのパーティションテーブルは壊れています:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>パーティションテーブルを修復しました。</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>パーティションテーブルはまだ破損しています。</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>パーティションテーブルは正常です。</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>パーティションテーブル</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>このデバイスの GPT は正常です。ヘッダーと、それが指すパーティションエントリは一致しています。</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>このデバイスに GPT はありません。</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>このデバイスには GPT がないため、ここで確認する破損は発生しません。</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>パーティションテーブルを読み取れませんでした。</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>パーティションテーブルを読み取れなかったか、この機能が修復できる破損とは別の形で壊れています。</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>書き込み中…</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>イメージがデバイスより大きいため、末尾は書き込まれず、デバイスには完全なイメージが入っていません。

圧縮イメージは非圧縮サイズを記録しないため、デバイスがいっぱいになるまでこれを検出できませんでした。</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>書き込みに成功しました。

GPT がデバイス (%1) と一致したため、Windows が修復するものはありません。通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>書き込みに成功しました。

このイメージは GPT ではなく MBR パーティションテーブルを使用しているため、Windows の GPT 書き換えの不具合の影響を受けません。通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>書き込みに成功しました。

このイメージにはパーティションテーブルがないため、Windows の GPT 書き換えの不具合の影響を受けません。通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>デバイスはオフラインにされ、取り出されました。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>デバイスをオフラインにできませんでした。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>GPT を修正できませんでした (%1)。</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>不正な GPT</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>「書き込み後に GPT を修正する」はオフです。</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>このイメージは影響を受けます。最初のパーティションの前に領域を確保しているため、再スキャンによりプライマリテーブルが誤ったセクタを指します。Windows は結果を受け入れますが Linux は受け入れず、デバイスは起動しません。</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>このイメージは影響を受けません。再スキャンではテーブルが書き換えられますが、このレイアウトでは正しい値が書き込まれます。今取り外せば、いずれにせよイメージと同一のままです。</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>このイメージが影響を受けるかどうかは判別できませんでした。影響を受けるものとして扱ってください。再スキャンにより Linux が拒否するテーブルが残り、デバイスは起動しません。</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>書き込みに成功しましたが、パーティションテーブルが危険な状態です。

%1 %2

%3

今すぐデバイスを取り外し、このパソコンに再挿入しないでください。そのまま対象のハードウェアに差し込んでください。</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation>パーティションの選択</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation>イメージに含めるパーティションを選択してください。チェックを外したものは、未パーティション領域と同様に取り除かれます。</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation>パーティション %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation>パーティション %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation>少なくとも1つのパーティションはチェックしたままにしてください。</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>読み込みエラー</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation>イメージは非圧縮でのみ読み戻せます。.gz または .xz 拡張子を使わないファイル名を選択するか、「.img.gz として読み取る」または「.img.xz として読み取る」をオンにしてください。</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>読み込み元のデバイスを選択してください。</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>ファイルを上書きしてもよろしいですか？</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation>デバイスにパーティションテーブルが見つからなかったため、選択できるものがありません。デバイス全体を読み取ります。</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation>読み取りをキャンセルしました。</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>指定されたイメージに対しディスク容量が十分ではありません。</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>読み込み中…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>読み込みがキャンセルされました。</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>読み込み成功。</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>ファイル情報</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>データを保存するファイルを指定してください。</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>照合エラー</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>照合するデバイスを選択してください。</translation>
    </message>
    <message>
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
        <source>Size Mismatch!</source>
        <translation>サイズが合いません！</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>照合失敗</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>セクタ %1 で検証に失敗しました。</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>イメージがデバイスより大きいため、収まる部分のみ比較できました。比較した範囲はすべて一致しましたが、デバイスには完全なイメージが入っていません。

圧縮イメージは非圧縮サイズを記録しないため、デバイスの末尾に達するまでこれを検出できませんでした。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">検証に成功しました。

イメージとデバイスの違いは GPT のみで、これは「書き込み後に GPT を修正する」オプションが意図的に書き換えたものです。</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

デバイスを取り出しました。今すぐ取り外してください。</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

デバイスを自動的にオフラインにできませんでした。</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>照合に成功しました。</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>ファイルのハンドルを取得しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>デバイスのハンドルを取得しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>%1 を含むボリュームの空き領域を取得できませんでした。
エラー %2: %3
空き領域の確認はスキップされます。</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>ロックエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">ボリュームをロックしようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>アンロックエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>ボリュームのロックを解除しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>マウント解除エラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>ボリュームをマウント解除しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>読み込みエラー</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>セクター数が多すぎます。</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>読み取りバッファー用のメモリを確保できませんでした。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>ハンドルからデータを読み取ろうとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>ハンドルにデータを書き込もうとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>デバイスは %2 バイトのうち %1 バイトしか受け付けませんでした。デバイス上のイメージは不完全です。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>デバイスのジオメトリを取得中にエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>ファイルサイズを取得中にエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>空き領域エラー</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">ドライブ%1の空きスペースを取得できませんでした。
エラー%2：%3
空き容量の確認はスキップされます。</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>不明なデバイス</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>ボリューム %1 をロックできませんでした: まだ使用中です。
デバイスを使用しているプログラムをすべて閉じて、もう一度お試しください。
エラー %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>プライマリ GPT ヘッダーのサイズが範囲外です</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>プライマリ GPT ヘッダーのチェックサムが不正です</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT パーティションエントリ配列がヘッダーの示す位置にありません</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>GPT エントリ配列がデバイスに収まりません</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>GPT パーティションエントリ配列のチェックサムが不正です</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>パーティションがデバイスの末尾を超えて広がっています</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>バックアップ GPT を LBA %1 に移動しました。最終使用可能 LBA は %2 になりました。LBA %3 の古いコピーは消去しました</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>バックアップ GPT を LBA %1 に移動しました。最終使用可能 LBA は %2 になりました</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>MBR に縮小先となるパーティションがありません</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>再配置後のレイアウトは 32 ビットの MBR エントリに収まりません</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>デバイスはすでにこれ以上詰められない状態です。縮小できません</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA は再配置に使用できません</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>GPT に縮小先となるパーティションがありません</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>パーティションエントリがあり得ない範囲を示しています</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>デバイスのジオメトリが使用できません</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>プライマリ GPT ヘッダーを読み取れません</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>GPT エントリ配列のジオメトリが使用できません</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>デバイスが小さすぎてエントリ配列を保持できません</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>パーティションエントリを読み取れませんでした</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>パーティションエントリが LBA 2 にないため、この機能で修復できる破損ではありません</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>修復したヘッダーを書き込めませんでした</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA を LBA 2 に戻し、ヘッダーのチェックサムを再計算しました</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>デバイスがセクタサイズ 0 を報告しています。</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>イメージファイルを開けませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>イメージファイルのサイズを読み取れませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>イメージファイルを読み取れませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>イメージファイルを先頭に戻せませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>gzip 展開処理を開始できませんでした (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>xz 展開処理を開始できませんでした (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>イメージファイルが圧縮データの途中で終わっています。切り詰められているか破損しています。</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>gzip イメージを展開できませんでした。</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>gzip イメージが破損しています (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>xz イメージが破損しています (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>圧縮イメージは前方向にしか読み取れません。</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>イメージファイルを作成できませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>gzip 圧縮処理を開始できませんでした (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>xz 圧縮処理を開始できませんでした (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>gzip 圧縮に失敗しました (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>xz 圧縮に失敗しました (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>イメージファイルを書き込めませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>イメージファイルは書き込み用に開かれていません。</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>イメージファイルをフラッシュできませんでした (エラー %1)。</translation>
    </message>
</context>
</TS>
