package horus.datamining.wrapper;

import java.time.DayOfWeek;
import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;


public final class PurchasePricePredictionWrapper
{
	private static Environment environment;
	private static Model model;

	
	public static void setModelPath(String modelPath) throws Exception
	{
		environment = new EnvironmentImpl();
		environment.setModelPath(modelPath);
		model = new PurchasePricePrediction(environment);
	}


	// Input: year - numeric, month - numeric, day - numeric
	// Output: Price - numeric
	public static double[] predictPrice(int year, int month, int day)
	{
		FeatureVector featureVector = model.createFeatureVector();
		LocalDate date = LocalDate.of(year, month, day);
		featureVector.setValue("Year", date.getYear());
		featureVector.setValue("Month", date.getMonthValue());
		featureVector.setValue("Day", date.getDayOfMonth());
		int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("WeekDay", dayOfWeek);

		double[] result = null;
		try
		{
			Suggestion suggestion = model.solve(featureVector);
			result = new double[1];
			result[0] = (double) suggestion.getFieldValue("Price");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	
	public static void test() throws Exception
	{
		PurchasePricePredictionWrapper.setModelPath("D:/my-git/data-mining/DataMining/models/");
		double[] result = PurchasePricePredictionWrapper.predictPrice(2017, 4, 1);
		System.out.println(result[0]);
	}
}
