cd C:\saturn\sblproj
make -f makefile
mkdir cd
copy out.bin cd\0.bin
copy c:\saturn\cdtools\abs.txt cd\abs.txt
copy c:\saturn\cdtools\bib.txt cd\bib.txt
copy c:\saturn\cdtools\cpy.txt cd\cpy.txt
c:\saturn\cdtools\mkisofs -sysid "SEGA SATURN" -volid "SaturnApp" -volset "SaturnApp" -publisher "SEGA ENTERPRISES, LTD." -preparer "SEGA ENTERPRISES, LTD." -appid "SaturnApp" -abstract "c:\saturn\cdtools\ABS.TXT" -copyright "c:\saturn\cdtools\CPY.TXT" -biblio "c:\saturn\cdtools\BIB.TXT" -generic-boot "c:\saturn\cdtools\ip_gnu.bin" -full-iso9660-filenames -o out.iso cd 
pause
c:\saturn\yabause\!yabause.exe 