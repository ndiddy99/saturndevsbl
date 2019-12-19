import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.PrintWriter;
import java.util.Scanner;

public class MapReader {
    //palettes used for each tile
    private int[] palettes = new int[]{0x100, 0x100};
    private int[][] mapArr;

    public MapReader(String filename) {
        File mapFile = new File(filename);
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        DocumentBuilder db = null;
        Document mapDocument = null;
        try {
            db = dbf.newDocumentBuilder();
            mapDocument = db.parse(mapFile);
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }
        mapDocument.getDocumentElement().normalize();
        Element map = mapDocument.getDocumentElement();
        System.out.println(map.getNodeName());
        int width = Integer.parseInt(map.getAttribute("width"));
        int height = Integer.parseInt(map.getAttribute("height"));
        System.out.println("width: " + width);
        System.out.println("height: " + height);
        Element layer = (Element) map.getElementsByTagName("layer").item(0);
        Element data = (Element) layer.getElementsByTagName("data").item(0);
        String mapStr = data.getTextContent();
        mapArr = new int[height][width];
        Scanner scanner = new Scanner(mapStr);
        int rowCount = 0;
        while (scanner.hasNext()) {
            String line = scanner.nextLine();
            while (line.equals("") && scanner.hasNext()) {
                line = scanner.nextLine();
            }
            String[] elements = line.split(",");
            for (int i = 0; i < elements.length; i++) {
                if (!elements[i].equals("")) {
                    mapArr[rowCount][i] = Integer.parseInt(elements[i]) - 1;
                }
            }
            rowCount++;
            if (rowCount == height) {
                break;
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                System.out.print(mapArr[i][j] + " ");
            }
            System.out.println();
        }
        scanner.close();
    }

    public void outputMap(String filename) {
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(filename);
        }
        catch (Exception e) {
            e.printStackTrace();
            return;
        }
        writer.println(filename.substring(filename.lastIndexOf('/') + 1, filename.indexOf('.')) + ":");
        for (int i = 0; i < mapArr[0].length; i++) {
            for (int j = 0; j < mapArr.length; j++) {
                int tile = mapArr[j][i];
                writer.print("\tdc.w ");
                writer.print(String.format("$%04X,", tile));
                writer.println(String.format("$%04X", palettes[tile]));
            }
        }
        writer.close();
    }

    public void writeInfo(String filename) {
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(filename);
        }
        catch (Exception e) {
            e.printStackTrace();
            return;
        }
        String varName = filename.substring(filename.lastIndexOf('/') + 1, filename.indexOf('.'));
        writer.println("const Uint16 " + varName + "_width = " + mapArr[0].length);
        writer.println("const Uint16 " + varName + "_height = " + mapArr.length);
        writer.println("const char " + varName + "_filename[] = \"" + varName + ".map\";");
        writer.close();
    }
}