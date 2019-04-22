program SaturnConv;
uses
  SysUtils, Graphics;
var
  i,
  outType, //c file (0) or binary (1)
  fileCount:
  Integer;
  files: array of String;
type WordMatrix = array of array of Word;
procedure printHelp;
begin
  WriteLn('Usage: SaturnConv [rgb bitmap].bmp [outputfile]');
  WriteLn('Optional parameters:');
  WriteLn('-oc or -ob: output to c file or binary (default: c file)');
end;

procedure outputC(filename: String; img: WordMatrix);
var
  i, j, index: Integer;
  cFile: TextFile;
  arrayName: String;
begin
  AssignFile(cFile, filename);
  ReWrite(cFile);
  //split off variable name
  if LastDelimiter('\', filename) <> 0 then
    index := LastDelimiter('\', filename) + 1 //split of
  else
    index := 1;
  arrayName := copy(filename, index, pos('.', filename) - index);
  WriteLn(cFile, 'const Uint16 ', arrayName, '[] = {');
  for i := 0 to Length(img) - 1 do
  begin
    for j := 0 to Length(img[0]) - 1 do
    begin
      if (j < Length(img[0]) - 1) or (i < Length(img) - 1) then
        Write(cFile, img[i][j], ',')
      else
        Write(cFile, img[i][j]);
    end;
    Write(cFile, #13#10); //new line
  end;
  WriteLn(cFile, '};');
  CloseFile(cFile);
end;

procedure handleImage(input: array of String; outType: Integer);
var
  img: TBitmap;
  line: pByteArray;
  saturnImg: WordMatrix; //array holding the converted image
  //Saturn image format:
  //0123456789ABCDEF
  //BBBBBGGGGGRRRRR1
  i, j, count: Integer;
  r, g, b: Word;
begin
  img := TBitmap.Create;
  img.LoadFromFile(input);
  SetLength(saturnImg, img.Height, img.Width);

  for i:=0 to (img.Height - 1) do
  begin
    line := pByteArray(img.ScanLine[i]);
    j := 0;
    count := 0;
    while j < (img.Width * 3) - 1 do
    begin
      r := line[j] div 8; //8 bpp to 5 bpp
      g := line[j+1] div 8;
      b := line[j + 2] div 8;
      WriteLn('r in: ', line[j], ' out: ', r);
      WriteLn('g in: ', line[j + 1], ' out: ', g);
      WriteLn('b in: ', line[j + 2], ' out: ', b);
      saturnImg[i][count] := ($8000 or b or (g shl 5) or (r shl 10));
      WriteLn('out: ', saturnImg[i][count]);
      j := j + 3;
      count := count + 1;
    end;
  end;
  if outType = 0 then
    outputC(output, saturnImg);
//  else
//    outputBin(saturnImg);
  img.Destroy;
end;

begin
  //set up defaults
  outType := 0;
  fileCount := 0;
  if ParamCount > 0 then
  begin //process command line parameters
    SetLength(files, ParamCount - 1);
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
      files[fileCount] := paramstr(i);
      fileCount := fileCount + 1;
    end;
  handleImage(infile, outfile, outType);
  end
  else
    printHelp;
end.


