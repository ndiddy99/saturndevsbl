import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class SatConv {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Usage: neoconv [list].txt");
            return;
        }
        System.out.println(args[0]);
        File listFile = new File(args[0]);
        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader(listFile));
            String line = reader.readLine();
            while (line != null) {
                System.out.println(line);
                //sprite folder
                if (line.charAt(0) == 's') {
                    File folder = new File(line.substring(2));
                    File[] fileList = folder.listFiles();
                    SpriteConverter spriteConverter = new SpriteConverter();
                    for (int i = 0; i < fileList.length; i++) {
//                        System.out.println(i);
                        spriteConverter.addImage(fileList[i]);
                    }
                    spriteConverter.writeInfo(fileList[0], line.substring(2) + ".c");
                    spriteConverter.writeImages(line.substring(2) + ".spr");
                }
                //4bpp tiles
                if (line.charAt(0) == 't') {
                    if (line.charAt(1) == '4') {
                        File tiles = new File(line.substring(3));
                        TileConverter tileConverter = new TileConverter(tiles, 4);
                        tileConverter.writeInfo(line.substring(3, line.indexOf('.')) + ".c");
                        tileConverter.writeTiles(line.substring(3, line.indexOf('.')) + ".tle");
                    }
                    //8bpp tiles
                    else if (line.charAt(1) == '8') {
                        File tiles = new File(line.substring(3));
                        TileConverter tileConverter = new TileConverter(tiles, 8);
                        tileConverter.writeInfo(line.substring(3, line.indexOf('.')) + ".c");
                        tileConverter.writeTiles(line.substring(3, line.indexOf('.')) + ".tle");
                    }
                }
                //tiled map
                else if (line.charAt(0) == 'm') {
                    if (line.charAt(1) == '4') {
                        MapReader mapReader = new MapReader(line.substring(3), 4);
                        mapReader.outputMap(line.substring(3, line.indexOf('.')) + ".map");
                        mapReader.writeInfo(line.substring(3, line.indexOf('.')) + ".c");
                    }
                    else if (line.charAt(1) == '8') {
                        MapReader mapReader = new MapReader(line.substring(2), 8);
                        mapReader.outputMap(line.substring(3, line.indexOf('.')) + ".map");
                        mapReader.writeInfo(line.substring(3, line.indexOf('.')) + ".c");
                    }
                }
                line = reader.readLine();
            }
        } catch (Exception e) { e.printStackTrace(); }
    }
}
