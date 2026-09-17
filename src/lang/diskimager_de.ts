<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE">
<context>
    <name>MainWindow</name>
    <message>
        <source>Win32 Disk Imager</source>
        <translation>Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Image File</source>
        <translation>Image-Datei</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>Prüfen</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Datenträger</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation>Image beim Lesen verkleinern</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation>Liest den MBR oder die GPT des Datenträgers und verkleinert das Image so, dass es nur die tatsächlichen Partitionen umfasst. Verschiebt die Sicherungs-GPT an das Ende des belegten Speicherbereichs.</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation>Als .img.gz lesen</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation>Komprimiert das vom Gerät gelesene Image mit gz</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation>Als .img.xz lesen</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation>Komprimiert das vom Gerät gelesene Image mit xz</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation>GPT prüfen</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>Prüft den aktuell ausgewählten Datenträger auf eine beschädigte GPT und bietet an, sie zu reparieren.</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>Prüfsumme der Image-Datei</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>Prüfsummentyp, der für die Image-Datei berechnet wird</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>Ausgewählten Hash-Wert für Datei erzeugen</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>Erzeugen</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>Hash-Wert in Zwischenablage kopieren</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>GPT nach dem Schreiben reparieren</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>Verschiebt die Sicherungs-GPT nach dem Schreiben an das Ende des Datenträgers und passt den Header an, sodass Windows nichts zu „reparieren“ hat. Ohne Häkchen wird stattdessen eine Warnung angezeigt, den Datenträger zu entfernen.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Alle Datenträger anzeigen</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Listet auch fest eingebaute Datenträger auf. Interne PCIe-Kartenleser melden die Karte oft als nicht wechselbares Gerät, das sonst ausgeblendet bleibt. Der Datenträger, von dem Windows läuft, wird nie aufgeführt.</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>Fortschritt</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>Aktuellen Vorgang abbrechen.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Vom Datenträger lesen und als Image-Datei schreiben</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Lesen</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Image-Datei auf den Datenträger schreiben</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>Schreiben</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Datenträger mit der Image-Datei vergleichen</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">Image-Datei mit ausgewähltem Datenträger vergleichen</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">Nur prüfen</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation>Win32 Disk Imager beenden</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>Beenden?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt das Programm beenden, führt das zu einer beschädigten Image-Datei.
Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt das Programm beenden, führt das zu einem beschädigten Datenträger.
Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>Wählen Sie eine Image-Datei aus</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>Wird berechnet …</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>Abbrechen?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Wenn Sie jetzt abbrechen, führt das zu einem beschädigten Ziel.
Sind Sie sicher, dass Sie jetzt abbrechen möchten?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Fehler beim Schreiben</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>Image-Datei kann nicht auf dem Zielgerät gefunden werden.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>Überschreiben bestätigen</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">Warte auf etwas zu tun.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt beenden, wird die Überprüfung des Images abgebrochen.
Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Überprüfung abbrechen.
Sind Sie sicher, dass Sie abbrechen möchten?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>Nicht genug verfügbarer Speicherplatz!</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>Dateifehler</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">Schreiben erfolgreich, aber die Partitionstabelle ist gefährdet.

%1
%2

%3

Entfernen Sie den Datenträger JETZT physisch, bevor Sie irgendetwas anderes tun, und stecken Sie ihn nicht wieder in diesen Computer. Stecken Sie ihn stattdessen in die Zielhardware.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>Die ausgewählte Datei ist nicht vorhanden.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>Die angegebene Datei enthält keine Daten.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>Erledigt.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>Abgeschlossen</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>Schreiben war erfolgreich.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation>Datenträger-Images (*.img *.IMG *.img.gz *.img.xz)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation>Komprimierte Datenträger-Images (*.img.gz *.img.xz *.gz *.xz)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation>Die Datei konnte zum Berechnen der Prüfsumme nicht geöffnet werden:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>Bitte wählen Sie einen Zieldatenträger aus.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Alle Dateien und Daten auf diesem Gerät werden gelöscht.
(Zieldatenträger: %1)
Möchten Sie wirklich fortfahren?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>Der Datenträger hat eingebundene Volumes</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 ist in Windows als %2 eingebunden.

