package horus.datamining.wrapper;

import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.Model;
import horus.datamining.model.SaleCommentsPrediction;
import horus.datamining.model.Suggestion;
import horus.datamining.model.feature.FeatureVector;

public class TemperaturePredictionWrapper
{
	private static Environment environment;
	private static Model model;
	
	
	public static void setModelPath(String modelPath) throws Exception
	{
		environment = new EnvironmentImpl();
		environment.setModelPath(modelPath);
		model = new SaleCommentsPrediction(environment);
	}
	
	
	// Input: year - numeric, month - numeric, day - numeric	(in 2017)
	// Output: Temperature - numeric
	public static double[] predictTemperature(int year, int month, int day)
	{
		FeatureVector featureVector = model.createFeatureVector();
		LocalDate date = LocalDate.of(year, month, day);
		featureVector.setValue("Year", date.getYear());
		featureVector.setValue("DayOfYear", date.getDayOfYear());
		
		double[] result = null;
		try
		{
			Suggestion suggestion = model.solve(featureVector);
			result = new double[1];
			result[0] = (double) suggestion.getFieldValue("Comments");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	
	public static void test() throws Exception
	{
		setModelPath("D:/my-git/data-mining/DataMining/models/");
		double[] result = predictTemperature(2017, 4, 1);
		System.out.println(result[0]);
	}
}
