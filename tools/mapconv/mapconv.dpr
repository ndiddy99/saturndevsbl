program MapConv;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils, ActiveX, XMLIntf, XMLDoc;
type WordMatrix = array of array of Uint16;
procedure outputC(name: String; map: WordMatrix);
var
  i, j: Integer;
  outFile: TextFile;
begin
  AssignFile(outFile, 'out.c');
  ReWrite(outFile);
  WriteLn(outFile, 'const Uint16 ', name, '[', Length(map), '][', Length(map[0]), '] = {');
  for i := 0 to Length(map) - 1 do
  begin
    for j := 0 to Length(map[0]) - 1 do
    begin
      if j = 0 then
        Write(outFile, '  {', map[i][j], ',')
      else
        Write(outFile, map[i][j], ',');
    end;
    Write(outFile, '},', #13#10);
  end;
  WriteLn(outFile, '};');
  CloseFile(outFile);
end;
procedure handleMap(input: String);
var
  tiledFile: IXMLDocument;
  element: IXMLNode;
  val, currElement: String;
  width, height, i, j, count, strIndex: Integer;
  map: array of array of Uint32;
  saturnMap: WordMatrix;
begin
  width := 0;
  height := 0;
  CoInitialize(nil);
  tiledFile := TXMLDocument.Create(nil);
  tiledFile.LoadFromFile(input);
  element := tiledFile.ChildNodes.FindNode('map');
  if element <> nil then
  begin
    if element.HasAttribute('width') then //get mape width & height
    begin
      val := element.Attributes['width'];
      width := strtoint(val);
    end;
    if element.HasAttribute('height') then
    begin
      val := element.Attributes['height'];
      height := strtoint(val);
    end;
    Writeln('width: ', width, ' height: ', height);
    SetLength(map, height, width);
    SetLength(saturnMap, height, width);
    element := element.ChildNodes.FindNode('layer');
    element := element.ChildNodes.FindNode('data'); //where the map is stored
    val := element.Text;
    i := 0;
    j := 0;
    count := 1;
    //convert map from csv to array
    while count < val.Length do
    begin
      strIndex := 1;
      currElement := '          '; //max length of a 32 bit int
      while val[count] <> ',' do
      begin
        currElement[strIndex] := val[count];
        Inc(strIndex);
        Inc(count);
        if count > val.Length then
          break;
      end;
      Inc(count);
      currElement := StringReplace(currElement, #13, '', [rfReplaceAll]);
      currElement := StringReplace(currElement, #10, '', [rfReplaceAll]);
      currElement := StringReplace(currElement, ' ', '', [rfReplaceAll]);
//      Writeln(currElement);
      map[i][j] := StrToUint(currElement);
//      Writeln('map row ', i, ' col ', j, ' val ', map[i][j]);
      Inc(j);
      if j = width then
      begin
        j := 0;
        Inc(i);
      end;
    end;
    //convert array to saturn graphics format
    for i := 0 to height - 1 do
    begin
      for j := 0 to width - 1 do
        begin
          saturnMap[i][j] := (map[i][j] and $3ff) - 1; //tiled stores maps 1 indexed for some reason
          if (map[i][j] and $80000000) = $80000000 then  //is tile horizontally flipped?
            saturnMap[i][j] := saturnMap[i][j] or $400;
          if (map[i][j] and $40000000) = $40000000 then //is vertically flipped?
            saturnMap[i][j] := saturnMap[i][j] or $800;
        end;
    end;
    outputC('tiles', saturnMap);

  end

  else
    Writeln('shit');

end;

begin
  if ParamCount > 0 then
  begin
    handleMap(paramstr(1));
  end
  else
    WriteLn('Usage: mapconv [tiled name].tmx');
end.
