package horus.datamining.test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.LinkedList;
import java.util.List;


public class Goal1Reader
{
	public static final String defaultDataFile = "D:/my-git/data-mining/DataMining/delivery/goal_1-input.txt";
	
	
	public static List<LocalDate> readData(String file) throws IOException
	{
		List<String> content = Files.readAllLines(Paths.get(file));
		List<LocalDate> result = new LinkedList<LocalDate>();
		for (String line : content)
		{
			LocalDate date = LocalDate.parse(line);
			result.add(date);
		}
		return result;
	}


	// Test
	public static void main(String[] args)
	{
		try
		{
			List<LocalDate> dates = readData(defaultDataFile);
			for (LocalDate date : dates)
			{
				System.out.println(date);
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}
}
