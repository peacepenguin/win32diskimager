To help contribute translations, you can either download the base translation
for the current release (src/lang/diskimager_en.ts) or clone the git
repository.

===========
Getting Qt:
===========
You will need Qt Linguist. The MSYS2 setup in BUILD.md already installs it,
as part of mingw-w64-ucrt-x86_64-qt6-tools, and it lands at

    C:\msys64\ucrt64\bin\linguist.exe

======================
Add a new translation:
======================
Run Qt Linguist.
File -> Open src/lang/diskimager_en.ts
Edit -> Translation File Settings, set target language and country/region.
File -> Save As src/lang/diskimager_??.ts (where ?? is the region code for
your translation).
Translate all the strings into the new language and save the file.

To have it built into the program, add the language code to the LANGUAGES
list in src/CMakeLists.txt and add the matching lang/diskimager_??.qm entry
to src/translations.qrc, then rebuild.

======================
Share your translation:
======================
Attach the diskimager_??.ts file to an issue, or fork the repository and open
a pull request.
