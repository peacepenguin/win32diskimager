<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="+32"/>
        <source>Win32 Disk Imager</source>
        <translation>Win32 Disk Imager</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Image File</source>
        <translation>Image-Datei</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Verify</source>
        <translation>Prüfen</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Device</source>
        <translation>Datenträger</translation>
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
        <translation>Prüfsumme der Image-Datei</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Hash type to generate for image file</source>
        <translation>Prüfsummentyp, der für die Image-Datei berechnet wird</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Generate selected hash on file</source>
        <translation>Ausgewählten Hash-Wert für Datei erzeugen</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Generate</source>
        <translation>Erzeugen</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Copy hash to clipboard</source>
        <translation>Hash-Wert in Zwischenablage kopieren</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <location line="-131"/>
        <source>Fix GPT after write</source>
        <translation>GPT nach dem Schreiben reparieren</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>Verschiebt die Sicherungs-GPT nach dem Schreiben an das Ende des Datenträgers und passt den Header an, sodass Windows nichts zu „reparieren“ hat. Ohne Häkchen wird stattdessen eine Warnung angezeigt, den Datenträger zu entfernen.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Show all devices</source>
        <translation>Alle Datenträger anzeigen</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Listet auch fest eingebaute Datenträger auf. Interne PCIe-Kartenleser melden die Karte oft als nicht wechselbares Gerät, das sonst ausgeblendet bleibt. Der Datenträger, von dem Windows läuft, wird nie aufgeführt.</translation>
    </message>
    <message>
        <location line="+158"/>
        <source>Progress</source>
        <translation>Fortschritt</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Cancel current process.</source>
        <translation>Aktuellen Vorgang abbrechen.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location line="-316"/>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Vom Datenträger lesen und als Image-Datei schreiben</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Read</source>
        <translation>Lesen</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Image Datei auf Datenträger schreiben</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Write</source>
        <translation>Schreiben</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Datenträger mit der Image Datei vergleichen</translation>
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
        <location line="+364"/>
        <source>Exit Win32 Disk Imager</source>
        <translation>Win32 Disk Imager beenden</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Exit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="+533"/>
        <source>Exit?</source>
        <translation>Beenden?</translation>
    </message>
    <message>
        <location line="-18"/>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt das Programm beenden, führt das zu einer beschädigte Image-Datei. Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt das Programm beenden, führt das zu einem beschädigten Datenträger. Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Select a disk image</source>
        <translation>Wählen Sie eine Image-Datei</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Generating...</source>
        <translation>Berechne...</translation>
    </message>
    <message>
        <location line="+181"/>
        <location line="+9"/>
        <source>Cancel?</source>
        <translation>Abbrechen?</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Wenn Sie jetzt abbrechen, führt das zu einem beschädigtem Ziel. Sind Sie sicher, dass Sie jetzt abbrechen möchten?</translation>
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
        <translation>Fehler beim Schreiben</translation>
    </message>
    <message>
        <location line="-504"/>
        <location line="+423"/>
        <location line="+168"/>
        <source>Image file cannot be located on the target device.</source>
        <translation>Image-Datei kann nicht auf dem Zielgerät gefunden werden.</translation>
    </message>
    <message>
        <location line="-582"/>
        <source>Confirm overwrite</source>
        <translation>Überschreiben bestätigen</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">Warte auf etwas zu tun.</translation>
    </message>
    <message>
        <location line="-295"/>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Überprüfung im Gange. Sind Sie sicher, dass Sie jetzt beenden wollen?</translation>
    </message>
    <message>
        <location line="+265"/>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Überprüfung abbrechen. Sind Sie sicher, dass Sie abbrechen wollen?</translation>
    </message>
    <message>
        <location line="+101"/>
        <location line="+31"/>
        <source>Not enough available space!</source>
        <translation>Nicht genug verfügbarer Speicherplatz!</translation>
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
        <location line="-578"/>
        <location line="+555"/>
        <source>The selected file does not exist.</source>
        <translation>Die ausgewählte Datei ist nicht vorhanden.</translation>
    </message>
    <message>
        <location line="-835"/>
        <location line="+290"/>
        <location line="+275"/>
        <location line="+280"/>
        <source>The specified file contains no data.</source>
        <translation>Die angegebene Datei enthält keine Daten.</translation>
    </message>
    <message>
        <location line="-551"/>
        <location line="+200"/>
        <location line="+355"/>
        <source>Done.</source>
        <translation>Erledigt.</translation>
    </message>
    <message>
        <location line="-351"/>
        <location line="+2"/>
        <location line="+328"/>
        <location line="+25"/>
        <source>Complete</source>
        <translation>Abschließen</translation>
    </message>
    <message>
        <location line="-555"/>
        <source>Write Successful.</source>
        <translation>Schreiben war erfolgreich.</translation>
    </message>
    <message>
        <location line="-742"/>
        <location line="+1"/>
        <source>Disk Images (*.img *.IMG *.img.gz *.img.xz)</source>
        <translation>Datenträger-Images (*.img *.IMG *.img.gz *.img.xz)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)</source>
        <translation>Komprimierte Datenträger-Images (*.img.gz *.img.xz *.gz *.xz)</translation>
    </message>
    <message>
        <location line="+182"/>
        <location line="+11"/>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation>Die Datei konnte zum Berechnen der Prüfsumme nicht geöffnet werden:
