program bgconv;
uses
  Windows, SysUtils, VCL.Graphics;

var
  i,
  outType, //c file (0) or binary (1)
  fileCount:
  Integer;
  filename: String;
procedure printHelp;
begin
  WriteLn('Usage: bgconv [palette bitmap].bmp [outputfile]');
  WriteLn('Optional parameters:');
  WriteLn('-oc or -ob: output to c file or binary (default: c file)');
end;

procedure outputC(var cFile: TextFile; varName: String; tiles: array of Uint8; palette: array of Uint32);
var
  i: Integer;
begin
  WriteLn(cFile, 'const Uint32 ', varName, '_pal[] = {');
  for i := 0 to Length(palette) - 1 do
  begin
    Write(cFile, palette[i], ',');
  end;
  WriteLn(cFile, '};');
  WriteLn(cFile, 'const Uint8 ', varName, '_chr[] = {');
  for i := 0 to Length(tiles) do
  begin
    Write(cFile, tiles[i], ',');
    if (i + 1) mod 8 = 0 then
      Write(cFile, #13#10);
  end;
  WriteLn(cFile, '};');
end;

procedure handleImage(input: String; outType: Integer);
var
  img: TBitmap;
  imgPal: TMaxLogPalette;
  line: pByteArray;
  saturnTiles: array of Uint8;
  //Saturn palette format:
  //0123456789ABCDEF0123456789ABCDEF
  //RRRRRRRRGGGGGGGGBBBBBBBB00000000
  saturnPal: array[0..255] of Uint32;
  i, j, k, l, count: Integer;
  r, g, b: Uint32;
  outFile: TextFile;
  varName: String;
begin
  AssignFile(outFile, 'out.c');
  ReWrite(outFile);
  img := TBitmap.Create;
  img.LoadFromFile(input);
  SetLength(saturnTiles, img.Height * img.Width);
  GetPaletteEntries(img.Palette, 0, 256, imgPal.palPalEntry);
  for i := 0 to 255 do
  begin
    r := imgPal.palPalEntry[i].peRed;
    g := imgPal.palPalEntry[i].peGreen;
    b := imgPal.palPalEntry[i].peBlue;
//    WriteLn('r: ', imgPal.palPalEntry[i].peRed, 'g: ', imgPal.palPalEntry[i].peGreen,
//      'b: ', imgPal.palPalEntry[i].peBlue);
    saturnPal[i] := r or (g shl 8) or (b shl 16);
  end;
  i := 0;
  j := 0;
  count := 0;
  while i < img.Height - 1 do
  begin
    while j < img.Height - 1 do
    begin
      for k := i to (i + 8) - 1 do //top left
      begin
        line := pByteArray(img.ScanLine[k]);
        for l := j to (j + 8) - 1 do
        begin
          saturnTiles[count] := line[l];
          Inc(count);
        end;
      end;

      for k := i to (i + 8) - 1 do //top right
      begin
        line := pByteArray(img.ScanLine[k]);
        for l := j + 8 to (j + 16) - 1 do
        begin
          saturnTiles[count] := line[l];
          Inc(count);
        end;
      end;

      for k := i + 8 to (i + 16) - 1 do //bottom left
      begin
        line := pByteArray(img.ScanLine[k]);
        for l := j to (j + 8) - 1 do
        begin
          saturnTiles[count] := line[l];
          Inc(count);
        end;
      end;

      for k := i + 8 to (i + 16) - 1 do //bottom right
      begin
        line := pByteArray(img.ScanLine[k]);
        for l := j + 8 to (j + 16) - 1 do
        begin
          saturnTiles[count] := line[l];
          Inc(count);
        end;
      end;
      j := j + 16; //position: top pixel + 16
    end;
    i := i + 16; //inc y pos
  end;


  if outType = 0 then
  begin
    varName := copy(input, 0, pos('.', input) - 1);
    outputC(outFile, varName, saturnTiles, saturnPal);
  end;
//  else
//    outputBin(saturnImg);
  img.Destroy;
  //end;
  CloseFile(outFile);
end;

begin
  //set up defaults
  outType := 0;
  if ParamCount > 0 then
  begin //process command line parameters
    for i := 1 to ParamCount do
    if paramstr(i)[1] = '-' then
    begin
      begin
        if paramstr(i)[2] = 'o' then
        begin
          if paramstr(i)[3] = 'c' then
            outType := 0
          else if paramstr(i)[3] = 'b' then
            outType := 1
          else
          begin
            printHelp;
            exit;
          end
        end
        else
        begin
          printHelp;
          exit;
        end;
      end;
    end
    else
    begin
      filename := paramstr(i);
    end;
  handleImage(filename, outType);
  end
  else
    printHelp;
end.