Alles auf diesem Datenträger, auf jeder seiner Partitionen, wird unwiederbringlich zerstört.

Prüfen Sie, dass %2 kein Laufwerk ist, das Sie behalten wollten.

Trotzdem auf diesen Datenträger schreiben?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>Schreiben fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Datenträgerfehler</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>Der Datenträger meldet eine Größe von null. Falls es sich um einen Kartenleser handelt, wurde die Karte möglicherweise entfernt.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: mindestens %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Das Ende des Images wird nicht geschrieben, der Datenträger enthält dann kein vollständiges Image.

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Es wird mehr Speicherplatz benötigt als verfügbar ist:
  Benötigt: %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich konnte nicht auf Daten geprüft werden, weil das Image komprimiert ist

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Es wird mehr Speicherplatz benötigt als verfügbar ist:
  Benötigt: %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint Daten zu ENTHALTEN

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Es wird mehr Speicherplatz benötigt als verfügbar ist:
  Benötigt: %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint keine Daten zu enthalten

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>Schreiben abgebrochen.</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>Alte Partitionstabellen werden gelöscht …</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>Die vorhandenen Partitionstabellen auf dem Datenträger konnten nicht gelöscht werden.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>Der Datenträger wurde nur teilweise beschrieben und enthält kein verwendbares Image mehr. Schreiben Sie das Image erneut, bevor Sie den Datenträger verwenden.</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>GPT wird repariert …</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>Image abgeschnitten</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">Das Image ist größer als der Datenträger, daher wurde sein Ende nicht geschrieben und der Datenträger enthält kein vollständiges Image.

Dies konnte erst festgestellt werden, als der Datenträger voll war, weil ein gzip-Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">Schreiben erfolgreich.

Die GPT wurde mit dem Datenträger in Einklang gebracht (%1), sodass Windows keine beschädigte Tabelle zu reparieren hat. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">Schreiben erfolgreich.