%1</translation>
    </message>
    <message>
        <location line="+201"/>
        <source>Please select a target device.</source>
        <translation>Bitte wählen Sie einen Zieldatenträger aus.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Writing to a physical device can corrupt the device.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Das Schreiben auf einen physischen Datenträger kann diesen beschädigen.
(Zieldatenträger: %1)
Möchten Sie wirklich fortfahren?</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Device has mounted volumes</source>
        <translation>Der Datenträger hat eingebundene Volumes</translation>
    </message>
    <message>
        <location line="+1"/>
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
        <location line="+21"/>
        <location line="+21"/>
        <location line="+21"/>
        <location line="+55"/>
        <location line="+24"/>
        <location line="+23"/>
        <source>Write failed.</source>
        <translation>Schreiben fehlgeschlagen.</translation>
    </message>
    <message>
        <location line="-788"/>
        <location line="+464"/>
        <location line="+24"/>
        <location line="+590"/>
        <source>Device Error</source>
        <translation>Datenträgerfehler</translation>
    </message>
    <message>
        <location line="-1077"/>
        <location line="+488"/>
        <location line="+590"/>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>Der Datenträger meldet eine Größe von null. Falls es sich um einen Kartenleser handelt, wurde die Karte möglicherweise entfernt.</translation>
    </message>
    <message>
        <location line="-405"/>
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
        <location line="+25"/>
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
        <location line="+5"/>
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
        <location line="+3"/>
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
        <location line="+15"/>
        <source>Write cancelled.</source>
        <translation>Schreiben abgebrochen.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Clearing old partition tables...</source>
        <translation>Alte Partitionstabellen werden gelöscht …</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>Die vorhandenen Partitionstabellen auf dem Datenträger konnten nicht gelöscht werden.</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+24"/>
        <location line="+21"/>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>Das Gerät wurde nur teilweise beschrieben und enthält kein verwendbares Abbild mehr. Schreiben Sie das Abbild erneut, bevor Sie das Gerät verwenden.</translation>
    </message>
    <message>
        <location line="+59"/>
        <source>Fixing GPT...</source>
        <translation>GPT wird repariert …</translation>
    </message>
    <message>
        <location line="+17"/>
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
        <location line="+38"/>
        <source>Write successful.</source>
        <translation>Schreiben erfolgreich.</translation>
    </message>
    <message>
        <location line="+2"/>
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
        <location line="+11"/>
        <source>Fixing the GPT failed (%1).</source>
        <translation>Das Reparieren der GPT ist fehlgeschlagen (%1).</translation>
    </message>
    <message>
        <location line="+1"/>
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
        <location line="+22"/>
        <source>Remove the device now</source>
        <translation>Entfernen Sie den Datenträger jetzt</translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+555"/>
        <source>You do not have permission to read the selected file.</source>
        <translation>Sie haben keine Berechtigung, die ausgewählte Datei zu lesen.</translation>
    </message>
    <message>
        <location line="-486"/>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation>Images können nur unkomprimiert zurückgelesen werden. Wählen Sie einen Dateinamen ohne die Endung .gz oder .xz.

Komprimierte Images (.img.gz, .img.xz) können geschrieben und geprüft werden.</translation>
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
        <translation>Lesen fehlgeschlagen.</translation>
    </message>
    <message>
        <location line="+72"/>
        <location line="+20"/>
        <location line="+21"/>
        <location line="+66"/>
        <location line="+24"/>
        <source>Verify failed.</source>
        <translation>Überprüfung fehlgeschlagen.</translation>
    </message>
    <message>
        <location line="-102"/>
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
        <location line="+22"/>
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
        <location line="+23"/>
        <source>Verify cancelled.</source>
        <translation>Überprüfung abgebrochen.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Verifying...</source>
        <translation>Überprüfen …</translation>
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
        <location line="+997"/>
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
        <location line="+179"/>
        <source>[Disk %1]</source>
        <translation>[Datenträger %1]</translation>
    </message>
    <message>
        <location line="-663"/>
        <location line="+554"/>
        <source>Please specify an image file to use.</source>
        <translation>Bitte geben Sie eine Image-Datei an, die Sie verwenden wollen.</translation>
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
        <translation>Schreiben …</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>Das Image ist größer als der Datenträger, daher wurde sein Ende nicht geschrieben und der Datenträger enthält kein vollständiges Image.

