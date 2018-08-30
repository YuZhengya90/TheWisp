package horus.datamining.test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.*;


public class Goal3Reader
{
	public static class Item
	{
		public LocalDate date;
		public double temperature;
		public double salePrice;
	}

	
	public static final String defaultDataFile = "D:/my-git/data-mining/DataMining/delivery/goal_3-input.txt";
	
	
	public static List<Item> readData(String file) throws IOException
	{
		List<String> content = Files.readAllLines(Paths.get(file));
		List<Item> result = new LinkedList<Item>();
		for (String line : content)
		{
			Scanner scanner = new Scanner(line);
			Item item = new Item();
			
			String dateString  = scanner.next();
			item.date = LocalDate.parse(dateString);
			item.temperature = scanner.nextDouble();
			item.salePrice = scanner.nextDouble();
			
			result.add(item);
			scanner.close();
		}
		return result;
	}
	
	
	// Test
	public static void main(String[] args)
	{
		try
		{
			List<Item> items = readData(defaultDataFile);
			for (Item item : items)
			{
				System.out.println(item.date + ", " + item.temperature + ", " + item.salePrice);
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}
}