Das Image enthält keine GPT, also gibt es für Windows keine Partitionstabelle zu reparieren. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>Schreiben erfolgreich.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>Schreiben erfolgreich</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">Der Datenträger wurde offline geschaltet und ausgeworfen.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">Der Datenträger konnte NICHT automatisch offline geschaltet werden.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">Die GPT konnte nicht automatisch repariert werden (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">die GPT ist fehlerhaft aufgebaut</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>Das Reparieren der GPT ist fehlgeschlagen (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>Schreibfehler</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">Die Option „GPT nach dem Schreiben reparieren“ ist nicht aktiviert.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">Dieses Image IST vom GPT-Rewrite-Fehler von Windows betroffen.

Es reserviert Platz vor seiner ersten Partition, daher schreibt Windows bei einem erneuten Einlesen die primäre Partitionstabelle so um, dass sie auf die falschen Sektoren zeigt. Das Ergebnis besteht die Prüfungen von Windows weiterhin, wird von Linux aber abgelehnt, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">Dieses Image ist NICHT vom GPT-Rewrite-Fehler von Windows betroffen.

Windows schreibt die Tabelle bei einem erneuten Einlesen trotzdem um, weil die Sicherungs-GPT nicht am Ende des Datenträgers liegt, aber bei diesem Layout trifft die Neuberechnung die richtigen Werte. Wenn Sie den Datenträger jetzt entfernen, bleibt er in jedem Fall Byte für Byte mit dem Image identisch.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">Ob dieses Image vom GPT-Rewrite-Fehler von Windows betroffen ist, konnte nicht ermittelt werden. Gehen Sie davon aus, dass es betroffen ist: Ein erneutes Einlesen kann dazu führen, dass Linux die Partitionstabelle ablehnt und der Datenträger nicht mehr startet.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>Entfernen Sie den Datenträger jetzt</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>Sie haben keine Berechtigung, die ausgewählte Datei zu lesen.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">Images können nur unkomprimiert zurückgelesen werden. Wählen Sie einen Dateinamen ohne die Endung .gz oder .xz.

Komprimierte Images (.img.gz, .img.xz) können geschrieben und geprüft werden.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>Lesen fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>Überprüfung fehlgeschlagen.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: mindestens %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Es kann nur der Teil verglichen werden, der darauf passt.

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich konnte nicht auf Daten geprüft werden, weil das Image komprimiert ist

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>Überprüfung abgebrochen.</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>Überprüfen …</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>Partitionstabelle beschädigt</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>Reparatur fehlgeschlagen</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>Die Partitionstabelle konnte nicht repariert werden: %1</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>Image größer als der Datenträger</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">Das Image ist größer als der Datenträger, daher konnte nur der Teil verglichen werden, der darauf passt. Alles Verglichene stimmte überein, aber der Datenträger enthält kein vollständiges Image.

Dies konnte erst am Ende des Datenträgers festgestellt werden, weil ein gzip-Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[Datenträger %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>Bitte geben Sie eine Image-Datei an, die Sie verwenden möchten.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>Datenträger werden durchsucht …</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation>Die Datei konnte zum Berechnen der Prüfsumme nicht vollständig gelesen werden:
%1</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1 Der primäre GPT-Header verweist auf Sektoren, in denen die Partitionseinträge nicht liegen.

Genau das hinterlässt Windows, wenn es eine Karte neu einliest, die ohne „GPT nach dem Schreiben reparieren“ geschrieben wurde. Es sind keine Daten verloren gegangen, aber der Datenträger startet nicht und die meisten Programme lehnen die Tabelle ab.

Die Partitionstabelle jetzt reparieren?</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>Bitte wählen Sie einen Datenträger aus.</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>Der Datenträger konnte nicht gesperrt werden.</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>Der Datenträger konnte nicht geöffnet werden.</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>Die Partitionstabelle dieses Datenträgers ist beschädigt:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>Partitionstabelle repariert.</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>Die Partitionstabelle ist weiterhin beschädigt.</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>Die Partitionstabelle ist gültig.</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>Partitionstabelle</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>Die GPT auf diesem Datenträger ist gültig: Der Header und die Partitionseinträge, auf die er verweist, stimmen überein.</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>Keine GPT auf diesem Datenträger.</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>Dieser Datenträger hat keine GPT und kann daher den hier geprüften Schaden nicht aufweisen.</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>Die Partitionstabelle konnte nicht gelesen werden.</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>Die Partitionstabelle konnte nicht gelesen werden oder ist auf andere Weise beschädigt, als diese Funktion reparieren kann.</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Schreiben …</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>Das Image ist größer als der Datenträger, daher wurde sein Ende nicht geschrieben und der Datenträger enthält kein vollständiges Image.

Dies konnte erst festgestellt werden, als der Datenträger voll war, weil das komprimierte Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Die GPT entspricht jetzt dem Datenträger (%1), sodass Windows nichts zu reparieren hat. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Dieses Image verwendet eine MBR-Partitionstabelle und keine GPT, der Windows-GPT-Fehler kann es also nicht betreffen. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Dieses Image hat keine Partitionstabelle, der Windows-GPT-Fehler kann es also nicht betreffen. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>Der Datenträger ist offline und ausgeworfen.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>Der Datenträger konnte NICHT offline geschaltet werden.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>Die GPT konnte nicht repariert werden (%1).</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>fehlerhafte GPT</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>„GPT nach dem Schreiben reparieren“ ist deaktiviert.</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>Dieses Image IST betroffen: Es reserviert Platz vor der ersten Partition, sodass ein erneutes Einlesen die primäre Tabelle auf die falschen Sektoren zeigen lässt. Windows akzeptiert das Ergebnis weiterhin, Linux nicht, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>Dieses Image ist NICHT betroffen: Ein erneutes Einlesen schreibt die Tabelle zwar neu, bei diesem Layout aber mit den richtigen Werten. Wird der Datenträger jetzt entfernt, bleibt er ohnehin mit dem Image identisch.</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>Ob dieses Image betroffen ist, konnte nicht ermittelt werden. Gehen Sie davon aus: Ein erneutes Einlesen kann eine Tabelle hinterlassen, die Linux ablehnt, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>Schreiben erfolgreich, aber die Partitionstabelle ist gefährdet.

%1 %2

%3

Entfernen Sie den Datenträger JETZT und stecken Sie ihn hier nicht wieder ein. Setzen Sie ihn direkt in die Zielhardware ein.</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Lesefehler</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation>Images können nur unkomprimiert zurückgelesen werden. Wählen Sie einen Dateinamen ohne die Endung .gz oder .xz, oder aktivieren Sie „Als .img.gz lesen“ oder „Als .img.xz lesen“.</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>Bitte wählen Sie einen Quelldatenträger aus.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>Überschreiben bestätigen</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>Sind Sie sicher, dass die angegebene Datei überschrieben werden soll?</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>Der Datenträger ist nicht groß genug für die angegebene Image-Datei.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Lesen …</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>Lesevorgang abgebrochen.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>Lesen war erfolgreich.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>Datei-Info</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>Bitte geben Sie eine Datei zum Speichern der Daten an.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>Fehler beim Überprüfen</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>Bitte wählen Sie einen Datenträger zum Vergleichen aus.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint Daten zu ENTHALTEN

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint keine Daten zu enthalten

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>Der Datenträger konnte bei Sektor %1 nicht gelesen werden.</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>Der Datenträger enthält das Image korrekt, aber seine Partitionstabelle ist beschädigt:</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>Der Datenträger enthält das Image korrekt, aber seine Partitionstabelle ist weiterhin beschädigt. Schreiben Sie das Image erneut mit aktivierter Option „GPT nach dem Schreiben reparieren“, oder führen Sie die Überprüfung erneut aus und bestätigen Sie die Reparatur.</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>Überprüfung erfolgreich.

Die Partitionstabelle des Datenträgers war beschädigt und wurde repariert.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>Überprüfung erfolgreich.

Image und Datenträger unterscheiden sich nur in der GPT, und die GPT auf dem Datenträger ist gültig.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>Überprüfung erfolgreich.

Image und Datenträger unterscheiden sich nur in der GPT.</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>Größe stimmt nicht überein!</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>Überprüfung fehlgeschlagen</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>Überprüfung fehlgeschlagen bei Sektor: %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>Das Image ist größer als der Datenträger, daher konnte nur der Teil verglichen werden, der darauf passt. Alles Verglichene stimmte überein, aber der Datenträger enthält kein vollständiges Image.

Dies konnte erst am Ende des Datenträgers festgestellt werden, weil das komprimierte Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">Prüfung erfolgreich.

Image und Datenträger unterscheiden sich nur in der GPT, die von der Option „GPT nach dem Schreiben reparieren“ absichtlich neu geschrieben wird.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

Der Datenträger wurde ausgeworfen. Entfernen Sie ihn jetzt.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

Der Datenträger konnte NICHT automatisch offline geschaltet werden.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>Überprüfung erfolgreich.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>Dateifehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>Fehler beim Versuch, ein Handle auf die Datei zu erhalten.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Datenträgerfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>Fehler beim Versuch, ein Handle auf den Datenträger zu erhalten.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>Der freie Speicherplatz auf dem Volume mit %1 konnte nicht ermittelt werden.
Fehler %2: %3
Die Prüfung des freien Speicherplatzes wird übersprungen.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>Sperrfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">Fehler beim Versuch, den Datenträger zu sperren. Fehler %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>Entsperrfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>Fehler beim Versuch, den Datenträger zu entsperren.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>Fehler beim Aushängen</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>Fehler beim Versuch, den Datenträger auszuhängen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Lesefehler</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>Die Sektoranzahl ist zu groß.</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>Für den Lesepuffer konnte kein Speicher reserviert werden.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>Fehler beim Versuch, Daten zu lesen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Fehler beim Schreiben</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>Fehler beim Versuch, Daten zu schreiben.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>Der Datenträger hat nur %1 von %2 Bytes angenommen. Das Image auf dem Datenträger ist unvollständig.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>Fehler beim Versuch, die Geometrie des Datenträgers abzufragen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>Fehler beim Versuch, die Dateigröße abzufragen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>Fehler beim freien Speicherplatz</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Fehler beim Ermitteln des freien Speicherplatzes auf Laufwerk %1. Fehler %2: %3 Überprüfung des freien Speicherplatzes wird übersprungen.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>Unbekannter Datenträger</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>Volume %1 konnte nicht gesperrt werden: Es ist noch in Benutzung.
Schließen Sie alle Programme, die den Datenträger verwenden, und versuchen Sie es erneut.
Fehler %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>die Größe des primären GPT-Headers liegt außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>die Prüfsumme des primären GPT-Headers ist ungültig</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>das GPT-Partitionseintragsfeld liegt nicht dort, wo der Header es angibt</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>das GPT-Eintragsfeld passt nicht auf den Datenträger</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>die Prüfsumme des GPT-Partitionseintragsfelds ist ungültig</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>eine Partition reicht über das Ende des Datenträgers hinaus</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>Sicherungs-GPT nach LBA %1 verschoben; letzte nutzbare LBA ist jetzt %2; die veraltete Kopie bei LBA %3 wurde gelöscht</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>Sicherungs-GPT nach LBA %1 verschoben; letzte nutzbare LBA ist jetzt %2</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>der MBR enthält keine Partitionen, auf die verkleinert werden könnte</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>der Datenträger ist bereits so knapp bemessen; nichts zu verkleinern</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA eignet sich nicht zum Neupacken</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>die GPT enthält keine Partitionen, auf die verkleinert werden könnte</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>ein Partitionseintrag beschreibt einen unmöglichen Bereich</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>die Geometrie des Datenträgers ist unbrauchbar</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>der primäre GPT-Header ist nicht lesbar</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>die Geometrie des GPT-Eintragsfelds ist unbrauchbar</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>der Datenträger ist zu klein für ein Eintragsfeld</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>die Partitionseinträge konnten nicht gelesen werden</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>die Partitionseinträge liegen nicht bei LBA 2, daher handelt es sich nicht um den Schaden, den diese Funktion reparieren kann</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>der reparierte Header konnte nicht geschrieben werden</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA wurde wieder auf LBA 2 gesetzt und die Header-Prüfsumme neu berechnet</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>Der Datenträger meldet eine Sektorgröße von null.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>Die Image-Datei konnte nicht geöffnet werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>Die Größe der Image-Datei konnte nicht gelesen werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>Die Image-Datei konnte nicht gelesen werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>Die Image-Datei konnte nicht zurückgespult werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>Die gzip-Dekomprimierung konnte nicht gestartet werden (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>Die xz-Dekomprimierung konnte nicht gestartet werden (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>Die Image-Datei endet mitten in den komprimierten Daten. Sie ist abgeschnitten oder beschädigt.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>Das gzip-Image konnte nicht dekomprimiert werden.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>Das gzip-Image ist beschädigt (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>Das xz-Image ist beschädigt (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>Ein komprimiertes Image kann nur vorwärts gelesen werden.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>Die Image-Datei konnte nicht erstellt werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>Die gzip-Komprimierung konnte nicht gestartet werden (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>Die xz-Komprimierung konnte nicht gestartet werden (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>Die gzip-Komprimierung ist fehlgeschlagen (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>Die xz-Komprimierung ist fehlgeschlagen (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>Die Image-Datei konnte nicht geschrieben werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>Die Puffer der Image-Datei konnten nicht geleert werden (Fehler %1).</translation>
    </message>
</context>
</TS>