Dies konnte erst festgestellt werden, als der Datenträger voll war, weil das komprimierte Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Die GPT entspricht jetzt dem Datenträger (%1), sodass Windows nichts zu reparieren hat. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Dieses Image verwendet eine MBR-Partitionstabelle und keine GPT, der Windows-GPT-Fehler kann es also nicht betreffen. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Dieses Image hat keine Partitionstabelle, der Windows-GPT-Fehler kann es also nicht betreffen. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>The device is offline and ejected.</source>
        <translation>Der Datenträger ist offline und ausgeworfen.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The device could NOT be taken offline.</source>
        <translation>Der Datenträger konnte NICHT offline geschaltet werden.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The GPT could not be fixed (%1).</source>
        <translation>Die GPT konnte nicht repariert werden (%1).</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>malformed GPT</source>
        <translation>fehlerhafte GPT</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>&quot;GPT nach dem Schreiben reparieren&quot; ist deaktiviert.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>Dieses Image IST betroffen: Es reserviert Platz vor der ersten Partition, sodass ein erneutes Einlesen die primäre Tabelle auf die falschen Sektoren zeigen lässt. Windows akzeptiert das Ergebnis weiterhin, Linux nicht, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>Dieses Image ist NICHT betroffen: Ein erneutes Einlesen schreibt die Tabelle zwar neu, bei diesem Layout aber mit den richtigen Werten. Wird der Datenträger jetzt entfernt, bleibt er ohnehin mit dem Image identisch.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>Ob dieses Image betroffen ist, konnte nicht ermittelt werden. Gehen Sie davon aus: Ein erneutes Einlesen kann eine Tabelle hinterlassen, die Linux ablehnt, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <location line="+5"/>
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
        <location line="+82"/>
        <location line="+15"/>
        <source>Read Error</source>
        <translation>Lesefehler</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please select a source device.</source>
        <translation>Bitte wählen Sie einen Quelldatenträger aus.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Confirm Overwrite</source>
        <translation>Überschreiben bestätigen</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>Sind Sie sicher, dass die angegebene Datei überschrieben werden soll?</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Disk is not large enough for the specified image.</source>
        <translation>Der Datenträger ist nicht groß genug für die angegebene Image-Datei.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Reading...</source>
        <translation>Lesen …</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Read Canceled.</source>
        <translation>Lesevorgang abgebrochen.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Read Successful.</source>
        <translation>Lesen war erfolgreich.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>File Info</source>
        <translation>Datei-Info</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Please specify a file to save data to.</source>
        <translation>Bitte geben Sie eine Datei an zum Speichern der Daten.</translation>
    </message>
    <message>
        <location line="+23"/>
        <location line="+6"/>
        <location line="+13"/>
        <location line="+103"/>
        <location line="+21"/>
        <source>Verify Error</source>
        <translation>Fehler beim Überprüfen</translation>
    </message>
    <message>
        <location line="-137"/>
        <source>Please select a device to verify against.</source>
        <translation>Bitte wählen Sie einen Datenträger zum Vergleichen aus.</translation>
    </message>
    <message>
        <location line="+69"/>
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
        <location line="+3"/>
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
        <location line="+66"/>
        <source>The device could not be read at sector %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+85"/>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
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
        <location line="-238"/>
        <location line="+28"/>
        <source>Size Mismatch!</source>
        <translation>Größe stimmt nicht überein!</translation>
    </message>
    <message>
        <location line="+107"/>
        <source>Verify Failure</source>
        <translation>Überprüfung fehlgeschlagen</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Verification failed at sector: %1</source>
        <translation>Überprüfung fehlgeschlagen bei Sektor: %1</translation>
    </message>
    <message>
        <location line="+69"/>
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
        <location line="+37"/>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

Der Datenträger wurde ausgeworfen. Entfernen Sie ihn jetzt.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

