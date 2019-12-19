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
                    spriteConverter.addImage(fileList[0]);
                    spriteConverter.writeInfo(fileList[0], line.substring(2) + ".c");
                    for (int i = 1; i < fileList.length; i++) {
//                        System.out.println(i);
                        spriteConverter.addImage(fileList[i]);
                    }
                    spriteConverter.writeImages(line.substring(2) + ".spr");
                }
                //tiles
                else if (line.charAt(0) == 't') {

                }
                //tiled map
                else if (line.charAt(0) == 'm') {
                    MapReader mapReader = new MapReader(line.substring(2));
                    mapReader.outputMap(line.substring(2, line.indexOf('.')) + ".c");
                }
                line = reader.readLine();
            }
        } catch (Exception e) { e.printStackTrace(); }
    }
}