Der Datenträger konnte NICHT automatisch offline geschaltet werden.</translation>
    </message>
    <message>
        <location line="-20"/>
        <location line="+46"/>
        <source>Verify Successful.</source>
        <translation>Überprüfung erfolgreich.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../disk.cpp" line="+64"/>
        <location line="+171"/>
        <source>File Error</source>
        <translation>Dateifehler</translation>
    </message>
    <message>
        <location line="-170"/>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>Fehler beim Versuch, ein Handle auf die Datei zu erhalten. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="+22"/>
        <location line="+115"/>
        <source>Device Error</source>
        <translation>Gerätefehler</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>Fehler beim Versuch, ein Handle auf das Gerät zu erhalten. Fehler %1: %2</translation>
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
        <translation>Sperrfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">Fehler beim Versuch, den Datenträger zu sperren. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="-400"/>
        <source>Unlock Error</source>
        <translation>Entsperrfehler</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>Fehler beim Versuch, den Datenträger zu entsperren. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Dismount Error</source>
        <translation>Aushängen Fehler</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>Fehler beim Versuch, den Datenträger auszuhängen. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="+17"/>
        <location line="+8"/>
        <source>Read Error</source>
        <translation>Lesefehler</translation>
    </message>
    <message>
        <location line="-7"/>
        <location line="+8"/>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>Fehler beim Versuch, Daten zu lesen. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="+23"/>
        <location line="+8"/>
        <location line="+10"/>
        <source>Write Error</source>
        <translation>Fehler beim Schreiben</translation>
    </message>
    <message>
        <location line="-17"/>
        <location line="+8"/>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>Fehler beim Versuch, Daten zu schreiben. Fehler %1: %2</translation>
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
        <translation>Fehler beim Versuch, die Gerätegeometrie abzufragen. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>Fehler beim Versuch, die Dateigröße abzufragen. Fehler %1: %2</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Free Space Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Fehler beim Ermitteln des freien Speicherplatzes auf Laufwerk %1. Fehler %2: %3 Überprüfung des freien Speicherplatzes wird übersprungen.</translation>
    </message>
    <message>
        <location line="+199"/>
        <source>Unknown device</source>
        <translation>Unbekannter Datenträger</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>Volume %1 konnte nicht gesperrt werden: Es ist noch in Benutzung.
Schließen Sie alle Programme, die den Datenträger verwenden, und versuchen Sie es erneut.
Fehler %2</translation>
    </message>
    <message>
        <location line="+214"/>
        <source>the primary GPT header size is out of range</source>
        <translation>die Größe des primären GPT-Headers liegt außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>the primary GPT header checksum is invalid</source>
        <translation>die Prüfsumme des primären GPT-Headers ist ungültig</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>das GPT-Partitionseintragsfeld liegt nicht dort, wo der Header es angibt</translation>
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
        <translation>eine Partition reicht über das Ende des Datenträgers hinaus</translation>
    </message>
    <message>
        <location line="+131"/>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>Sicherungs-GPT nach LBA %1 verschoben; letzte nutzbare LBA ist jetzt %2; die veraltete Kopie bei LBA %3 wurde gelöscht</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>Sicherungs-GPT nach LBA %1 verschoben; letzte nutzbare LBA ist jetzt %2</translation>
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
        <translation>Der Datenträger meldet eine Sektorgröße von null.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The image file could not be opened (error %1).</source>
        <translation>Die Image-Datei konnte nicht geöffnet werden (Fehler %1).</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>Die Größe der Image-Datei konnte nicht gelesen werden (Fehler %1).</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+65"/>
        <location line="+7"/>
        <location line="+183"/>
        <location line="+198"/>
        <source>The image file could not be read (error %1).</source>
        <translation>Die Image-Datei konnte nicht gelesen werden (Fehler %1).</translation>
    </message>
    <message>
        <location line="-411"/>
        <source>The image file could not be rewound (error %1).</source>
        <translation>Die Image-Datei konnte nicht zurückgespult werden (Fehler %1).</translation>
    </message>
    <message>
        <location line="+178"/>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>Die gzip-Dekomprimierung konnte nicht gestartet werden (zlib-Fehler %1).</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>Die xz-Dekomprimierung konnte nicht gestartet werden (lzma-Fehler %1).</translation>
    </message>
    <message>
        <location line="+73"/>
        <location line="+84"/>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>Die Image-Datei endet mitten in den komprimierten Daten. Sie ist abgeschnitten oder beschädigt.</translation>
    </message>
    <message>
        <location line="-37"/>
        <source>The gzip image could not be decompressed.</source>
        <translation>Das gzip-Image konnte nicht dekomprimiert werden.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>Das gzip-Image ist beschädigt (zlib-Fehler %1).</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>Das xz-Image ist beschädigt (lzma-Fehler %1).</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>A compressed image can only be read forwards.</source>
        <translation>Ein komprimiertes Image kann nur vorwärts gelesen werden.</translation>
    </message>
</context>
</TS>
